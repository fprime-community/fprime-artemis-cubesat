// ======================================================================
// \title  Topology.cpp
// \author mstarch
// \brief cpp file containing the topology instantiation code
//
// \copyright
// Copyright 2009-2022, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
// ======================================================================
// Provides access to autocoded functions
#include <Isc/Top/IscTopologyAc.hpp>

// Necessary project-specified types
#include <Fw/Types/MallocAllocator.hpp>
#include <Os/Log.hpp>
#include <Svc/FramingProtocol/FprimeProtocol.hpp>

// Used for 1Hz synthetic cycling
#include <Os/Mutex.hpp>

// Allows easy reference to objects in FPP/autocoder required namespaces
using namespace Isc;

// Instantiate a system logger that will handle Fw::Logger::logMsg calls
Os::Log logger;

// The reference topology uses a malloc-based allocator for components that need to allocate memory during the
// initialization phase.
Fw::MallocAllocator mallocator;

// The reference topology uses the F´ packet protocol when communicating with the ground and therefore uses the F´
// framing and deframing implementations.
Svc::FprimeFraming framing;
Svc::FprimeDeframing deframing;

// The reference topology divides the incoming clock signal (1Hz) into sub-signals: 1Hz, 1/2Hz, and 1/4Hz
NATIVE_INT_TYPE rateGroupDivisors[Svc::RateGroupDriver::DIVIDER_SIZE] = {1, 2, 4};

// Rate groups may supply a context token to each of the attached children whose purpose is set by the project. The
// reference topology sets each token to zero as these contexts are unused in this project.
NATIVE_INT_TYPE rateGroup1Context[Svc::ActiveRateGroup::CONNECTION_COUNT_MAX] = {};
NATIVE_INT_TYPE rateGroup2Context[Svc::ActiveRateGroup::CONNECTION_COUNT_MAX] = {};
NATIVE_INT_TYPE rateGroup3Context[Svc::ActiveRateGroup::CONNECTION_COUNT_MAX] = {};

// A number of constants are needed for construction of the topology. These are specified here.
enum TopologyConstants {
    CMD_SEQ_BUFFER_SIZE = 5 * 1024,
    FILE_DOWNLINK_TIMEOUT = 1000,
    FILE_DOWNLINK_COOLDOWN = 1000,
    FILE_DOWNLINK_CYCLE_TIME = 1000,
    FILE_DOWNLINK_FILE_QUEUE_DEPTH = 10,
    HEALTH_WATCHDOG_CODE = 0x123,
    COMM_PRIORITY = 100,
    UPLINK_BUFFER_MANAGER_STORE_SIZE = 3000,
    UPLINK_BUFFER_MANAGER_QUEUE_SIZE = 30,
    UPLINK_BUFFER_MANAGER_ID = 200
};


/**
 * \brief configure/setup components in project-specific way
 *
 * This is a *helper* function which configures/sets up each component requiring project specific input. This includes
 * allocating resources, passing-in arguments, etc. This function may be inlined into the topology setup function if
 * desired, but is extracted here for clarity.
 */
void configureTopology() {
    commDriver.configure(0, 115200);
    rateDriver.configure(100); // Rate group period in milliseconds
    (void) ledPin.open(13, Arduino::GpioDriver::OUT); // Open pin 13 as our LED pin
    // Command sequencer needs to allocate memory to hold contents of command sequences
    cmdSeq.allocateBuffer(0, mallocator, CMD_SEQ_BUFFER_SIZE);

    // Rate group driver needs a divisor list
    rateGroupDriverComp.configure(rateGroupDivisors, FW_NUM_ARRAY_ELEMENTS(rateGroupDivisors));

    // Rate groups require context arrays. Empty for Iscerence example.
    rateGroup1Comp.configure(rateGroup1Context, FW_NUM_ARRAY_ELEMENTS(rateGroup1Context));

    // File downlink requires some project-derived properties.
    fileDownlink.configure(FILE_DOWNLINK_TIMEOUT, FILE_DOWNLINK_COOLDOWN, FILE_DOWNLINK_CYCLE_TIME,
                           FILE_DOWNLINK_FILE_QUEUE_DEPTH);

    // Parameter database is configured with a database file name, and that file must be initially read.
    //prmDb.configure("PrmDb.dat");
    //prmDb.readParamFile();

    Svc::ComQueue::QueueConfigurationTable configurationTable;
    // Channels, deep queue, low priority
    configurationTable.entries[0] = {.depth = 500, .priority = 2};
    // Events , highest-priority
    configurationTable.entries[1] = {.depth = 100, .priority = 0};
    // File Downlink
    configurationTable.entries[2] = {.depth = 100, .priority = 1};
    // Allocation identifier is 0 as the MallocAllocator discards it
    commQueue.configure(configurationTable, 0, mallocator);

    // Buffer managers need a configured set of buckets and an allocator used to allocate memory for those buckets.
    Svc::BufferManager::BufferBins upBuffMgrBins;
    memset(&upBuffMgrBins, 0, sizeof(upBuffMgrBins));
    upBuffMgrBins.bins[0].bufferSize = UPLINK_BUFFER_MANAGER_STORE_SIZE;
    upBuffMgrBins.bins[0].numBuffers = UPLINK_BUFFER_MANAGER_QUEUE_SIZE;
    fileUplinkBufferManager.setup(UPLINK_BUFFER_MANAGER_ID, 0, mallocator, upBuffMgrBins);

    // Framer and Deframer components need to be passed a protocol handler
    framer.setup(framing);
    deframer.setup(deframing);

    // Note: Uncomment when using Svc:TlmPacketizer
    //tlmSend.setPacketList(IscPacketsPkts, IscPacketsIgnore, 1);
}

// Public functions for use in main program are namespaced with deployment name Isc
namespace Isc {
void setupTopology(const TopologyState& state) {
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
    //loadParameters();
    // Autocoded task kick-off (active components). Function provided by autocoder.
    startTasks(state);

    rateDriver.start(); // Start rate group
}

void teardownTopology(const TopologyState& state) {
    // Autocoded (active component) task clean-up. Functions provided by topology autocoder.
    stopTasks(state);
    freeThreads(state);

    // Other task clean-up.
    //comm.stopSocketTask();
    //(void)comm.joinSocketTask(nullptr);

    // Resource deallocation
    cmdSeq.deallocateBuffer(mallocator);
    fileUplinkBufferManager.cleanup();
}
};  // namespace Isc
