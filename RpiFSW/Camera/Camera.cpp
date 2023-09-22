// ======================================================================
// \title  Camera.cpp
// \author ethan
// \brief  cpp file for Camera component implementation class
// ======================================================================

#include <RpiFSW/Camera/Camera.hpp>
#include <FpConfig.hpp>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>

namespace Payload
{

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  Camera ::
      Camera(
          const char *const compName) : CameraComponentBase(compName),
                                        m_photoCount(0)
  {
  }

  Camera ::
      ~Camera()
  {
  }

  bool Camera::open(I32 deviceIndex)
  {
    if (camManager->start() < 0)
    {
      this->log_WARNING_HI_CameraOpenError();
      return false;
    }

    if (camManager->cameras().empty())
    {
      this->log_WARNING_HI_CameraNotDetected();
      return false;
    }

    cam = camManager->cameras()[deviceIndex];
    I32 ret = cam->acquire();
    if (ret == 0 || ret == EBUSY)
    {
      this->log_ACTIVITY_HI_CameraOpenSuccess();
      return true;
    }

    this->log_WARNING_HI_CameraOpenError();
    return false;
  }

  void Camera ::parameterUpdated(FwPrmIdType id)
  {
    // Read the parameter value
    Fw::ParamValid isValid;
    ImgResolution resolution = this->paramGet_IMG_RESOLUTION(isValid);
    FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);

