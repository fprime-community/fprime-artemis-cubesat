module Components {

    enum PDU_SW {
        None,
        All,
        SW_3V3_1,
        RFM23_RADIO,
        SW_5V_1,
        HEATER,
        SW_5V_3,
        SW_12V,
        VBATT,
        BURN1,
        BURN2,
        RPI
    }

    enum TRQ_SELECT {
        TRQ1,
        TRQ2,
        TRQ1A,
        TRQ1B,
        TRQ2A,
        TRQ2B,
    }

    enum TRQ_CONFIG {
        SLEEP,
        MOTOR_COAST_FAST_DECAY,
        DIR_REVERSE,
        DIR_FORWARD,
        MOTOR_BREAK_SLOW_DECAY,
    }

    struct pduTlmStruct {
        sw: string
        state: U8
    }

    struct trqTlmStruct {
        trq: string
        mode: string
    }
    
    array PDUTlm = [11] pduTlmStruct

    array trqTlm = [6] trqTlmStruct
    
    port PDU_SW_CMD (
        sw: PDU_SW
        state: Fw.On
    )

    port PDU_GET_SW_CMD (
        ref states:Components.PDUTlm
    )

    @ Component for the Artemis CubeSat's PDU
    passive component PDU {

        @ Port: receiving calls from the rate group
        sync input port run: Svc.Sched

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

        @ Port to allow other components to control PDU switches
        sync input port SetSwitchInternal: Components.PDU_SW_CMD

        @ Port to allow other components to get PDU switch status
        sync input port GetSwitchInternal: Components.PDU_GET_SW_CMD

        @ Port: receiving calls from the rate group
        sync input port wdt: Svc.Sched

        @ Port to get current Operation Mode
        output port getOpMode: Components.OpMode  

        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------
 
        @ PDU Switch Statuses 0 = OFF 1 = ON
        telemetry SwitchStatus:  PDUTlm update always

        @ Torque Coil Satus
        telemetry TRQStatus: trqTlm update always

        # ----------------------------------------------------------------------
        # Events
        # ----------------------------------------------------------------------

        @ PDU connection successful
        event PduPong() \
        severity activity high \
        format "PDU is connected"

        event SwitchDenied(mode: Components.OpModes) \
        severity warning high \
        format "Spacecraft is in {}"

        event AllSwitchesOff () \
        severity warning high \
        format "All switches have been disabled due to low battery charge."

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

        @ Command to set TRQ mode
        sync command SetTRQ(trq: Components.TRQ_SELECT, mode: Components.TRQ_CONFIG)

        @ Command to get statuses of torque coils
        sync command GetTRQ()

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