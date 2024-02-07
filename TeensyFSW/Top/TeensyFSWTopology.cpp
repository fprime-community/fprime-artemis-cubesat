// ======================================================================
// \title  TeensyFSWTopology.cpp
// \brief cpp file containing the topology instantiation code
//
// ======================================================================
// Provides access to autocoded functions
#include <TeensyFSW/Top/TeensyFSWPacketsAc.hpp>
#include <TeensyFSW/Top/TeensyFSWTopologyAc.hpp>
#include <config/FppConstantsAc.hpp>

// Necessary project-specified types
#include <Fw/Types/MallocAllocator.hpp>
#include <Svc/FramingProtocol/FprimeProtocol.hpp>

#include <Wire.h>

// Allows easy reference to objects in FPP/autocoder required namespaces
using namespace TeensyFSW;

// The reference topology uses a malloc-based allocator for components that need
// to allocate memory during the initialization phase.
Fw::MallocAllocator mallocator;

// The reference topology uses the F´ packet protocol when communicating with
// the ground and therefore uses the F´ framing and deframing implementations.
Svc::FprimeFraming framing;
Svc::FprimeDeframing deframing;

Svc::FprimeFraming hubframing;
Svc::FprimeDeframing hubdeframing;

// The reference topology divides the incoming clock signal (1Hz) into
// sub-signals: 1/100Hz, 1/200Hz, and 1/1000Hz
Svc::RateGroupDriver::DividerSet rateGroupDivisors = {
    {{1, 0}, {100, 0}, {1000, 0}, {1500, 0}, {10000, 0}}};

// Rate groups may supply a context token to each of the attached children whose
// purpose is set by the project. The reference topology sets each token to zero
// as these contexts are unused in this project.
NATIVE_INT_TYPE
rateGroup1Context[FppConstant_PassiveRateGroupOutputPorts::PassiveRateGroupOutputPorts] = {};
NATIVE_INT_TYPE
rateGroup2Context[FppConstant_PassiveRateGroupOutputPorts::PassiveRateGroupOutputPorts] = {};
NATIVE_INT_TYPE
rateGroup3Context[FppConstant_PassiveRateGroupOutputPorts::PassiveRateGroupOutputPorts] = {};
NATIVE_INT_TYPE
rateGroup4Context[FppConstant_PassiveRateGroupOutputPorts::PassiveRateGroupOutputPorts] = {};
NATIVE_INT_TYPE
rateGroup5Context[FppConstant_PassiveRateGroupOutputPorts::PassiveRateGroupOutputPorts] = {};

// A number of constants are needed for construction of the topology. These are specified here.
enum TopologyConstants {
    FILE_DOWNLINK_TIMEOUT          = 30000,
    FILE_DOWNLINK_COOLDOWN         = 1000,
    FILE_DOWNLINK_CYCLE_TIME       = 1000,
    FILE_DOWNLINK_FILE_QUEUE_DEPTH = 10,
};

/**
 * \brief configure/setup components in project-specific way
 *
 * This is a *helper* function which configures/sets up each component requiring
 * project specific input. This includes allocating resources, passing-in
 * arguments, etc. This function may be inlined into the topology setup function
 * if desired, but is extracted here for clarity.
 */
void configureTopology() {
    // Rate group driver needs a divisor list
    rateGroupDriver.configure(rateGroupDivisors);

    // Rate groups require context arrays.
    rateGroup1.configure(rateGroup1Context, FW_NUM_ARRAY_ELEMENTS(rateGroup1Context));
    rateGroup2.configure(rateGroup2Context, FW_NUM_ARRAY_ELEMENTS(rateGroup2Context));
    rateGroup3.configure(rateGroup3Context, FW_NUM_ARRAY_ELEMENTS(rateGroup3Context));
    rateGroup4.configure(rateGroup4Context, FW_NUM_ARRAY_ELEMENTS(rateGroup4Context));
    rateGroup5.configure(rateGroup5Context, FW_NUM_ARRAY_ELEMENTS(rateGroup5Context));

    // File downlink requires some project-derived properties. a
    fileDownlink.configure(FILE_DOWNLINK_TIMEOUT, FILE_DOWNLINK_COOLDOWN, FILE_DOWNLINK_CYCLE_TIME,
                           FILE_DOWNLINK_FILE_QUEUE_DEPTH);

    // Set up ComQueue
    Svc::ComQueue::QueueConfigurationTable configurationTable;
    // Channels, deep queue, low priority
    configurationTable.entries[0] = {.depth = 100, .priority = 1};
    // Events , highest-priority
    configurationTable.entries[1] = {.depth = 100, .priority = 0};
    // ???
    configurationTable.entries[2] = {.depth = 100, .priority = 2};
    // Allocation identifier is 0 as the MallocAllocator discards it
    commQueue.configure(configurationTable, 0, mallocator);

    // Framer and Deframer components need to be passed a protocol handler
    framer.setup(framing);
    deframer.setup(deframing);
    hubFramer.setup(hubframing);
    hubDeframer.setup(hubdeframing);
}

