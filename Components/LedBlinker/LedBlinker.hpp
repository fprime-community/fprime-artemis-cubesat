// ======================================================================
// \title  LedBlinker.hpp
// \author ethan
// \brief  hpp file for LedBlinker component implementation class
// ======================================================================

#ifndef LedBlinker_HPP
#define LedBlinker_HPP

#include "Components/LedBlinker/LedBlinkerComponentAc.hpp"

namespace Components {

class LedBlinker : public LedBlinkerComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object LedBlinker
    //!
    LedBlinker(const char* const compName /*!< The component name*/
    );

    //! Destroy object LedBlinker
    //!
    ~LedBlinker();

    //! Emit parameter updated EVR
    //!
    void parameterUpdated(FwPrmIdType id /*!< The parameter ID*/
    );

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for run
    //!
    void run_handler(const NATIVE_INT_TYPE portNum, /*!< The port number */
                     NATIVE_UINT_TYPE context       /*!< The call order */
    );

    // ----------------------------------------------------------------------
    // Command handler implementations
    // ----------------------------------------------------------------------

    //! Implementation for BLINKING_ON_OFF command handler
    //! Command to turn on or off the blinking LED
    void BLINKING_ON_OFF_cmdHandler(
        const FwOpcodeType opCode, /*!< The opcode*/
        const U32 cmdSeq,          /*!< The command sequence number*/
        Fw::On on_off              /*!< Indicates whether the blinking should be on or off*/
    );

    Fw::On state;     //! Keeps track if LED is on or off
    U32 transitions;  //! The number of on/off transitions that have occurred from FSW boot up
    U16 count;        //! Keeps track of how many ticks the LED has been on for
    bool blinking;    //! Flag: if true then LED blinking will occur else no blinking will happen
};

}  // end namespace Components

#endif
