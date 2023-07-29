module Radios {
    @ Component for RFM23BPS Radio
    passive component RFM23 {

        @ Data coming in from the framing component
        sync input port comDataIn: Drv.ByteStreamSend

        @ Status of the last radio transmission
        output port comStatus: Fw.SuccessCondition

        @ Com data passing back out
        output port comDataOut: Drv.ByteStreamRecv

        # ----------------------------------------------------------------------
        # Implementation ports
        # ----------------------------------------------------------------------

        @ Allows for deallocation of radio command communications
        output port deallocate: Fw.BufferSend

        @ Allows for allocation of buffers
        output port allocate: Fw.BufferGet

          # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------

        @ Telemetry channel for radio status
        telemetry Status: Fw.On

        @ Telemetry channel counting packets sent
        telemetry NumPacketsSent: U16

        @ Telemetry channel counting packets received
        telemetry NumPacketsReceived: U16

        @ Telemetry channel for radio RSSI
        telemetry RSSI: I16

         @ Telemetry channel for radio temp
        telemetry temp: I32

        @ Prints received packet payload
        event PayloadMessageTX(msg: U32) \
            severity diagnostic \
            format "Payload Size Sent: {}"

        @ Prints received packet payload
        event PayloadMessageRX(msg: U32) \
            severity diagnostic \
            format "Payload Size Recevied: {}"
        
        # ----------------------------------------------------------------------
        # Special ports
        # ----------------------------------------------------------------------

        @ Port receiving calls from the rate group
        sync input port run: Svc.Sched

        @ Port sending calls to the GPIO driver
        output port gpioSetRxOn: Drv.GpioWrite

        @ Port sending calls to the GPIO driver
        output port gpioSetTxOn: Drv.GpioWrite

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

    }
}