// ======================================================================
// \title  TMP36.hpp
// \author root
// \brief  hpp file for TMP36 component implementation class
// ======================================================================

#ifndef TMP36_HPP
#define TMP36_HPP

#include "TeensyFSW/Sensors/TMP36/TMP36ComponentAc.hpp"

namespace Sensors {

class TMP36 : public TMP36ComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object TMP36
    //!
    TMP36(const char* const compName /*!< The component name*/
    );

    //! Destroy object TMP36
    //!
    ~TMP36();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for run
    //!
    void run_handler(const NATIVE_INT_TYPE portNum, /*!< The port number */
                     NATIVE_UINT_TYPE context       /*!< The call order */
    );

    const F32 MV_PER_DEGREE_F = 1.0;              // 1 mV/°F
    const F32 OFFSET_F = 58.0;                    // 58 mV (58°F) offset in the output voltage
    const F32 MV_PER_ADC_UNIT = 3300.0 / 1024.0;  // Assuming 3.3V reference voltage and 10-bit ADC resolution
};

}  // end namespace Sensors

#endif
