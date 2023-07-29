module Sensors {

    struct MAG_Measurement {
    mag_x: F32
    mag_y: F32
    mag_z: F32
  }

    @ Component for the Adafruit LIS3MDL
    passive component LIS3MDL {

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        # @ Example async command
        # async command COMMAND_NAME(param_name: U32)

        # @ Example telemetry counter
        # telemetry ExampleCounter: U64

        # @ Example event
        # event ExampleStateEvent(example_state: Fw.On) severity activity high id 0 format "State set to {}"

        # @ Example port: receiving calls from the rate group
        # sync input port run: Svc.Sched

        @ Scheduler output port
        sync input port SchedIn: Svc.Sched

        # @ Example parameter
        # param PARAMETER_NAME: U32

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
        telemetry MAG_Telem: MAG_Measurement id 0

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}