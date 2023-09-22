module Payload {

    enum ImgResolution { SIZE_640x480 = 0 , SIZE_800x600 = 1 }

    @ Raspberry Pi Camera Component
    active component Camera {

        @ Capture image and save the raw data
        async command CaptureImage()

        # ----------------------------------------------------------------------
        # General ports
        # ----------------------------------------------------------------------

        @ Allocates memory to hold photo buffers
        output port allocate: Fw.BufferGet

        @ Deallocates memory that holds photo buffers
        output port deallocate: Fw.BufferSend

        @ Save photo to disk, send image to buffer logger
        output port $save: Fw.BufferSend

        # -----------------------------------------------------------------------------
        # Events
        # -----------------------------------------------------------------------------

        @ Event where no camera was detected
        event CameraNotDetected \
            severity warning high \
            format "No cameras were detected" \

        @ Event where error occurred when setting up camera
        event CameraOpenError \
            severity warning high \
            format "Camera failed to open" \

        @ Event where Camera successfully opened
        event CameraOpenSuccess \
            severity activity high \
            format "Camera opened!" \

        @ Event image configuration has been set
        event SetImgConfig(
            resolution: ImgResolution @< Image size
        ) \
            severity activity high \
            format "The image resolution has been set to {}" \

        @ Failed to set size and color format
        event ImgConfigSetFail(
            resolution: ImgResolution @< Image size
        ) \
            severity warning high \
            format "Image resolution of {} failed to set" \

        @ Camera failed to capture image
        event CameraCaptureFail \
            severity warning high \
            format "Camera failed to capture image"

        @ Camera successfully saved image
        event CameraSave \
            severity activity low \
            format "Image was saved"

        @ Blank frame Error
        event BlankFrame \
            severity warning high \
            format "Error: Blank frame was grabbed" \

        @ Invalid buffer size error
        event InvalidBufferSizeError(
            imgBufferSize: U32  @< size of imgBuffer to hold image data
            imgSize: U32        @< size of image
        ) \
            severity warning high \
            format "imgBuffer of size {} is less than imgSize of size {}"

        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------

        @ Total number of files captured
        telemetry photosTaken: U32 id 0 update on change

        # ----------------------------------------------------------------------
        # Parameters
        # ----------------------------------------------------------------------
        
        @ Image resolution that the camera should be configured for
        param IMG_RESOLUTION: ImgResolution

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}