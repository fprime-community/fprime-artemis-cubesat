// ======================================================================
// \title  LedBlinker.cpp
// \author ethan
// \brief  cpp file for LedBlinker component implementation class
// ======================================================================

#include <Components/LedBlinker/LedBlinker.hpp>
#include <FpConfig.hpp>

namespace Components {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

LedBlinker::LedBlinker(const char* const compName)
    : LedBlinkerComponentBase(compName),
      state(Fw::On::OFF),
      transitions(0),
      count(0),
      blinking(true) {}

LedBlinker::~LedBlinker() {}

void LedBlinker::parameterUpdated(FwPrmIdType id) {
    // Read back the parameter value
    Fw::ParamValid isValid;
    U32 interval = this->paramGet_BLINK_INTERVAL(isValid);
    // NOTE: isValid is always VALID
    FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);

    // Check the parameter ID is expected
    if (PARAMID_BLINK_INTERVAL == id) {
        // Emit the blink set event
        this->log_ACTIVITY_HI_BlinkIntervalSet(interval);
    }
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void LedBlinker::run_handler(const NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    // Read back the parameter value
    Fw::ParamValid isValid;
    U16 interval = this->paramGet_BLINK_INTERVAL(isValid);

    // Force interval to be 0 when invalid or not set
    interval = ((Fw::ParamValid::INVALID == isValid) || (Fw::ParamValid::UNINIT == isValid))
                   ? 0
                   : interval;

    // Only perform actions when set to blinking
    if (this->blinking) {
        Fw::On new_state = this->state;
        // Check for transitions
        if ((0 == this->count) && (this->state == Fw::On::OFF)) {
            new_state = Fw::On::ON;
        } else if (((interval / 2) == this->count) && (this->state == Fw::On::ON)) {
            new_state = Fw::On::OFF;
        }

        // A transition has occurred
        if (this->state != new_state) {
            this->transitions = this->transitions + 1;
            this->tlmWrite_LedTransitions(this->transitions);

            // Port may not be connected, so check before sending output
            if (this->isConnected_gpioSet_OutputPort(0)) {
                this->gpioSet_out(0, (Fw::On::ON == new_state) ? Fw::Logic::HIGH : Fw::Logic::LOW);
            }

            // this->log_DIAGNOSTIC_LedState(new_state);
            this->state = new_state;
        }

        this->count = ((this->count + 1) >= interval) ? 0 : (this->count + 1);
    }
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void LedBlinker::BLINKING_ON_OFF_cmdHandler(const FwOpcodeType opCode,
                                            const U32 cmdSeq,
                                            Fw::On on_off) {
    // Create a variable to represent the command response
    auto cmdResp = Fw::CmdResponse::OK;

    // Verify if on_off is a valid argument.
    // Note: isValid is an autogenerate helper function for enums defined in fpp.
    if (!on_off.isValid()) {
        this->log_WARNING_LO_InvalidBlinkArgument(on_off);

        // Update command response with a validation error
        cmdResp = Fw::CmdResponse::VALIDATION_ERROR;
    } else {
        this->count    = 0;                     // Reset count on any successful command
        this->blinking = Fw::On::ON == on_off;  // Update blinking state

        this->log_ACTIVITY_HI_SetBlinkingState(on_off);
        this->tlmWrite_BlinkingState(on_off);
    }

    // Provide command response
    this->cmdResponse_out(opCode, cmdSeq, cmdResp);
}

}  // end namespace Components
