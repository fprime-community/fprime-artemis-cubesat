// ======================================================================
// \title  PA1010D.hpp
// \author root
// \brief  hpp file for PA1010D component implementation class
// ======================================================================

#ifndef Sensors_PA1010D_HPP
#define Sensors_PA1010D_HPP

#define GPSSerial Serial1

#include "Adafruit_GPS.h"
#include "TeensyFSW/Sensors/PA1010D/PA1010DComponentAc.hpp"

namespace Sensors {

class PA1010D : public PA1010DComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct PA1010D object
    PA1010D(const char* const compName  //!< The component name
    );

    //! Initialize object PA1010D
    //!
    void init(const NATIVE_INT_TYPE instance = 0 /*!< The instance number */
    );

    //! Destroy PA1010D object
    ~PA1010D();

    bool init_gps(void);

  private:
    Adafruit_GPS gps;

    PRIVATE :

        // ----------------------------------------------------------------------
        // Handler implementations for user-defined typed input ports
        // ----------------------------------------------------------------------

        //! Handler implementation for run
        //!
        //! Port: receiving calls from the rate group
        void
        run_handler(NATIVE_INT_TYPE portNum,  //!< The port number
                    NATIVE_UINT_TYPE context  //!< The call order
        );
};

}  // namespace Sensors

#endif
