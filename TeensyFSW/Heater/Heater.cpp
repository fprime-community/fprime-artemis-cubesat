// ======================================================================
// \title  Heater.cpp
// \author root
// \brief  cpp file for Heater component implementation class
// ======================================================================

#include <FpConfig.hpp>
#include <TeensyFSW/Heater/Heater.hpp>
#include <TeensyFSW/PDU/PDU.hpp>

namespace Components {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Heater ::Heater(const char* const compName) : HeaterComponentBase(compName), batteryTemp(0) {}

Heater ::~Heater() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Heater ::BatteryTemp_handler(const NATIVE_INT_TYPE portNum, F32 val) {
    this->batteryTemp = val;
}

void Heater::run_handler(const NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    Components::OpModes opMode;
    this->getOpMode_out(0, opMode);
    if (opMode == Components::OpModes::Startup || opMode == Components::OpModes::PowerEmergency) {
        return;
    }

    if (!this->enableDisableAuto) {
        return;
    }

    if (this->batteryTemp < this->thresholdTemperature) {
        this->PDUSetSwitch_out(0, Components::PDU_SW::HEATER, Fw::On::ON);
    } else {
        this->PDUSetSwitch_out(0, Components::PDU_SW::HEATER, Fw::On::OFF);
    }
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void Heater ::SetHeater_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq, Fw::On state) {
    Components::OpModes opMode;
    this->getOpMode_out(0, opMode);
    if (opMode == Components::OpModes::PowerEmergency) {
        this->log_WARNING_HI_HeaterDenied(opMode);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
        return;
    }
    this->PDUSetSwitch_out(0, Components::PDU_SW::HEATER, state);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void Heater ::ToggleAuto_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq, Fw::On state) {
    this->enableDisableAuto = (state == Fw::On::ON) ? true : false;
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // end namespace Components
