// ======================================================================
// \title  Camera.hpp
// \author ethan
// \brief  hpp file for Camera component implementation class
// ======================================================================

#ifndef Camera_HPP
#define Camera_HPP

#include "RpiFSW/Camera/CameraComponentAc.hpp"
#include <libcamera/libcamera.h>
#include <queue>

namespace Payload
{

  class Camera : public CameraComponentBase
  {

  public:
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object Camera
    //!
    Camera(
        const char *const compName /*!< The component name*/
    );

    //! Destroy object Camera
    //!
    ~Camera();

    //! parameter updates
    //!
    void parameterUpdated(FwPrmIdType id);

    //! Startup camera device
    //!
    bool open(I32 deviceIndex = 0);

    //! allocate buffers
    //!
    void allocateBuffers();

    //! create or re-use request
    //!
    void configureRequests();

    //! update camera configuration based on specified resolution
    //!
    bool setCameraConfiguration(ImgResolution resolution);

    //! stop camera, deallocate memory, etc.
    //!
    void cleanup();

    PRIVATE :

        void
        requestComplete(libcamera::Request *request);

    // ----------------------------------------------------------------------
    // Command handler implementations
    // ----------------------------------------------------------------------

    //! Implementation for CaptureImage command handler
    //! Capture image and save the raw data
    void CaptureImage_cmdHandler(
        const FwOpcodeType opCode, /*!< The opcode*/
        const U32 cmdSeq           /*!< The command sequence number*/
    );

    PRIVATE :

        const ImgResolution DEFAULT_IMG_RESOLUTION = ImgResolution::SIZE_640x480;

    std::shared_ptr<libcamera::Camera> cam;
    std::unique_ptr<libcamera::CameraManager> camManager = std::make_unique<libcamera::CameraManager>();
    std::unique_ptr<libcamera::CameraConfiguration> camConfig;
    libcamera::Request *requestReceivedPtr;
    libcamera::FrameBufferAllocator *allocatorPtr = nullptr;
    std::unique_ptr<libcamera::Request> frameRequest;
    std::map<libcamera::FrameBuffer *, std::vector<libcamera::Span<U8>>> mappedBuffers;
    std::map<libcamera::Stream *, std::queue<libcamera::FrameBuffer *>> availableFrameBuffers;

    bool cameraStarted = false;
    U32 m_photoCount;
  };

} // end namespace Payload

#endif