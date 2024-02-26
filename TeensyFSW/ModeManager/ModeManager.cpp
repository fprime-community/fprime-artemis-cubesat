// ======================================================================
// \title  ModeManager.cpp
// \author root
// \brief  cpp file for ModeManager component implementation class
// ======================================================================

#include "TeensyFSW/ModeManager/ModeManager.hpp"
#include <Os/File.hpp>
#include "FpConfig.hpp"

namespace Components {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

ModeManager ::ModeManager(const char* const compName)
    : ModeManagerComponentBase(compName),
      currentMode(OpModes::Startup),
      started(false),
      timeout(0) {}

ModeManager ::~ModeManager() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void ModeManager ::getOpMode_handler(NATIVE_INT_TYPE portNum, Components::OpModes& mode) {
    mode = this->currentMode;
}

void ModeManager ::tlmSend_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    this->tlmWrite_CurrentOpMode(this->currentMode);
}

void ModeManager ::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    F32 vbattVoltage;
    F32 vbattCurrent;

    // Read battery power only if startup is complete
    this->getBatteryPower_out(0, vbattVoltage, vbattCurrent);

    switch (this->currentMode) {
        case OpModes::Startup: {
            // Check if deployment was already made by looking for the deployment file
            Os::File deploymentFile;
            Os::File::Status fileStatus =
                deploymentFile.open("deployment.txt", Os::File::OPEN_READ);

            bool deploymentCompleted = (fileStatus == Os::File::OP_OK);

            if (deploymentCompleted) {
                this->timeout     = 0;
                this->currentMode = OpModes::Initialization;
            } else {
                // Wait x amount of time before deploying antennas.
                if (this->timeout > 10000) {
                    this->timeout     = 0;
                    this->currentMode = OpModes::Deployment;
                }
            }
            break;
        }
        case OpModes::Deployment: {
            // delay for 30 seconds to allow burn wires to complete.
            if (this->timeout > 30000) {
                // Create and write to deployment.txt
                Os::File deploymentFile;
                Os::File::Status fileStatus =
                    deploymentFile.open("deployment.txt", Os::File::OPEN_WRITE);

                if (fileStatus == Os::File::OP_OK) {
                    const char dataToWrite = '1';
                    NATIVE_INT_TYPE size   = sizeof(dataToWrite);
                    deploymentFile.write(&dataToWrite, size);
                    deploymentFile.close();
                }
                this->timeout     = 0;
                this->currentMode = OpModes::Initialization;
            }
            break;
        }
        case OpModes::Initialization: {
            // Give 5 seconds to let everything start up properly
            if (this->timeout > 5000) {
                this->currentMode = OpModes::Nominal;
                this->started     = true;
            }
            break;
        }
        case OpModes::Nominal: {
            break;
        }
        case OpModes::DataTransmit: {
            break;
        }
        case OpModes::PowerEmergency:
            // If battery power is back to reasonable power, resume to the previous operation mode.
            if (vbattVoltage >= 6.5) {
                this->currentMode = this->prevOpMode;
            }
            break;
        default:
            break;
    }

    // Check power of battery. If below threshold, switch to PowerEmergency mode.
    if (this->currentMode != OpModes::Startup && this->currentMode != OpModes::PowerEmergency &&
        vbattVoltage < 6.5) {
        this->prevOpMode  = this->currentMode;
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
