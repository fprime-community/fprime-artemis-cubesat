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
        CommandSetTRQ,
        CommandGetTRQTelem,
        DataPong,
        DataSwitchStatus,
        DataSwitchTelem,
        DataTRQTelem,
    };

    enum class PDU_SW : U8 {
        None,
        All,
        SW_3V3_1,
        RFM23_RADIO,
        SW_5V_1,
        HEATER,
        SW_5V_3,
        SW_12V,
        VBATT,
        BURN1,
        BURN2,
        RPI
    };

    enum class TRQ_SELECT : U8 {
        TRQ1,
        TRQ2,
        TRQ1A,
        TRQ1B,
        TRQ2A,
        TRQ2B,
    };

    enum class TRQ_CONFIG : U8 {
        SLEEP,
        MOTOR_COAST_FAST_DECAY,
        DIR_REVERSE,
        DIR_FORWARD,
        MOTOR_BREAK_SLOW_DECAY,
    };

    struct __attribute__((packed)) pdu_nop_packet {
        PDU_Type type;
    };

    struct __attribute__((packed)) pdu_sw_packet {
        PDU_Type type;
        PDU_SW sw;
        U8 sw_state;
    };

    struct __attribute__((packed)) pdu_sw_telem {
        PDU_Type type;
        U8 sw_state[10];
    };

    struct __attribute__((packed)) pdu_hbridge_packet {
        PDU_Type type;
        TRQ_SELECT select;
        TRQ_CONFIG config;
    };

    struct __attribute__((packed)) pdu_hbridge_telem {
        PDU_Type type;
        U8 hbridge_state[2];
        TRQ_CONFIG trq_state[4];
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
    //! Handler implementation for run
    //!
    //! Port: receiving calls from the rate group
    void run_handler(NATIVE_INT_TYPE portNum,  //!< The port number
                     NATIVE_UINT_TYPE context  //!< The call order
    );

    void send(U8* buf, NATIVE_INT_TYPE len);

    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for comDataIn
    //!
    void comDataIn_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                           Fw::Buffer& recvBuffer,
                           const Drv::RecvStatus& recvStatus);

    //! Handler implementation for wdt
    //!
    void wdt_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                     NATIVE_UINT_TYPE context       /*!<
                       The call order
                       */
    );

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

    //! Implementation for SetTRQ command handler
    //! Command to set TRQ mode
    void SetTRQ_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                           const U32 cmdSeq,          /*!< The command sequence number*/
                           Components::TRQ_SELECT trq,
                           Components::TRQ_CONFIG mode);

    //! Implementation for GetTRQ command handler
    //! Command to get statuses of torque coils
    void GetTRQ_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                           const U32 cmdSeq           /*!< The command sequence number*/
    );

    bool started;
    bool burnWireOn;
    bool isPowerEmergency;

    U8 pdu_packet_cmd[4];
    Components::PDUTlm sw_telem;
    Components::trqTlm trq_telem;

    Fw::String sw_lookup[11]     = {"3V_1", "RFM23", "5V_1",  "HEATER", "5V_3", "12V_REG",
                                    "12V",  "VBATT", "BURN1", "BURN2",  "RPI"};
    Fw::String trqName_lookup[6] = {"HBRIDGE1", "HBRIDGE2", "1A", "1B", "2A", "2B"};
    Fw::String trqMode_lookup[5] = {"SLEEP", "COAST", "REVERSE", "FORWARD", "BRAKE"};
};
}  // end namespace Components

#endif
