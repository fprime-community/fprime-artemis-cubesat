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
  }

  PDU ::
      ~PDU()
  {
  }

  void PDU::send(pdu_packet packet)
  {
    char *cmd = (char *)malloc(sizeof(packet));
    memcpy(cmd, &packet, sizeof(packet));

    U8 msg[sizeof(packet) + 1];
    for (size_t i = 0; i < sizeof(packet); i++)
    {
      msg[i] += cmd[i] + PDU_CMD_OFFSET;
    }
    msg[sizeof(packet)] = '\n';

    Fw::Buffer sendBuffer = this->allocate_out(0, sizeof(msg));
    sendBuffer.setData(msg);
    sendBuffer.setSize(sizeof(msg));
    this->comDataOut_out(0, sendBuffer);
  }

  I32 PDU::recv(std::string &response) {
    return 0;
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
    if (recvBuffer.getSize() > sizeof(struct pdu_packet))
    {
      // assuming pdu telem
    }
    else
    {
      // its a pdu packet
      // but need to check if data pong or data switch status
      // if data switch status then only update the corresponding telem data index
      Components::PDUTlm telem;
      for (NATIVE_INT_TYPE i = 0; i < telem.SIZE; i++)
      {
        telem[i] = 1;
      }
      this->tlmWrite_SwitchStatus(telem);
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
    std::string response;
    pdu_packet packet;
    packet.type = PDU_Type::CommandSetSwitch;
    packet.sw = sw;
    packet.sw_state = (state == Fw::On::ON) ? 1 : 0;
    U8 attempts = 1;
    Fw::Time timeout;
    while (1)
    {
      send(packet);
      while (recv(response) < 0)
      {
        if (timeout.getSeconds() > 5)
        {
          timeout.set(0, 0);

          if (++attempts == 5)
          {
            this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::EXECUTION_ERROR);
            return;
          }
          break;
        }
      }
      if ((response[1] == static_cast<U8>(sw) + PDU_CMD_OFFSET) || (sw == PDU_SW::All && response[0] == static_cast<U8>(PDU::PDU_Type::DataSwitchTelem) + PDU_CMD_OFFSET))
      {
        break;
      }
    }
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void PDU ::
      GetSwitch_cmdHandler(
          const FwOpcodeType opCode,
          const U32 cmdSeq,
          Components::PDU_SW sw)
  {
    std::string response;
    pdu_packet packet;
    packet.type = PDU_Type::CommandGetSwitchStatus;
    packet.sw = PDU_SW::All;
    uint8_t attempts = 1;
    Fw::Time timeout;
    while (1)
    {
      send(packet);
      while (recv(response) < 0)
      {
        if (timeout.getSeconds() > 5)
        {
          timeout.set(0, 0);

          if (++attempts == 5)
          {
            this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::EXECUTION_ERROR);
            return;
          }
          break;
        }
      }
      if ((response[1] == static_cast<U8>(sw) + PDU_CMD_OFFSET) || (sw == PDU_SW::All && response[0] == static_cast<U8>(PDU::PDU_Type::DataSwitchTelem) + PDU_CMD_OFFSET))
      {
        break;
      }
    }
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }
} // end namespace Components
