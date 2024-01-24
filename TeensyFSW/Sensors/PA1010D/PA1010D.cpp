// ======================================================================
// \title  PA1010D.cpp
// \author root
// \brief  cpp file for PA1010D component implementation class
// ======================================================================

#include "TeensyFSW/Sensors/PA1010D/PA1010D.hpp"
#include "FpConfig.hpp"

namespace Sensors {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

// PA1010D::PA1010D(const char* const compName) : PA1010DComponentBase(compName) {}

PA1010D::PA1010D(const char* const compName) : PA1010DComponentBase(compName), gps(&GPSSerial) {
    data[0].setdata("Time");
    data[1].setdata("Date");
    data[2].setdata("Fix Quality");
    data[3].setdata("Satellites");
    data[4].setdata("Speed");
    data[5].setdata("Angle");
    data[6].setdata("Altitude");
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
                data[0].setval(gps.hour * 3600 + gps.minute * 60 + gps.seconds);
                data[1].setval((gps.year % 100) * 10000 + gps.month * 100 + gps.day);
                data[2].setval(gps.fixquality);
                data[3].setval(gps.satellites);
                data[4].setval(gps.speed);
                data[5].setval(gps.angle);
                data[6].setval(gps.altitude);
                this->tlmWrite_GPSTlm(data);
            }
        }
    }
}

}  // namespace Sensors
