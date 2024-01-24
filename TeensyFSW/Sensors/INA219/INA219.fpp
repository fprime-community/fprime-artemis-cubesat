module Sensors {

    array CurrentTlm = [3] F32

    @ Component for the INA219 current sensors
    passive component INA219 {

        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------
        
        @ Voltage, Current, and Power Data
        telemetry CurrentSensorData: CurrentTlm

        @ Port: receiving calls from the rate group
        sync input port run: Svc.Sched

        # @ Example parameter
        # param PARAMETER_NAME: U32

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