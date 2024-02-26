// ======================================================================
// \title  INA219.hpp
// \author root
// \brief  hpp file for INA219 component implementation class
// ======================================================================

#ifndef INA219_HPP
#define INA219_HPP
#include <Adafruit_INA219.h>
#include <Wire.h>

#include "TeensyFSW/Sensors/INA219/INA219ComponentAc.hpp"

namespace Sensors {

class INA219 : public INA219ComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object INA219
    //!
    INA219(const char* const compName /*!< The component name*/
    );

    //! Destroy object INA219
    //!
    ~INA219();

    void config(U8 addr, TwoWire* wire);

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for run
    //!
    void run_handler(const NATIVE_INT_TYPE portNum, /*!< The port number */
                     NATIVE_UINT_TYPE context       /*!< The call order */
    );

    //! Handler implementation for getPowerData
      //!
      //! Port to return power data of current sensor
      void getPowerData_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          F32& voltage,
          F32& current
      );

    Adafruit_INA219* currentSensor;
};

}  // end namespace Sensors

#endif
