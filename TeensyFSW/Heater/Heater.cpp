// ======================================================================
// \title  Heater.cpp
// \author root
// \brief  cpp file for Heater component implementation class
// ======================================================================

#include <FpConfig.hpp>
#include <TeensyFSW/Heater/Heater.hpp>
#include <TeensyFSW/PDU/PDU.hpp>

namespace Components {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Heater ::Heater(const char* const compName) : HeaterComponentBase(compName) {}

Heater ::~Heater() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Heater ::comDataIn_handler(const NATIVE_INT_TYPE portNum,
                                Fw::Buffer& recvBuffer,
                                const Drv::RecvStatus& recvStatus) {
    // TODO
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void Heater ::SetHeater_cmdHandler(const FwOpcodeType opCode,
                                   const U32 cmdSeq,
                                   Components::PDU_SW sw,
                                   Fw::On state) {
    pdu_packet packet;
    packet.type     = PDU_Type::CommandSetSwitch;
    packet.sw       = (Components::PDU::PDU_SW)(U8)sw;
    packet.sw_state = (state == Fw::On::ON) ? 1 : 0;

    if (packet.sw == Components::PDU::PDU_SW::RPI) {
        this->rpiGpioSet_out(0, (packet.sw_state == 1) ? Fw::Logic::HIGH : Fw::Logic::LOW);
        Fw::Logic state;
        this->rpiGpioRead_out(0, state);
        telem[12] = state;
        this->tlmWrite_SwitchStatus(telem);
    } else {
        send(packet);
    }
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void Heater ::GetHeater_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq) {
    pdu_packet packet;
    packet.type = PDU_Type::CommandGetSwitchStatus;
    packet.sw   = PDU_SW::SW_5V_2;

    send(packet);
    Fw::Logic state;
    telem[12] = state;
    this->tlmWrite_SwitchStatus(telem);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void Heater ::AutoControlHeater_cmdHandler(const FwOpcodeType opCode,
                                           const U32 cmdSeq,
                                           F32 temperature) {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // end namespace Components
