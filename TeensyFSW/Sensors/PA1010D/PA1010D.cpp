// ======================================================================
// \title  PA1010D.cpp
// \author root
// \brief  cpp file for PA1010D component implementation class
// ======================================================================

#include "TeensyFSW/Sensors/PA1010D/PA1010D.hpp"
#include "FpConfig.hpp"
#include "TimeLib.h"

namespace Sensors {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

// PA1010D::PA1010D(const char* const compName) : PA1010DComponentBase(compName) {}

PA1010D::PA1010D(const char* const compName) : PA1010DComponentBase(compName), gps(&GPSSerial) {
    data[0].setdata("Fix Quality");
    data[1].setdata("Satellites");
    data[2].setdata("Speed");
    data[3].setdata("Angle");
    data[4].setdata("Altitude");
}

PA1010D::~PA1010D() {}

bool PA1010D::init_gps(void) {
    gps.begin(9600);
    // Set gps modes
    gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    return true;
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void PA1010D::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
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
