// ======================================================================
// \title  INA219.cpp
// \author root
// \brief  cpp file for INA219 component implementation class
// ======================================================================

#include <FpConfig.hpp>
#include <TeensyFSW/Sensors/INA219/INA219.hpp>

namespace Sensors {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

INA219::INA219(const char* const compName) : INA219ComponentBase(compName) {}

INA219::~INA219() {}

void INA219::config(U8 addr, TwoWire* wire) {
    currentSensor = new Adafruit_INA219(addr);
    currentSensor->begin(wire);
    currentSensor->setCalibration_16V_400mA();
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void INA219::run_handler(const NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    Sensors::CurrentTlm data;
    data[0] = currentSensor->getBusVoltage_V();
    data[1] = currentSensor->getCurrent_mA();
    data[2] = currentSensor->getPower_mW();
    this->tlmWrite_CurrentSensorData(data);
}

void INA219::getPowerData_handler(NATIVE_INT_TYPE portNum, F32& voltage, F32& current) {
    voltage = currentSensor->getBusVoltage_V();
    current = currentSensor->getCurrent_mA();
}

}  // end namespace Sensors
