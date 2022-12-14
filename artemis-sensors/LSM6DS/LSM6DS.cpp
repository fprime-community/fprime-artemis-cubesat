// ======================================================================
// \title  LSM6DS.cpp
// \author ethan
// \brief  cpp file for LSM6DS component implementation class
// ======================================================================


#include <artemis-sensors/LSM6DS/LSM6DS.hpp>
#include <FpConfig.hpp>

namespace ArtemisSensors {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  LSM6DS ::
    LSM6DS(
        const char *const compName
    ) : LSM6DSComponentBase(compName)
  {

  }

  void LSM6DS ::
    init(
        const NATIVE_INT_TYPE instance
    )
  {
    LSM6DSComponentBase::init(instance);
  }

  LSM6DS ::
    ~LSM6DS()
  {

  }

  bool init_imu(void) {
    if (!imu.begin_I2C())
    {
      return false;
    }
    imu.setAccelRange(LSM6DS_ACCEL_RANGE_16_G);
    imu.setGyroRange(LSM6DS_GYRO_RANGE_2000_DPS);
    imu.setAccelDataRate(LSM6DS_RATE_6_66K_HZ);
    imu.setGyroDataRate(LSM6DS_RATE_6_66K_HZ);

    return true;
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void LSM6DS ::
    SchedIn_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    imu.getEvent(&accel, &gyro, &temp); 
  }

} // end namespace ArtemisSensors
