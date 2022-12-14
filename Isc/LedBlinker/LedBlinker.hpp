// ======================================================================
// \title  LedBlinker.hpp
// \author Sterling Peet <sterling.peet@ae.gatech.edu>
// \brief  Example component to support Arduino Blink deployment.
// ======================================================================

#ifndef Isc_LedBlinker_HPP
#define Isc_LedBlinker_HPP

#include "Isc/LedBlinker/LedBlinkerComponentAc.hpp"

namespace ArduinoBlink {

  class LedBlinker :
    public LedBlinkerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object LedBlinker
      //!
      LedBlinker(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object LedBlinker
      //!
      void init(
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object LedBlinker
      //!
      ~LedBlinker(void);

    PRIVATE:
      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Blink a given pin
      void blink();
      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for schedIn
      //!
      void run_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< The call order*/
      );

      bool m_state;
      U32 m_blinks;
    };

} // end namespace ArduinoBlink

#endif // end LedBlinker_HPP
