// ======================================================================
// \title  LSM6DS.hpp
// \author ethan
// \brief  hpp file for LSM6DS component implementation class
// ======================================================================

#ifndef LSM6DS_HPP
#define LSM6DS_HPP

#include "artemis-sensors/LSM6DS/LSM6DSComponentAc.hpp"
#include "Adafruit_LSM6DSOX.h"
#include "Adafruit_Sensor.h"

namespace ArtemisSensors {

  class LSM6DS :
    public LSM6DSComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object LSM6DS
      //!
      LSM6DS(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object LSM6DS
      //!
      void init(
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object LSM6DS
      //!
      ~LSM6DS();

      bool init_imu(void);

    PRIVATE:

      Adafruit_LSM6DSOX imu;

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for SchedIn
      //!
      void SchedIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< 
      The call order
      */
      );


    };

} // end namespace ArtemisSensors

#endif
