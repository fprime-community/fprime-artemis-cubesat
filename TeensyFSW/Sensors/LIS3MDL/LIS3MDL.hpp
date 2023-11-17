// ======================================================================
// \title  LIS3MDL.hpp
// \author lukeclements
// \brief  hpp file for LIS3MDL component implementation class
// ======================================================================

#ifndef LIS3MDL_HPP
#define LIS3MDL_HPP

#include "Adafruit_LIS3MDL.h"
#include "Adafruit_Sensor.h"
#include "TeensyFSW/Sensors/LIS3MDL/LIS3MDLComponentAc.hpp"

namespace Sensors {

class LIS3MDL : public LIS3MDLComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object LIS3MDL
    //!
    LIS3MDL(const char* const compName /*!< The component name*/
    );

    //! Initialize object LSM6DS
    //!
    void init(const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
    );

    //! Destroy object LIS3MDL
    //!
    ~LIS3MDL();

    bool init_mag(void);

  private:
    //! Handler implementation for SchedIn
    //!
    void run_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                     NATIVE_UINT_TYPE context       /*!< The call order */
    );

    Adafruit_LIS3MDL* mag = new Adafruit_LIS3MDL();
};

}  // end namespace Sensors

#endif
