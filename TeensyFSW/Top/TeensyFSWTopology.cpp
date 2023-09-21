// ======================================================================
// \title  TeensyFSWTopology.cpp
// \brief cpp file containing the topology instantiation code
//
// ======================================================================
// Provides access to autocoded functions
#include <TeensyFSW/Top/TeensyFSWTopologyAc.hpp>
#include <TeensyFSW/Top/TeensyFSWPacketsAc.hpp>
#include <config/FppConstantsAc.hpp>

// Necessary project-specified types
#include <Fw/Types/MallocAllocator.hpp>
#include <Svc/FramingProtocol/FprimeProtocol.hpp>

#include <Wire.h>

// Allows easy reference to objects in FPP/autocoder required namespaces
using namespace TeensyFSW;

// The reference topology uses a malloc-based allocator for components that need to allocate memory during the
// initialization phase.
Fw::MallocAllocator mallocator;

// The reference topology uses the F´ packet protocol when communicating with the ground and therefore uses the F´
// framing and deframing implementations.
Svc::FprimeFraming framing;
Svc::FprimeDeframing deframing;

Svc::FprimeFraming hubframing;
Svc::FprimeDeframing hubdeframing;

// The reference topology divides the incoming clock signal (1Hz) into sub-signals: 1/100Hz, 1/200Hz, and 1/1000Hz
NATIVE_INT_TYPE rateGroupDivisors[Svc::RateGroupDriver::DIVIDER_SIZE] = {100, 1000};

// Rate groups may supply a context token to each of the attached children whose purpose is set by the project. The
// reference topology sets each token to zero as these contexts are unused in this project.
NATIVE_INT_TYPE rateGroup1Context[FppConstant_PassiveRateGroupOutputPorts::PassiveRateGroupOutputPorts] = {};
NATIVE_INT_TYPE rateGroup2Context[FppConstant_PassiveRateGroupOutputPorts::PassiveRateGroupOutputPorts] = {};

/**
 * \brief configure/setup components in project-specific way
 *
 * This is a *helper* function which configures/sets up each component requiring project specific input. This includes
 * allocating resources, passing-in arguments, etc. This function may be inlined into the topology setup function if
 * desired, but is extracted here for clarity.
 */
void configureTopology()
{
    // Rate group driver needs a divisor list
    rateGroupDriver.configure(rateGroupDivisors, FW_NUM_ARRAY_ELEMENTS(rateGroupDivisors));

    // Rate groups require context arrays.
    rateGroup1.configure(rateGroup1Context, FW_NUM_ARRAY_ELEMENTS(rateGroup1Context));
    rateGroup2.configure(rateGroup2Context, FW_NUM_ARRAY_ELEMENTS(rateGroup2Context));

    // Set up ComQueue
    Svc::ComQueue::QueueConfigurationTable configurationTable;
    // Channels, deep queue, low priority
    configurationTable.entries[0] = {.depth = 100, .priority = 1};
    // Events , highest-priority
    configurationTable.entries[1] = {.depth = 100, .priority = 0};
    // ???
    configurationTable.entries[2] = {.depth = 1, .priority = 2};
    // Allocation identifier is 0 as the MallocAllocator discards it
    commQueue.configure(configurationTable, 0, mallocator);

    // Framer and Deframer components need to be passed a protocol handler
    framer.setup(framing);
    deframer.setup(deframing);
    hubFramer.setup(hubframing);
    hubDeframer.setup(hubdeframing);
}

// Public functions for use in main program are namespaced with deployment name TeensyFSW
namespace TeensyFSW
{
    void setupTopology(const TopologyState &state)
    {
        // Autocoded initialization. Function provided by autocoder.
        initComponents(state);
        // Autocoded id setup. Function provided by autocoder.
        setBaseIds();
        // Autocoded connection wiring. Function provided by autocoder.
        connectComponents();
        // Autocoded command registration. Function provided by autocoder.
        regCommands();
        // Project-specific component configuration. Function provided above. May be inlined, if desired.
        configureTopology();
        // Autocoded parameter loading. Function provided by autocoder.
        // loadParameters();
        // Autocoded task kick-off (active components). Function provided by autocoder.
        startTasks(state);

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
        Analog0.open(A0, Arduino::AnalogDriver::GpioDirection::IN);
        Analog1.open(A1, Arduino::AnalogDriver::GpioDirection::IN);
        Analog6.open(A6, Arduino::AnalogDriver::GpioDirection::IN);
        Analog7.open(A7, Arduino::AnalogDriver::GpioDirection::IN);
        Analog8.open(A8, Arduino::AnalogDriver::GpioDirection::IN);
        Analog9.open(A9, Arduino::AnalogDriver::GpioDirection::IN);
        Analog17.open(A17, Arduino::AnalogDriver::GpioDirection::IN);

        rateDriver.start();
    }

    void teardownTopology(const TopologyState &state)
    {
        // Autocoded (active component) task clean-up. Functions provided by topology autocoder.
        stopTasks(state);
        freeThreads(state);
    }
}; // namespace TeensyFSW
