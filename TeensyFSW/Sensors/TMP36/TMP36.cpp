// ======================================================================
// \title  TMP36.cpp
// \author root
// \brief  cpp file for TMP36 component implementation class
// ======================================================================

#include <FpConfig.hpp>
#include <TeensyFSW/Sensors/TMP36/TMP36.hpp>

namespace Sensors {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

TMP36::TMP36(const char* const compName) : TMP36ComponentBase(compName) {}

TMP36::~TMP36() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void TMP36::run_handler(const NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    U16 adcValue;
    this->readAnalog_out(0, adcValue);
    F32 voltage = adcValue * this->MV_PER_ADC_UNIT;

    // Convert voltage to temperature in Fahrenheit and then subtract the offset
    F32 temperatureF = (voltage - this->OFFSET_F) / this->MV_PER_DEGREE_F;

    // Convert temperature to Celsius
    F32 temperatureC = (temperatureF - 32) * 5 / 9;

    if (this->isConnected_temperature_OutputPort(0)) {
        this->temperature_out(0, temperatureC);
    }

    this->tlmWrite_TemperatureSensorData(temperatureC);
}

}  // end namespace Sensors
