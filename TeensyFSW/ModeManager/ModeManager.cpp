// ======================================================================
// \title  ModeManager.cpp
// \author root
// \brief  cpp file for ModeManager component implementation class
// ======================================================================

#include "TeensyFSW/ModeManager/ModeManager.hpp"
#include "FpConfig.hpp"

namespace Components {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

ModeManager ::ModeManager(const char* const compName)
    : ModeManagerComponentBase(compName), currentMode(OpModes::Startup), started(false), startupTimeout(0), deploymentTimeout(0) {}

ModeManager ::~ModeManager() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void ModeManager ::getOpMode_handler(NATIVE_INT_TYPE portNum, Components::OpModes& mode) {
    mode = this->currentMode;
}

void ModeManager ::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    F32 vbattVoltage;
    F32 vbattCurrent;

    // Read battery power only if startup is complete
    if (this->started) {
        this->getBatteryPower_out(0, vbattVoltage, vbattCurrent);
    }
    
    switch (this->currentMode) {
        case OpModes::Startup:
        {
            // TODO: Check SD Card to see if deployment was already made. If yes, go to Nominal mode. If not, go to deployment mode.
            bool deploymentCompleted = false;

            if (deploymentCompleted) {
                // Give 5 seconds to let everything start up properly
                if (this->startupTimeout > 5000) {
                    this->currentMode = OpModes::Nominal;
                    this->started = true;
                }
            } else {
                // Wait 30 minutes before deploying antennas.
                if (this->startupTimeout > 30 * 60 * 1000) {
                    this->deploymentTimeout = 0;
                    this->currentMode = OpModes::Deployment;
                    this->started = true;
                }
            }
            break;
        }
        case OpModes::Deployment:
        {
            // TODO: delay for 30 seconds? to allow burn wires to complete.
            if (this->deploymentTimeout > 30000) {
                this->currentMode = OpModes::Nominal;
            }
            break;
        }
        case OpModes::PowerEmergency:
            // If battery power is back to reasonable power, resume to the previous operation mode.
            this->currentMode = this->prevOpMode;
            break;
        default:
            break;
    }

    // Check power of battery. If below threshold, switch to PowerEmergency mode.
    if (this->currentMode != OpModes::Startup && vbattVoltage < 6) {
        this->prevOpMode = this->currentMode;
        this->currentMode = OpModes::PowerEmergency;
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
