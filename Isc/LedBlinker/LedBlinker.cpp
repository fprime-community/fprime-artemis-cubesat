// ======================================================================
// \title  LedBlinker.cpp
// \author Sterling Peet <sterling.peet@ae.gatech.edu>
// \brief  Example component to support Arduino Blink deployment.
// ======================================================================


#include <Isc/LedBlinker/LedBlinker.hpp>
#include "Fw/Types/BasicTypes.hpp"

namespace ArduinoBlink {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  LedBlinker :: LedBlinker(
        const char *const compName
    ) :
      LedBlinkerComponentBase(compName),
    m_state(false),
    m_blinks(0)
  {}

  LedBlinker ::
    ~LedBlinker(void)
  {

  }
  
  void LedBlinker ::
    init(
        const NATIVE_INT_TYPE instance
    )
  {
    LedBlinkerComponentBase::init(instance);
  }

  void LedBlinker ::
    blink()
  {
      this->gpio_out(0, (m_state) ? Fw::Logic::HIGH : Fw::Logic::LOW);
      m_state = !m_state;
      m_blinks += 1;
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void LedBlinker ::
    run_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
      blink();
      tlmWrite_numBlinks(m_blinks);
  }

} // end namespace ArduinoBlink