    if (PARAMID_IMG_RESOLUTION == id)
    {
      // update the camera configuration to use the updated IMG_RESOLUTION parameter value
      setCameraConfiguration(resolution);
    }
  }

  void Camera ::requestComplete(libcamera::Request *request)
  {
    if (request->status() == libcamera::Request::RequestCancelled)
    {
      return;
    }
    requestReceivedPtr = request;
  }

  void Camera ::allocateBuffers()
  {
    allocatorPtr = new libcamera::FrameBufferAllocator(cam);
    libcamera::Stream *stream = camConfig->at(0).stream();
    // Set up buffer for image data
    allocatorPtr->allocate(stream);
    const std::vector<std::unique_ptr<libcamera::FrameBuffer>> &buffers = allocatorPtr->buffers(stream);
    for (const std::unique_ptr<libcamera::FrameBuffer> &buffer : buffers)
    {
      size_t buffer_size = 0;
      for (unsigned i = 0; i < buffer->planes().size(); i++)
      {
        const libcamera::FrameBuffer::Plane &plane = buffer->planes()[i];
        buffer_size += plane.length;
        if (i == buffer->planes().size() - 1 || plane.fd.get() != buffer->planes()[i + 1].fd.get())
        {
          void *memory = mmap(NULL, buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, plane.fd.get(), 0);
          mappedBuffers[buffer.get()].push_back(libcamera::Span<U8>(static_cast<U8 *>(memory), buffer_size));
          buffer_size = 0;
        }
      }
      // multiple buffers may be allocated for the stream so we keep track of each
      availableFrameBuffers[stream].push(buffer.get());
    }
  }

  void Camera::configureRequests()
  {
    libcamera::Stream *stream = camConfig->at(0).stream();
    if (!availableFrameBuffers[stream].empty())
    {
      // create an empty frame capture request for the application to fill with buffers
      std::unique_ptr<libcamera::Request> request = cam->createRequest();
      if (request)
      {
        frameRequest = std::move(request);
        libcamera::FrameBuffer *buffer = availableFrameBuffers[stream].front();
        availableFrameBuffers[stream].pop();
        frameRequest->addBuffer(stream, buffer);
        cam->requestCompleted.connect(this, &Camera::requestComplete);
      }
    }
  }

  // update camera configuration based on specified resolution
  bool Camera ::setCameraConfiguration(ImgResolution resolution)
  {
    if (!cam)
    {
      this->log_WARNING_HI_CameraNotDetected();
      this->log_WARNING_HI_ImgConfigSetFail(resolution);
      return false;
    }

    // need to stop the camera before configuring it
    if (cameraStarted)
    {
      cam->stop();
      cameraStarted = false;
    }

    // configure camera
    // specify configuration type as a "StillCapture" (high resolution & high-quality still images with low frame rate)
    camConfig = cam->generateConfiguration({libcamera::StreamRole::StillCapture});
    if (!camConfig)
    {
      this->log_WARNING_HI_ImgConfigSetFail(resolution);
      return false;
    }

    // set stream height and width
    switch (resolution.e)
    {
    case ImgResolution::SIZE_640x480:
      camConfig->at(0).size.width = 640;
      camConfig->at(0).size.height = 480;
      break;
    case ImgResolution::SIZE_800x600:
      camConfig->at(0).size.width = 800;
      camConfig->at(0).size.height = 600;
      break;
    default:
      FW_ASSERT(0);
    }

    // set the pixel format
    camConfig->at(0).pixelFormat = libcamera::formats::RGB888;

    // check to see if the configuration was valid
    libcamera::CameraConfiguration::Status validationStatus = camConfig->validate();
    if (validationStatus != libcamera::CameraConfiguration::Status::Invalid)
    {
      // apply the configuration and keep track of the return code
      int returnCode = cam->configure(camConfig.get());

      // check to see if an error occurred and emit an ImgConfigSetFail event
      if (returnCode != 0)
      {
        this->log_WARNING_HI_ImgConfigSetFail(resolution);
        return false;
      }

      this->log_ACTIVITY_HI_SetImgConfig(resolution);
      return true;
    }
    else
    {
      // configuration was not valid, emit an ImgConfigSetFail event
      this->log_WARNING_HI_ImgConfigSetFail(resolution);
      return false;
    }
  }

  void Camera ::cleanup()
  {
    // remove requestCompleted signal
    cam->requestCompleted.disconnect(this, &Camera::requestComplete);
    // stop camera
    if (cameraStarted)
    {
      cam->stop();
      cameraStarted = false;
    }
    // deallocate mapped buffers
    for (auto &iter : mappedBuffers)
    {
      for (auto &span : iter.second)
      {
        munmap(span.data(), span.size());
      }
    }
    mappedBuffers.clear();
    availableFrameBuffers.clear();
    frameRequest = nullptr;
    // free buffers previously allocated for stream
    allocatorPtr->free(camConfig->at(0).stream());
    delete allocatorPtr;
    allocatorPtr = nullptr;
  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void Camera ::
      CaptureImage_cmdHandler(
          const FwOpcodeType opCode,
          const U32 cmdSeq)
  {
    if (cam == nullptr)
    {
      return;
    }

    // Read back the parameter value
    Fw::ParamValid isValid;
    ImgResolution currentResolution = this->paramGet_IMG_RESOLUTION(isValid);
    // if the IMG_RESOLUTION parameter is invalid or not set, use the default image resolution
    currentResolution = ((Fw::ParamValid::INVALID == isValid) || (Fw::ParamValid::UNINIT == isValid)) ? DEFAULT_IMG_RESOLUTION : currentResolution;

    // setup we need to do before we capture frames
    setCameraConfiguration(currentResolution);
    Fw::Buffer imgBuffer;
    allocateBuffers();
    configureRequests();

    // start the camera
    if (!cameraStarted)
    {
      cam->start();
      cameraStarted = true;
    }
    // queue request for capture
    cam->queueRequest(frameRequest.get());
    // wait a few seconds for the request to complete
    sleep(3);
    // check to see if the request was completed
    if (requestReceivedPtr && requestReceivedPtr->status() == libcamera::Request::RequestComplete)
    {
      const libcamera::Request::BufferMap &buffers = requestReceivedPtr->buffers();
      for (auto bufferPair : buffers)
      {
        libcamera::FrameBuffer *buffer = bufferPair.second;
        const libcamera::FrameMetadata &metadata = buffer->metadata();
        unsigned int bytesUsed = metadata.planes()[0].bytesused;

        // allocate memory to the framework buffer for storing frame data
        imgBuffer = allocate_out(0, bytesUsed);
        // check to see if the buffer is the correct size
        // if not, emit a InvalidBufferSizeError event
        if (imgBuffer.getSize() < bytesUsed)
        {
          this->log_WARNING_HI_InvalidBufferSizeError(imgBuffer.getSize(), bytesUsed);
          this->deallocate_out(0, imgBuffer);
          return;
        }

        // copy data to framework buffer and send it to the BufferLogger
        memcpy(imgBuffer.getData(), mappedBuffers.find(buffer)->second.back().data(), bytesUsed);
        this->save_out(0, imgBuffer);
        this->log_ACTIVITY_LO_CameraSave();
      }
    }
    // no data, send blank frame event
    else
    {
      cleanup();
      this->log_WARNING_HI_BlankFrame();
      return;
    }
    // cleanup we need to do after capturing frames
    // ie: stop camera, deallocate memory, etc.
    cleanup();
    m_photoCount++;
    this->tlmWrite_photosTaken(m_photoCount);

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

} // end namespace Payload