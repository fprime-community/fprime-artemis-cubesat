module Components {

    @ Component for battery heater
    passive component Heater {

        @ Data coming in from the framing component
        sync input port comDataIn: Drv.ByteStreamRecv

        @ Status of the last radio transmission
        output port comStatus: Fw.SuccessCondition

        @ Com data passing back out
        output port comDataOut: Drv.ByteStreamSend

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

        # ----------------------------------------------------------------------
        # Commands  
        # ----------------------------------------------------------------------

        @ Command to turn on/off Heater
        sync command SetHeater(
                                sw: Components.PDU_SW
                                state: Fw.On
                              )

        @ Command to get statuses of Heater switch
        sync command GetHeater()

        @ Command to automatically control the Heater based on temperature
        sync command AutoControlHeater(temperature: F32)

        # ----------------------------------------------------------------------
        # Parameters
        # ----------------------------------------------------------------------

        # @ Example parameter
        # param PARAMETER_NAME: U32

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