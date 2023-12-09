// ======================================================================
// \title  PDU.cpp
// \author lukeclements
// \brief  cpp file for PDU component implementation class
// ======================================================================

#include <FpConfig.hpp>
#include <Fw/Time/Time.hpp>
#include <TeensyFSW/PDU/PDU.hpp>

namespace Components {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

PDU::PDU(const char* const compName) : PDUComponentBase(compName) {
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

void PDU::comDataIn_handler(const NATIVE_INT_TYPE portNum,
                            Fw::Buffer& recvBuffer,
                            const Drv::RecvStatus& recvStatus) {
    if (recvBuffer.getSize() == 0) {
        this->deallocate_out(0, recvBuffer);
        return;
    }

    PDU_Type type =
        static_cast<PDU_Type>(static_cast<U8>(recvBuffer.getData()[1]) - PDU_CMD_OFFSET);

    if (type == PDU_Type::DataSwitchTelem) {
        for (NATIVE_INT_TYPE i = 0; i < sw_telem.SIZE - 1; i++) {
            sw_telem[i].setstate(recvBuffer.getData()[i + 2] - PDU_CMD_OFFSET);
        }
        this->tlmWrite_SwitchStatus(sw_telem);
    } else if (type == PDU_Type::DataTRQTelem) {
        for (NATIVE_INT_TYPE i = 0; i < trq_telem.SIZE; i++) {
            trq_telem[i].setmode(trqMode_lookup[recvBuffer.getData()[i + 2] - PDU_CMD_OFFSET]);
        }
        this->tlmWrite_TRQStatus(trq_telem);
    } else {
        if (type == PDU_Type::DataPong) {
            this->log_ACTIVITY_HI_PduPong();
        } else if (type == PDU_Type::DataSwitchStatus) {
            PDU_SW sw = static_cast<PDU_SW>(recvBuffer.getData()[2] - PDU_CMD_OFFSET);
            U8 state  = recvBuffer.getData()[3] - PDU_CMD_OFFSET;

            if (sw >= PDU_SW::SW_3V3_1 && sw <= PDU_SW::BURN2) {
                sw_telem[static_cast<U8>(sw) - 2].setstate(state);
            }

            this->tlmWrite_SwitchStatus(sw_telem);
        }
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
    pdu_sw_packet packet;
    packet.type     = PDU_Type::CommandSetSwitch;
    packet.sw       = (Components::PDU::PDU_SW)(U8)sw;
    packet.sw_state = (state == Fw::On::ON) ? 1 : 0;

    if (packet.sw == Components::PDU::PDU_SW::RPI) {
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

void PDU ::SetTRQ_cmdHandler(const FwOpcodeType opCode,
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

void PDU ::GetTRQ_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq) {
    pdu_nop_packet packet;
    packet.type = PDU_Type::CommandGetTRQTelem;

    memcpy(pdu_packet_cmd, &packet, sizeof(packet));
    send(pdu_packet_cmd, sizeof(packet));
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void PDU ::SetSwitchInternal_handler(const NATIVE_INT_TYPE portNum,
                                     const Components::PDU_SW& sw,
                                     const Fw::On& state) {
    pdu_sw_packet packet;
    packet.type     = PDU_Type::CommandSetSwitch;
    packet.sw       = (Components::PDU::PDU_SW)(U8)sw;
    packet.sw_state = (state == Fw::On::ON) ? 1 : 0;

    if (packet.sw == Components::PDU::PDU_SW::RPI) {
        this->rpiGpioSet_out(0, (packet.sw_state == 1) ? Fw::Logic::HIGH : Fw::Logic::LOW);
        Fw::Logic state;
        this->rpiGpioRead_out(0, state);
        sw_telem[12].setstate((state == Fw::Logic::HIGH) ? 1 : 0);
        this->tlmWrite_SwitchStatus(sw_telem);
    } else {
        memcpy(pdu_packet_cmd, &packet, sizeof(packet));
        send(pdu_packet_cmd, sizeof(packet));
    }
}

void PDU ::GetSwitchInternal_handler(const NATIVE_INT_TYPE portNum, Components::PDUTlm& states) {
    pdu_sw_packet packet;
    packet.type = PDU_Type::CommandGetSwitchStatus;
    packet.sw   = PDU_SW::All;

    memcpy(pdu_packet_cmd, &packet, sizeof(packet));
    send(pdu_packet_cmd, sizeof(packet));
    Fw::Logic state;
    this->rpiGpioRead_out(0, state);
    sw_telem[12].setstate(state);
    states = sw_telem;
}
}  // end namespace Components
