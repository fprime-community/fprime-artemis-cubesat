// ======================================================================
// \title  Heater.hpp
// \author root
// \brief  hpp file for Heater component implementation class
// ======================================================================

#ifndef Heater_HPP
#define Heater_HPP

#include "TeensyFSW/Heater/HeaterComponentAc.hpp"

namespace Components {

class Heater : public HeaterComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object Heater
    //!
    Heater(const char* const compName /*!< The component name*/
    );

    //! Destroy object Heater
    //!
    ~Heater();

    PRIVATE :

        // ----------------------------------------------------------------------
        // Handler implementations for user-defined typed input ports
        // ----------------------------------------------------------------------

        //! Handler implementation for comDataIn
        //!
        void
        comDataIn_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                          Fw::Buffer& recvBuffer,
                          const Drv::RecvStatus& recvStatus);

    PRIVATE :

        // ----------------------------------------------------------------------
        // Command handler implementations
        // ----------------------------------------------------------------------

        //! Implementation for SetHeater command handler
        //! Command to turn on/off Heater
        void
        SetHeater_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                             const U32 cmdSeq,          /*!< The command sequence number*/
                             Components::PDU_SW sw,
                             Fw::On state);

    //! Implementation for GetHeater command handler
    //! Command to get statuses of Heater switch
    void GetHeater_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                              const U32 cmdSeq           /*!< The command sequence number*/
    );

    //! Implementation for AutoControlHeater command handler
    //! Command to automatically control the Heater based on temperature
    void AutoControlHeater_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                                      const U32 cmdSeq,          /*!< The command sequence number*/
                                      F32 temperature);
};

}  // end namespace Components

#endif
