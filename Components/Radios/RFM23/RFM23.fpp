module Radios {
    @ Component for RFM23BPS Radio
    passive component RFM23 {

        @ Data coming in from the framing component
        sync input port comDataIn: Drv.ByteStreamSend

        @ Status of the last radio transmission
        output port comStatus: Fw.SuccessCondition

        @ Com data passing back out
        output port comDataOut: Drv.ByteStreamRecv

        @ Port to get current Operation Mode
        output port getOpMode: Components.OpMode  

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


        # ----------------------------------------------------------------------
        # Events
        # ----------------------------------------------------------------------

        @ Prints received packet payload
        event PayloadMessageTX(msg: U32) \
            severity diagnostic \
            format "Payload Size Sent: {}"

        @ Prints received packet payload
        event PayloadMessageRX(msg: U32) \
            severity diagnostic \
            format "Payload Size Recevied: {}"

        event RadioReset(sec: U32) \
            severity warning high \
            format "Radio was off for {} seconds. Resetting radio now."
        
        # ----------------------------------------------------------------------
        # Special ports
        # ----------------------------------------------------------------------

        @ Port receiving calls from the rate group
        sync input port run: Svc.Sched

        @ Port receiving calls from the rate group
        sync input port healthCheck: Svc.Sched

        @ Port sending calls to the GPIO driver
        output port gpioSetRxOn: Drv.GpioWrite

        @ Port sending calls to the GPIO driver
        output port gpioSetTxOn: Drv.GpioWrite

        @ Internal PDU set switch
        output port PDUSetSwitch: Components.PDU_SW_CMD

        @ Internal PDU get switch
        output port PDUGetSwitch: Components.PDU_GET_SW_CMD

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