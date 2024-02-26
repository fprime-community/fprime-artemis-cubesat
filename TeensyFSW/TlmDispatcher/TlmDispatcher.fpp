module Components {
    @ Component to check if satellite is in data transmit mode to determine wether or not to send telemetry down.
    passive component TlmDispatcher {

    @ Port to get current Operation Mode
    output port getOpMode: Components.OpMode

    output port comOut: Fw.Com

    @ Port to 
    guarded input port comIn: Fw.Com 
    }
}