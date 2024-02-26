module Sensors {

port TempVal(val:F32);

    @ Component for the TMP36 temperature sensors
    passive component TMP36 {

        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------
        
        @ Temperature
        telemetry TemperatureSensorData: F32

        # ----------------------------------------------------------------------
        # Implementation ports
        # ----------------------------------------------------------------------

        @ Port: receiving calls from the rate group
        sync input port run: Svc.Sched

        @ Port: 
        output port readAnalog: Drv.AnalogRead

        @ Port: 
        output port temperature: Sensors.TempVal

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

    }
}