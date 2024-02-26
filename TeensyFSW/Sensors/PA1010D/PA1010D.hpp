// ======================================================================
// \title  PA1010D.hpp
// \author root
// \brief  hpp file for PA1010D component implementation class
// ======================================================================

#ifndef Sensors_PA1010D_HPP
#define Sensors_PA1010D_HPP

#define GPSSerial Serial7

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

    //! Destroy PA1010D object
    ~PA1010D();

    bool init_gps(void);

  private:
    Adafruit_GPS gps;

    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for run
    //!
    //! Port: receiving calls from the rate group
    void run_handler(NATIVE_INT_TYPE portNum,  //!< The port number
                     NATIVE_UINT_TYPE context  //!< The call order
    );
    // ----------------------------------------------------------------------
    // Command handler implementations
    // -
    //! Implementation for SetHeater command handler

    //! Command to set GPS mode
    void SetGPSMode_cmdHandler(const FwOpcodeType opCode, /*!< The opcode*/
                               const U32 cmdSeq,          /*!< The command sequence number*/
                               Fw::On state);

    Sensors::GPSTlmData data;

    bool enabled;
};

}  // namespace Sensors

#endif