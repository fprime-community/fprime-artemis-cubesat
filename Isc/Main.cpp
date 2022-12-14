// ======================================================================
// \title  Main.cpp
// \author mstarch
// \brief main program for reference application. Intended for CLI-based systems (Linux, macOS)
//
// \copyright
// Copyright 2009-2022, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
// ======================================================================
// Used to access topology functions
#include <Isc/Top/IscTopology.hpp>
#include <Os/Log.hpp>
#include <FprimeArduino.hpp>
#include <Os/Baremetal/TaskRunner/TaskRunner.hpp>
#include <Arduino/Os/StreamLog.hpp>

extern "C" {
int _write( int handle, char *buf, int count) {
    Serial2.write(buf, count);
    return count;
}
};
#include <new>
namespace std {
    const std::nothrow_t nothrow;
}
void* operator new(unsigned int size, std::nothrow_t const&){ return malloc(size);}
void* operator new[](unsigned int size, std::nothrow_t const&){ return malloc(size);}

Os::TaskRunner taskRunner;

/**
 * \brief execute the program
 *
 * This F´ program is designed to run in standard environments (e.g. Linux/macOs running on a laptop). Thus it uses
 * command line inputs to specify how to connect.
 *
 */
int main() {
    Serial2.begin(115200);
    Os::setArduinoStreamLogHandler(&Serial2);
    Isc::TopologyState state;
    Isc::setupTopology(state);
    digitalWrite(13, 1);
    Fw::Logger::logMsg("[INFO] System Topology Constructed");
    while (1) {
        taskRunner.run();
    }
}
