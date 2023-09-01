module Components {

    enum PDU_SW {
        None,
        All,
        SW_3V3_1,
        SW_3V3_2,
        SW_5V_1,
        SW_5V_2,
        SW_5V_3,
        SW_5V_4,
        SW_12V,
        VBATT,
        PDU_WDT,
        HBRIDGE1,
        HBRIDGE2,
        BURN,
        BURN1,
        BURN2,
        RPI
    }

    array PDUTlm = [13] U8

    @ Component for the Artemis CubeSat's PDU
    passive component PDU {

        @ Data coming in from the framing component
        sync input port comDataIn: Drv.ByteStreamRecv

        @ Status of the last radio transmission
        output port comStatus: Fw.SuccessCondition

        @ Com data passing back out
        output port comDataOut: Drv.ByteStreamSend

        @ Port to write to RPi gpio
        output port rpiGpioSet: Drv.GpioWrite

        @ Port to read RPi gpio
        output port rpiGpioRead: Drv.GpioRead

        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------
 
        @ PDU Switch Statuses 0 = OFF 1 = ON
        telemetry SwitchStatus:  PDUTlm update always

        # ----------------------------------------------------------------------
        # Events
        # ----------------------------------------------------------------------

        @ PDU connection successful
        event PduPong() \
        severity activity high \
        format "PDU is connected"

        # ----------------------------------------------------------------------
        # Commands  
        # ----------------------------------------------------------------------

        @ Command to turn on/off PDU switch
        sync command SetSwitch(
                                sw: Components.PDU_SW
                                state: Fw.On
                              )

        @ Command to get statuses of PDU switches
        sync command GetSwitch()

        @ Command to Ping PDU
        sync command Ping()

        # ----------------------------------------------------------------------
        # Implementation ports
        # ----------------------------------------------------------------------

        @ Allows for deallocation of PDU command communications
        output port deallocate: Fw.BufferSend

        @ Allows for allocation of buffers
        output port allocate: Fw.BufferGet

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
    }
}