// Public functions for use in main program are namespaced with deployment name
// TeensyFSW
namespace TeensyFSW {
void setupTopology(const TopologyState& state) {
    // Autocoded initialization. Function provided by autocoder.
    initComponents(state);
    // Autocoded id setup. Function provided by autocoder.
    setBaseIds();
    // Autocoded connection wiring. Function provided by autocoder.
    connectComponents();
    // Autocoded command registration. Function provided by autocoder.
    regCommands();
    // Project-specific component configuration. Function provided above. May be
    // inlined, if desired.
    configureTopology();
    // Autocoded parameter loading. Function provided by autocoder.
    // loadParameters();
    // Autocoded task kick-off (active components). Function provided by
    // autocoder.
    startTasks(state);

    cmdSplitter.configure(0x10000000);

    rateDriver.configure(1);
    commDriver.configure(&Serial);
    pduCommDriver.configure(&Serial1);
    hubCommDriver.configure(&Serial2);

    // Configure GPIO pins
    gpioLed.open(Arduino::DEF_LED_BUILTIN, Arduino::GpioDriver::GpioDirection::OUT);
    gpioRxOn.open(ARTEMIS_TEENSY_PINOUT::RX_ON, Arduino::GpioDriver::GpioDirection::OUT);
    gpioTxOn.open(ARTEMIS_TEENSY_PINOUT::TX_ON, Arduino::GpioDriver::GpioDirection::OUT);
    rpiGpioEnable.open(ARTEMIS_TEENSY_PINOUT::RPI_ENABLE, Arduino::GpioDriver::GpioDirection::OUT);

    mag.init_mag();
    imu.init_imu();
    current_solar_panel_1.config(0x40, &Wire2);
    current_solar_panel_2.config(0x41, &Wire2);
    current_solar_panel_3.config(0x42, &Wire2);
    current_solar_panel_4.config(0x43, &Wire2);
    current_battery_board.config(0x44, &Wire2);
    Analog0.open(ARTEMIS_TEENSY_PINOUT::AIN0, Arduino::AnalogDriver::GpioDirection::IN);
    Analog1.open(ARTEMIS_TEENSY_PINOUT::AIN1, Arduino::AnalogDriver::GpioDirection::IN);
    Analog6.open(ARTEMIS_TEENSY_PINOUT::AIN3, Arduino::AnalogDriver::GpioDirection::IN);
    Analog7.open(ARTEMIS_TEENSY_PINOUT::AIN4, Arduino::AnalogDriver::GpioDirection::IN);
    Analog8.open(ARTEMIS_TEENSY_PINOUT::AIN5, Arduino::AnalogDriver::GpioDirection::IN);
    Analog9.open(ARTEMIS_TEENSY_PINOUT::AIN6, Arduino::AnalogDriver::GpioDirection::IN);
    Analog17.open(ARTEMIS_TEENSY_PINOUT::AIN2, Arduino::AnalogDriver::GpioDirection::IN);
    gps.init_gps();

    rateDriver.start();
}

void teardownTopology(const TopologyState& state) {
    // Autocoded (active component) task clean-up. Functions provided by
    // topology autocoder.
    stopTasks(state);
    freeThreads(state);
}
};  // namespace TeensyFSW
