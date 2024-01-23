module Sensors {

    array GPSTlm = [7] F32

    @ Component for the Adafruit Mini GPS PA1010D Module
    passive component PA1010D {
    
        # ----------------------------------------------------------------------
        # General Ports
        # ----------------------------------------------------------------------

        @ Port: receiving calls from the rate group
        sync input port run: Svc.Sched

        # ----------------------------------------------------------------------
        # Telemetry channels
        # ----------------------------------------------------------------------

        @ GPS Data
        telemetry Time: GPSTlm
        telemetry Date: GPSTlm
        telemetry FixQuality: GPSTlm
        telemetry Satellites: GPSTlm
        telemetry Speed: GPSTlm
        telemetry Angle: GPSTlm
        telemetry Altitude: GPSTlm

        # ----------------------------------------------------------------------
        # Commands  
        # ----------------------------------------------------------------------

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

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

    }
}