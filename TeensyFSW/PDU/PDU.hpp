// ======================================================================
// \title  PDU.hpp
// \author lukeclements
// \brief  hpp file for PDU component implementation class
// ======================================================================

#ifndef PDU_HPP
#define PDU_HPP

#include "TeensyFSW/PDU/PDUComponentAc.hpp"

namespace Components {

class PDU : public PDUComponentBase {
    const NATIVE_INT_TYPE PDU_CMD_OFFSET = 48;

    enum class PDU_Type : U8 {
        NOP,
        CommandPing,
        CommandSetSwitch,
        CommandGetSwitchStatus,
        DataPong,
        DataSwitchStatus,
        DataSwitchTelem,
    };

    enum class PDU_SW : U8 {
        None,
        All,
        SW_3V3_1,
        RADIO_RFM23,
        SW_5V_1,
        HEATER,
        SW_5V_3,
        REGULATOR_12V,
        SW_12V,
        VBATT,
        PDU_WDT,
        HBRIDGE1,
        HBRIDGE2,
        BURN,
        BURN1,
        BURN2,
        RPI
    };

    struct __attribute__((packed)) pdu_packet {
        PDU_Type type = PDU_Type::NOP;
        PDU_SW sw     = PDU_SW::None;
        U8 sw_state   = 0;
    };

    struct __attribute__((packed)) pdu_telem {
        PDU_Type type = PDU_Type::DataSwitchTelem;
        U8 sw_state[12];
    };

  public:
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object PDU
    //!
    PDU(const char* const compName /*!< The component name*/
    );

    //! Destroy object PDU
    //!
    ~PDU();

  private:
    void send(pdu_packet packet);

    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for comDataIn
    //!
    void comDataIn_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                           Fw::Buffer& recvBuffer,
                           const Drv::RecvStatus& recvStatus);

    //! Handler implementation for SetSwitchInternal
    //!
    void SetSwitchInternal_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                                   const Components::PDU_SW& sw,
                                   const Fw::On& state);

    //! Handler implementation for GetSwitchInternal
    //!
    void GetSwitchInternal_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                                   Components::PDUTlm& states);

    // ----------------------------------------------------------------------
    // Command handler implementations
    // ----------------------------------------------------------------------

    //! Implementation for SetSwitch command handler
    //! Command to control on switch
    void SetSwitch_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                              const U32 cmdSeq,          /*!< The command sequence number*/
                              Components::PDU_SW sw,
                              Fw::On state);

    //! Implementation for GetSwitch command handler
    //!
    void GetSwitch_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                              const U32 cmdSeq           /*!< The command sequence number*/
    );

    //! Implementation for Ping command handler
    //!
    void Ping_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                         const U32 cmdSeq           /*!< The command sequence number*/
    );

    U8 pdu_packet_cmd[4];
    Components::PDUTlm telem;

    Fw::String lookup[13] = {"3V_1",  "3V_2",  "5V_1",  "5V_2",     "5V_3",     "5V_4", "12V",
                             "VBATT", "BURN1", "BURN2", "HBRIDGE1", "HBRIDGE2", "RPI"};
};
}  // end namespace Components

#endif
