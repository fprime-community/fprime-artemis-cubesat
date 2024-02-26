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

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for BatteryTemp
    //!
    void BatteryTemp_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                             F32 val);
    //! Handler implementation for run
    //!
    void run_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                     NATIVE_UINT_TYPE context       /*!<
                       The call order
                       */
    );

  private:
    // ----------------------------------------------------------------------
    // Command handler implementations
    // ----------------------------------------------------------------------

    //! Implementation for SetHeater command handler
    //! Command to turn on/off Heater
    void SetHeater_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                              const U32 cmdSeq,          /*!< The command sequence number*/
                              Fw::On state);

    //! Implementation for ToggleAuto command handler
    //! Command to turn off heater automation
    void ToggleAuto_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                               const U32 cmdSeq,          /*!< The command sequence number*/
                               Fw::On state);

    U8 batteryTemp;

    bool enableDisableAuto;

    F32 thresholdTemperature = 0;
};

}  // end namespace Components

#endif
