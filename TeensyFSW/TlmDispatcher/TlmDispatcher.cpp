// ======================================================================
// \title  TlmDispatcher.cpp
// \author root
// \brief  cpp file for TlmDispatcher component implementation class
// ======================================================================

#include "TeensyFSW/TlmDispatcher/TlmDispatcher.hpp"
#include "FpConfig.hpp"

namespace Components {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

TlmDispatcher ::TlmDispatcher(const char* const compName) : TlmDispatcherComponentBase(compName) {}

TlmDispatcher ::~TlmDispatcher() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void TlmDispatcher ::comIn_handler(NATIVE_INT_TYPE portNum, Fw::ComBuffer& data, U32 context) {
    Components::OpModes opMode;
    this->getOpMode_out(0, opMode);
    if (opMode == Components::OpModes::DataTransmit ||
        opMode == Components::OpModes::PowerEmergency) {
        return;
    }
    this->comOut_out(0, data, 0);
}

}  // namespace Components
