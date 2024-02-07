// ======================================================================
// \title  PDU.cpp
// \author lukeclements
// \brief  cpp file for PDU component implementation class
// ======================================================================

#include <FpConfig.hpp>
#include <Fw/Time/Time.hpp>
#include <Os/File.hpp>
#include <TeensyFSW/PDU/PDU.hpp>

namespace Components {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

PDU::PDU(const char* const compName)
    : PDUComponentBase(compName), started(false), burnWireOn(false), isPowerEmergency(false) {
    for (NATIVE_INT_TYPE i = 0; i < sw_telem.SIZE; i++) {
        sw_telem[i].setsw(sw_lookup[i]);
        sw_telem[i].setstate(0);
    }
    for (NATIVE_INT_TYPE i = 0; i < trq_telem.SIZE; i++) {
        trq_telem[i].settrq(trqName_lookup[i]);
        trq_telem[i].setmode(trqMode_lookup[0]);
    }
}

PDU::~PDU() {}

void PDU::send(U8* buf, NATIVE_INT_TYPE len) {
    for (NATIVE_INT_TYPE i = 0; i < len; i++) {
        pdu_packet_cmd[i] = pdu_packet_cmd[i] + PDU_CMD_OFFSET;
    }
    pdu_packet_cmd[len] = '\n';

    Fw::Buffer sendBuffer = this->allocate_out(0, len + 1);
    memcpy(sendBuffer.getData(), &pdu_packet_cmd[0], len + 1);
    sendBuffer.setSize(len + 1);
    this->comDataOut_out(0, sendBuffer);
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void PDU::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    Components::OpModes opMode;
    this->getOpMode_out(0, opMode);

    // If in Startup mode and the radio switch was not previously switched on, turn on radio switch.
    if (opMode == Components::OpModes::Initialization && !this->started) {
        pdu_sw_packet packet;
        packet.type     = PDU_Type::CommandSetSwitch;
        packet.sw       = Components::PDU::PDU_SW::RFM23_RADIO;
        packet.sw_state = 1;
        memcpy(pdu_packet_cmd, &packet, sizeof(packet));
        send(pdu_packet_cmd, sizeof(packet));
        this->started = true;
    }
    // If in Deployment mode and BURN1 is off, turn on BURN1 switch.
    else if (opMode == Components::OpModes::Deployment && !this->burnWireOn) {
        pdu_sw_packet packet;
        packet.type     = PDU_Type::CommandSetSwitch;
        packet.sw       = Components::PDU::PDU_SW::BURN1;
        packet.sw_state = 1;
        memcpy(pdu_packet_cmd, &packet, sizeof(packet));
        send(pdu_packet_cmd, sizeof(packet));
        this->burnWireOn = true;
    }

    // If BURN1 is on but it is not in Deployment mode, turn BURN1 off.
    else if (opMode != Components::OpModes::Deployment && this->burnWireOn) {
        pdu_sw_packet packet;
        packet.type     = PDU_Type::CommandSetSwitch;
        packet.sw       = Components::PDU::PDU_SW::BURN1;
        packet.sw_state = 0;
        memcpy(pdu_packet_cmd, &packet, sizeof(packet));
        send(pdu_packet_cmd, sizeof(packet));
        this->burnWireOn = false;
    }

    else if (opMode == Components::OpModes::PowerEmergency && !this->isPowerEmergency) {
        this->log_WARNING_HI_AllSwitchesOff();
        pdu_sw_packet packet;
        packet.type     = PDU_Type::CommandSetSwitch;
        packet.sw       = Components::PDU::PDU_SW::All;
        packet.sw_state = 0;
        memcpy(pdu_packet_cmd, &packet, sizeof(packet));
        send(pdu_packet_cmd, sizeof(packet));
        this->isPowerEmergency = true;
    }

    else if (opMode != Components::OpModes::PowerEmergency && this->isPowerEmergency) {
        this->isPowerEmergency = false;
    }

    Fw::Logic rpiState;
    this->rpiGpioRead_out(0, rpiState);
    if (opMode == Components::OpModes::SafeMode && rpiState == Fw::Logic::HIGH) {
        this->rpiGpioSet_out(0, Fw::Logic::LOW);
    }
}

void PDU::comDataIn_handler(const NATIVE_INT_TYPE portNum,
                            Fw::Buffer& recvBuffer,
                            const Drv::RecvStatus& recvStatus) {
    if (recvBuffer.getSize() == 0) {
        this->deallocate_out(0, recvBuffer);
        return;
    }

    PDU_Type type =
        static_cast<PDU_Type>(static_cast<U8>(recvBuffer.getData()[1]) - PDU_CMD_OFFSET);

    switch (type) {
        case PDU_Type::DataPong: {
            this->log_ACTIVITY_HI_PduPong();
            break;
        }
        case PDU_Type::DataSwitchStatus: {
            PDU_SW sw = static_cast<PDU_SW>(recvBuffer.getData()[2] - PDU_CMD_OFFSET);
            U8 state  = recvBuffer.getData()[3] - PDU_CMD_OFFSET;

            if (sw >= PDU_SW::SW_3V3_1 && sw <= PDU_SW::BURN2) {
                sw_telem[static_cast<U8>(sw) - 2].setstate(state);
            }

            this->tlmWrite_SwitchStatus(sw_telem);
            break;
        }
        case PDU_Type::DataSwitchTelem: {
            for (NATIVE_INT_TYPE i = 0; i < sw_telem.SIZE - 1; i++) {
                sw_telem[i].setstate(recvBuffer.getData()[i + 2] - PDU_CMD_OFFSET);
            }
            this->tlmWrite_SwitchStatus(sw_telem);
            break;
        }
        case PDU_Type::DataTRQTelem: {
            trq_telem[0].setmode((recvBuffer.getData()[2] - PDU_CMD_OFFSET) == 1 ? "OPERATIONAL"
                                                                                 : "FAULT");
            trq_telem[1].setmode((recvBuffer.getData()[3] - PDU_CMD_OFFSET) == 1 ? "OPERATIONAL"
                                                                                 : "FAULT");
            for (NATIVE_INT_TYPE i = 2; i < trq_telem.SIZE; i++) {
                trq_telem[i].setmode(trqMode_lookup[recvBuffer.getData()[i + 2] - PDU_CMD_OFFSET]);
            }
            this->tlmWrite_TRQStatus(trq_telem);
            break;
        }
        default:
            break;
    }

    this->deallocate_out(0, recvBuffer);
}

void PDU ::wdt_handler(const NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    pdu_nop_packet packet;
    packet.type = PDU_Type::CommandPing;

    memcpy(pdu_packet_cmd, &packet, sizeof(packet));
    send(pdu_packet_cmd, sizeof(packet));
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void PDU::SetSwitch_cmdHandler(const FwOpcodeType opCode,
                               const U32 cmdSeq,
                               Components::PDU_SW sw,
                               Fw::On state) {
    Components::OpModes opMode;
    this->getOpMode_out(0, opMode);
    if (opMode == Components::OpModes::PowerEmergency && state == Fw::On::ON) {
        this->log_WARNING_HI_SwitchDenied(opMode);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
        return;
    }
    pdu_sw_packet packet;
    packet.type     = PDU_Type::CommandSetSwitch;
    packet.sw       = (Components::PDU::PDU_SW)(U8)sw;
    packet.sw_state = (state == Fw::On::ON) ? 1 : 0;

    if (packet.sw == Components::PDU::PDU_SW::RPI) {
        if (opMode == Components::OpModes::SafeMode) {
            this->log_WARNING_HI_SwitchDenied(opMode);
            this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
            return;
        }
        this->rpiGpioSet_out(0, (packet.sw_state == 1) ? Fw::Logic::HIGH : Fw::Logic::LOW);
        Fw::Logic state;
        this->rpiGpioRead_out(0, state);
        sw_telem[10].setstate(state);
        this->tlmWrite_SwitchStatus(sw_telem);
    } else {
        memcpy(pdu_packet_cmd, &packet, sizeof(packet));
        send(pdu_packet_cmd, sizeof(packet));
    }

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void PDU::GetSwitch_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq) {
    pdu_sw_packet packet;
    packet.type = PDU_Type::CommandGetSwitchStatus;
    packet.sw   = PDU_SW::All;

    memcpy(pdu_packet_cmd, &packet, sizeof(packet));
    send(pdu_packet_cmd, sizeof(packet));
    Fw::Logic state;
    this->rpiGpioRead_out(0, state);
    sw_telem[10].setstate((state == Fw::Logic::HIGH) ? 1 : 0);
    this->tlmWrite_SwitchStatus(sw_telem);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void PDU::Ping_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq) {
    pdu_nop_packet packet;
    packet.type = PDU_Type::CommandPing;

    memcpy(pdu_packet_cmd, &packet, sizeof(packet));
    send(pdu_packet_cmd, sizeof(packet));
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void PDU::SetTRQ_cmdHandler(const FwOpcodeType opCode,
                            const U32 cmdSeq,
                            Components::TRQ_SELECT trq,
                            Components::TRQ_CONFIG mode) {
    pdu_hbridge_packet packet;
    packet.type   = PDU_Type::CommandSetTRQ;
    packet.select = (PDU::TRQ_SELECT)(U8)trq;
    packet.config = (PDU::TRQ_CONFIG)(U8)mode;

    memcpy(pdu_packet_cmd, &packet, sizeof(packet));
    send(pdu_packet_cmd, sizeof(packet));
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void PDU::GetTRQ_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq) {
    pdu_nop_packet packet;
    packet.type = PDU_Type::CommandGetTRQTelem;

    memcpy(pdu_packet_cmd, &packet, sizeof(packet));
    send(pdu_packet_cmd, sizeof(packet));
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void PDU::SetSwitchInternal_handler(const NATIVE_INT_TYPE portNum,
                                    const Components::PDU_SW& sw,
                                    const Fw::On& state) {
    Components::OpModes opMode;
    this->getOpMode_out(0, opMode);
    if (opMode == Components::OpModes::PowerEmergency && state == Fw::On::ON) {
        return;
    }
    pdu_sw_packet packet;
    packet.type     = PDU_Type::CommandSetSwitch;
    packet.sw       = (Components::PDU::PDU_SW)(U8)sw;
    packet.sw_state = (state == Fw::On::ON) ? 1 : 0;

    if (packet.sw == Components::PDU::PDU_SW::RPI && opMode != Components::OpModes::SafeMode) {
        this->rpiGpioSet_out(0, (packet.sw_state == 1) ? Fw::Logic::HIGH : Fw::Logic::LOW);
        Fw::Logic state;
        this->rpiGpioRead_out(0, state);
        sw_telem[10].setstate((state == Fw::Logic::HIGH) ? 1 : 0);
        this->tlmWrite_SwitchStatus(sw_telem);
    } else {
        memcpy(pdu_packet_cmd, &packet, sizeof(packet));
        send(pdu_packet_cmd, sizeof(packet));
    }
}

void PDU::GetSwitchInternal_handler(const NATIVE_INT_TYPE portNum, Components::PDUTlm& states) {
    pdu_sw_packet packet;
    packet.type = PDU_Type::CommandGetSwitchStatus;
    packet.sw   = PDU_SW::All;

    memcpy(pdu_packet_cmd, &packet, sizeof(packet));
    send(pdu_packet_cmd, sizeof(packet));
    Fw::Logic state;
    this->rpiGpioRead_out(0, state);
    sw_telem[10].setstate(state);
    states = sw_telem;
}
}  // end namespace Components
