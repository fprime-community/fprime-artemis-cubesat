module Components {

    enum OpModes {
        Startup,
        Deployment,
        Initialization,
        Nominal,
        DataTransmit,
        SafeMode,
        PowerEmergency,
        Restart
    }

    port OpMode(ref mode: OpModes)

    @ Component that controls operational modes
    passive component ModeManager {
        
        # ----------------------------------------------------------------------
        # General Ports
        # ----------------------------------------------------------------------

        @ Port: receiving calls from the rate group
        sync input port run: Svc.Sched

        @ Port to get operation mode
        sync input port getOpMode: Components.OpMode

        sync input port tlmSend: Svc.Sched

        @ Port to read battery power
        output port getBatteryPower: Sensors.powerData

        # ----------------------------------------------------------------------
        # Telemetry channels
        # ----------------------------------------------------------------------
        
        telemetry CurrentOpMode: Components.OpModes

        # ----------------------------------------------------------------------
        # Commands  
        # ----------------------------------------------------------------------

        sync command SetMode(mode: Components.OpModes)

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