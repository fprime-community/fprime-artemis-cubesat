// ======================================================================
// \title  ModeManager.cpp
// \author root
// \brief  cpp file for ModeManager component implementation class
// ======================================================================

#include "TeensyFSW/ModeManager/ModeManager.hpp"
#include <TeensyFSW/PDU/PDU.hpp>
#include "FpConfig.hpp"

namespace Components {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

ModeManager ::ModeManager(const char* const compName)
    : ModeManagerComponentBase(compName), currentMode(OpModes::Startup) {}

ModeManager ::~ModeManager() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void ModeManager ::getOpMode_handler(NATIVE_INT_TYPE portNum, Components::OpModes& mode) {
    mode = this->currentMode;
}

void ModeManager ::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    switch (this->currentMode) {
        case OpModes::Startup:

            this->PDUSetSwitch_out(0, Components::PDU_SW::RFM23_RADIO, Fw::On::ON);
            this->enableHeater_out(0, true);
            this->enableGPS_out(0, true);
            break;
        case OpModes::PowerEmergency:
            this->enableHeater_out(0, false);
            break;
        default:
            break;
    }
}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void ModeManager ::SetMode_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, Components::OpModes mode) {
    this->currentMode = mode;
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // namespace Components
