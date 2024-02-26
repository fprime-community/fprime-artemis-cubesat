// ======================================================================
// \title  Main.cpp
// \brief main program for the F' application. Intended for CLI-based systems
// (Linux, macOS)
//
// ======================================================================
// Used to access topology functions
#include <TeensyFSW/Top/TeensyFSWTopology.hpp>
#include <TeensyFSW/Top/TeensyFSWTopologyAc.hpp>
// Used for Task Runner
#include <Os/Baremetal/TaskRunner/TaskRunner.hpp>

// Used for logging
#include <Arduino/Os/StreamLog.hpp>
#include <Os/Log.hpp>

#include <Components/Radios/RFM23/RFM23.hpp>

#include <SD.h>

// Instantiate a system logger that will handle Fw::Logger::logMsg calls
Os::Log logger;

// Task Runner
Os::TaskRunner taskrunner;

/**
 * \brief setup the program
 *
 * This is an extraction of the Arduino setup() function.
 */
void setup() {
    // Setup Serial
    Serial.begin(115200);
    Serial1.begin(115200);
    Serial2.begin(115200);
    SD.begin(BUILTIN_SDCARD);
    SPI1.setMISO(Radios::RFM23::SPI_MISO);
    SPI1.setMOSI(Radios::RFM23::SPI_MOSI);
    SPI1.setSCK(Radios::RFM23::SPI_SCK);

    Os::setArduinoStreamLogHandler(&Serial);
    delay(1000);
    Fw::Logger::logMsg("Program Started\n");

    // Object for communicating state to the reference topology
    TeensyFSW::TopologyState inputs;
    inputs.uartNumber = 0;
    inputs.uartBaud   = 115200;

    // Setup topology
    TeensyFSW::setupTopology(inputs);

    Fw::Logger::logMsg("[INFO] System Topology Constructed");
}

/**
 * \brief run the program
 *
 * This is an extraction of the Arduino loop() function.
 *
 */
void loop() {
#ifdef USE_BASIC_TIMER
    rateDriver.cycle();
#endif
    taskrunner.run();
}