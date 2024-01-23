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

PA1010D::PA1010D(const char* const compName) : PA1010DComponentBase(compName), gps(&GPSSerial) {}

void PA1010D::init(const NATIVE_INT_TYPE instance) {}

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
                Sensors::GPSTlmData data;
                data[0] = {"Time", gps.hour * 3600 + gps.minute * 60 + gps.seconds};
                data[1] = {"Date", (gps.year % 100) * 10000 + gps.month * 100 + gps.day};
                data[2] = {"Fix Quality", gps.fixquality};
                data[3] = {"Satellites", gps.satellites};
                data[4] = {"Speed", gps.speed};
                data[5] = {"Angle", gps.angle};
                data[6] = {"Altitude", gps.altitude};
                this->tlmWrite_GPSTlm(data);
            }
        }
    }
}

}  // namespace Sensors
