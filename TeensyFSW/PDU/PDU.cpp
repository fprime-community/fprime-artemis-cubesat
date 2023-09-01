// ======================================================================
// \title  PDU.cpp
// \author lukeclements
// \brief  cpp file for PDU component implementation class
// ======================================================================

#include <TeensyFSW/PDU/PDU.hpp>
#include <FpConfig.hpp>
#include <Fw/Time/Time.hpp>

namespace Components
{

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  PDU ::
      PDU(
          const char *const compName) : PDUComponentBase(compName)
  {
    for (NATIVE_INT_TYPE i = 0; i < telem.SIZE; i++)
    {
      telem[i] = 0;
    }
  }

  PDU ::
      ~PDU()
  {
  }

  void PDU::send(pdu_packet packet)
  {
    memcpy(pdu_packet_cmd, &packet, sizeof(packet));

    for (size_t i = 0; i < sizeof(packet); i++)
    {
      pdu_packet_cmd[i] = pdu_packet_cmd[i] + PDU_CMD_OFFSET;
    }
    pdu_packet_cmd[sizeof(packet)] = '\n';

    Fw::Buffer sendBuffer = this->allocate_out(0, sizeof(pdu_packet_cmd));
    memcpy(sendBuffer.getData(), &pdu_packet_cmd[0], sizeof(pdu_packet_cmd));
    sendBuffer.setSize(sizeof(pdu_packet_cmd));
    this->comDataOut_out(0, sendBuffer);
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void PDU ::
      comDataIn_handler(
          const NATIVE_INT_TYPE portNum,
          Fw::Buffer &recvBuffer,
          const Drv::RecvStatus &recvStatus)
  {
    if (recvBuffer.getSize() == 0)
    {
      this->deallocate_out(0, recvBuffer);
      return;
    }

    PDU_Type type = static_cast<PDU_Type>(static_cast<U8>(recvBuffer.getData()[1]) - PDU_CMD_OFFSET);

    if (type == PDU_Type::DataSwitchTelem)
    {
      for (NATIVE_INT_TYPE i = 0; i < telem.SIZE - 1; i++)
      {
        telem[i] = recvBuffer.getData()[i + 2] - PDU_CMD_OFFSET;
      }
      this->tlmWrite_SwitchStatus(telem);
    }
    else
    {
      if (type == PDU_Type::DataPong)
      {
        this->log_ACTIVITY_HI_PduPong();
      }
      else if (type == PDU_Type::DataSwitchStatus)
      {
        PDU_SW sw = static_cast<PDU_SW>(recvBuffer.getData()[2] - PDU_CMD_OFFSET);
        U8 state = recvBuffer.getData()[3] - PDU_CMD_OFFSET;

        if (sw >= PDU_SW::SW_3V3_1 && sw <= PDU_SW::VBATT)
        {
          telem[static_cast<U8>(sw) - 2] = state;
        }
        else if (sw == PDU_SW::BURN1)
        {
          telem[8] = state;
        }
        else if (sw == PDU_SW::BURN2)
        {
          telem[9] = state;
        }
        else if (sw == PDU_SW::HBRIDGE1)
        {
          telem[10] = state;
        }
        else if (sw == PDU_SW::HBRIDGE2)
        {
          telem[11] = state;
        }

        this->tlmWrite_SwitchStatus(telem);
      }
    }

    this->deallocate_out(0, recvBuffer);
  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void PDU ::
      SetSwitch_cmdHandler(
          const FwOpcodeType opCode,
          const U32 cmdSeq,
          Components::PDU_SW sw,
          Fw::On state)
  {
    pdu_packet packet;
    packet.type = PDU_Type::CommandSetSwitch;
    packet.sw = (Components::PDU::PDU_SW)(U8)sw;
    packet.sw_state = (state == Fw::On::ON) ? 1 : 0;

    if (packet.sw == Components::PDU::PDU_SW::RPI)
    {
      this->rpiGpioSet_out(0, (packet.sw_state == 1) ? Fw::Logic::HIGH : Fw::Logic::LOW);
      Fw::Logic state;
      this->rpiGpioRead_out(0, state);
      telem[12] = state;
      this->tlmWrite_SwitchStatus(telem);
    }
    else
    {
      send(packet);
    }

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void PDU ::
      GetSwitch_cmdHandler(
          const FwOpcodeType opCode,
          const U32 cmdSeq)
  {
    pdu_packet packet;
    packet.type = PDU_Type::CommandGetSwitchStatus;
    packet.sw = PDU_SW::All;

    send(packet);
    Fw::Logic state;
    this->rpiGpioRead_out(0, state);
    telem[12] = state;
    this->tlmWrite_SwitchStatus(telem);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void PDU ::
      Ping_cmdHandler(
          const FwOpcodeType opCode,
          const U32 cmdSeq)
  {
    pdu_packet packet;
    packet.type = PDU_Type::CommandPing;

    send(packet);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }
} // end namespace Components
