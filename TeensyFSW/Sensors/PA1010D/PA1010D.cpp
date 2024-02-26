// ======================================================================
// \title  PA1010D.cpp
// \author root
// \brief  cpp file for PA1010D component implementation class
// ======================================================================

#include "TeensyFSW/Sensors/PA1010D/PA1010D.hpp"
#include <TimeLib.h>
#include "FpConfig.hpp"

namespace Sensors {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

PA1010D::PA1010D(const char* const compName)
    : PA1010DComponentBase(compName), gps(&GPSSerial), enabled(false) {
    data[0].setdata("Fix Quality");
    data[1].setdata("Satellites");
    data[2].setdata("Speed");
    data[3].setdata("Angle");
    data[4].setdata("Altitude");
}

PA1010D::~PA1010D() {}

bool PA1010D::init_gps(void) {
    gps.begin(9600);
    // Set gps settings
    gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    return true;
}

// ----------------------------------------------------------------------
// Command handler implementations
// ----------------------------------------------------------------------

void PA1010D::SetGPSMode_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq, Fw::On state) {
    Components::OpModes opMode;
    this->getOpMode_out(0, opMode);

    if (opMode == Components::OpModes::Startup || opMode == Components::OpModes::Deployment ||
        opMode == Components::OpModes::PowerEmergency) {
        this->log_WARNING_HI_SetGPSDenied(opMode);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
        return;
    }
    if (state == Fw::On::ON) {
        // Wake up the GPS
        this->log_ACTIVITY_HI_GPSStatus("wakeup");
        gps.wakeup();
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
        this->enabled = true;
    } else {
        // Put GPS in standby mode
        this->log_ACTIVITY_HI_GPSStatus("standby");
        gps.standby();
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
        this->enabled = false;
    }
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void PA1010D::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    Components::OpModes opMode;
    this->getOpMode_out(0, opMode);
    if (opMode == Components::OpModes::Startup || opMode == Components::OpModes::Deployment) {
        return;
    }
    if (opMode == Components::OpModes::Initialization) {
        this->enabled = true;
    } else if (opMode == Components::OpModes::PowerEmergency && this->enabled) {
        this->log_ACTIVITY_HI_GPSStatus("standby");
        gps.standby();
        this->enabled = false;
    } else if (opMode != Components::OpModes::PowerEmergency && !this->enabled) {
        this->log_ACTIVITY_HI_GPSStatus("wakeup");
        gps.wakeup();
        this->enabled = true;
    }

    if (!this->enabled) {
        return;
    }

    // Read data from the gps
    gps.read();

    // If a new NMEA sentence is received, parse it and store the data in an array
    if (gps.newNMEAreceived()) {
        if (gps.parse(gps.lastNMEA())) {
            if (gps.fix) {
                data[0].setval(gps.fixquality);
                data[1].setval(gps.satellites);
                data[2].setval(gps.speed);
                data[3].setval(gps.angle);
                data[4].setval(gps.altitude);
                this->tlmWrite_GPSTlm(data);
                setTime(gps.hour, gps.minute, gps.seconds, gps.day, gps.month, gps.year);
            }
        }
    }
}

}  // namespace Sensors
