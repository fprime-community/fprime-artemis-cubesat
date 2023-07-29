// ======================================================================
// \title  PDU.hpp
// \author lukeclements
// \brief  hpp file for PDU component implementation class
// ======================================================================

#ifndef PDU_HPP
#define PDU_HPP

#include "TeensyFSW/PDU/PDUComponentAc.hpp"
#include <string>

namespace Components
{

  class PDU : public PDUComponentBase
  {
    const NATIVE_INT_TYPE PDU_CMD_OFFSET = 48;

    enum class PDU_Type : uint8_t
    {
      NOP,
      CommandPing,
      CommandSetSwitch,
      CommandGetSwitchStatus,
      DataPong,
      DataSwitchStatus,
      DataSwitchTelem,
    };

    struct __attribute__((packed)) pdu_packet
    {
      PDU_Type type = PDU_Type::NOP;
      Components::PDU_SW sw = Components::PDU_SW::None;
      U8 sw_state = 0;
    };

    struct __attribute__((packed)) pdu_telem
    {
      PDU_Type type = PDU_Type::DataSwitchTelem;
      U8 sw_state[12];
    };

  public:
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object PDU
    //!
    PDU(
        const char *const compName /*!< The component name*/
    );

    //! Destroy object PDU
    //!
    ~PDU();

    PRIVATE :

    void send(pdu_packet packet);
    I32 recv(std::string &response);

    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for comDataIn
    //!
    void comDataIn_handler(
        const NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::Buffer &recvBuffer,
        const Drv::RecvStatus &recvStatus);

    PRIVATE :

        // ----------------------------------------------------------------------
        // Command handler implementations
        // ----------------------------------------------------------------------

        //! Implementation for SetSwitch command handler
        //! Command to control on switch
        void
        SetSwitch_cmdHandler(
            const FwOpcodeType opCode, /*!< The opcode*/
            const U32 cmdSeq,          /*!< The command sequence number*/
            Components::PDU_SW sw,
            Fw::On state);

    //! Implementation for GetSwitch command handler
    //!
    void GetSwitch_cmdHandler(
        const FwOpcodeType opCode, /*!< The opcode*/
        const U32 cmdSeq,          /*!< The command sequence number*/
        Components::PDU_SW sw);
  };
} // end namespace Components

#endif
