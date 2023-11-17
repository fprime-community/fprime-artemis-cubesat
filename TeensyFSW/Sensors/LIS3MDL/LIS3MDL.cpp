// ======================================================================
// \title  LIS3MDL.cpp
// \author lukeclements
// \brief  cpp file for LIS3MDL component implementation class
// ======================================================================

#include <FpConfig.hpp>
#include <TeensyFSW/Sensors/LIS3MDL/LIS3MDL.hpp>

namespace Sensors {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

LIS3MDL::LIS3MDL(const char* const compName) : LIS3MDLComponentBase(compName) {}

void LIS3MDL::init(const NATIVE_INT_TYPE instance) {
    LIS3MDLComponentBase::init(instance);
}

LIS3MDL::~LIS3MDL() {}

bool LIS3MDL::init_mag(void) {
    if (!mag->begin_I2C()) {
        return false;
    }
    mag->setPerformanceMode(LIS3MDL_LOWPOWERMODE);
    mag->setDataRate(LIS3MDL_DATARATE_0_625_HZ);
    mag->setRange(LIS3MDL_RANGE_16_GAUSS);
    mag->setOperationMode(LIS3MDL_CONTINUOUSMODE);

    return true;
}

void LIS3MDL::run_handler(const NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    sensors_event_t mag_event;
    mag->getEvent(&mag_event);
    Sensors::MagTlm data;
    data[0] = mag_event.magnetic.x;
    data[1] = mag_event.magnetic.y;
    data[2] = mag_event.magnetic.z;
    this->tlmWrite_Data(data);
}
}  // end namespace Sensors
