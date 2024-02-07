module Components {

    @ Component for battery heater
    passive component Heater {

        @ Internal PDU set switch
        output port PDUSetSwitch: Components.PDU_SW_CMD

        @ Internal PDU get switch
        output port PDUGetSwitch: Components.PDU_GET_SW_CMD

        @ Internal read temperature sensor
        sync input port BatteryTemp: Sensors.TempVal

        @ Port: receiving calls from the rate group
        sync input port run: Svc.Sched

        @ Port to get current Operation Mode
        output port getOpMode: Components.OpMode

        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------
 
        @ Heater Status 0 = OFF 1 = ON
        telemetry HeaterStatus: Fw.On

        # ----------------------------------------------------------------------
        # Events
        # ----------------------------------------------------------------------

        @ Heater is on/off
        event HeaterState(on_off: Fw.On) \
        severity activity high \
        format "Heater is: {}"

        event HeaterDenied(mode: Components.OpModes) \
        severity warning high \
        format "Spacecraft is in {}"

        # ----------------------------------------------------------------------
        # Commands  
        # ----------------------------------------------------------------------

        @ Command to turn on/off Heater
        sync command SetHeater(state: Fw.On)

        @ Command to turn off heater automation
        sync command ToggleAuto(state: Fw.On)

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