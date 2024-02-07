module Sensors {

    array MagTlm = [3] F32

    @ Component for the Adafruit LIS3MDL
    passive component LIS3MDL {

        # ----------------------------------------------------------------------
        # Telemetry
        # ----------------------------------------------------------------------
        
        @ Magnetometer Data
        telemetry Data: MagTlm

        @ Scheduler output port
        sync input port run: Svc.Sched

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
      
        @ Port for requesting the current time
        time get port timeCaller

        # @ Port for sending command registrations
        # command reg port cmdRegOut

        # @ Port for receiving commands
        # command recv port cmdIn

        # @ Port for sending command responses
        # command resp port cmdResponseOut

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