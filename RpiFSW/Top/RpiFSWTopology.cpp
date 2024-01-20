// ======================================================================
// \title  RPIFSWTopology.cpp
// \brief cpp file containing the topology instantiation code
//
// ======================================================================
// Provides access to autocoded functions
#include <RpiFSW/Top/RpiFSWPacketsAc.hpp>
#include <RpiFSW/Top/RpiFSWTopologyAc.hpp>
#include <RpiFSW/Top/RpiFSWTopologyDefs.hpp>
#include <Utils/Hash/HashConfig.hpp>

// Necessary project-specified types
#include <Fw/Types/MallocAllocator.hpp>
#include <Os/Log.hpp>
#include <Svc/FramingProtocol/FprimeProtocol.hpp>

// Used for 1Hz synthetic cycling
#include <Os/Mutex.hpp>

// Allows easy reference to objects in FPP/autocoder required namespaces
using namespace RpiFSW;

// Instantiate a system logger that will handle Fw::Logger::logMsg calls
Os::Log logger;

// The reference topology uses a malloc-based allocator for components that need to allocate memory
// during the initialization phase.
Fw::MallocAllocator mallocator;

// The reference topology uses the F´ packet protocol when communicating with the ground and
// therefore uses the F´ framing and deframing implementations.
Svc::FprimeFraming framing;
Svc::FprimeDeframing deframing;

// The reference topology divides the incoming clock signal (1Hz) into sub-signals: 1Hz, 1/2Hz, and
// 1/4Hz
Svc::RateGroupDriver::DividerSet rateGroupDivisors = {{ {1, 0}, {100, 0}, {1000, 0} }};

// Rate groups may supply a context token to each of the attached children whose purpose is set by
// the project. The reference topology sets each token to zero as these contexts are unused in this
// project.
NATIVE_INT_TYPE rateGroup1Context[Svc::ActiveRateGroup::CONNECTION_COUNT_MAX] = {};
NATIVE_INT_TYPE rateGroup2Context[Svc::ActiveRateGroup::CONNECTION_COUNT_MAX] = {};
NATIVE_INT_TYPE rateGroup3Context[Svc::ActiveRateGroup::CONNECTION_COUNT_MAX] = {};

// A number of constants are needed for construction of the topology. These are specified here.
enum TopologyConstants {
    CMD_SEQ_BUFFER_SIZE            = 5 * 1024,
    FILE_DOWNLINK_TIMEOUT          = 30000,
    FILE_DOWNLINK_COOLDOWN         = 1000,
    FILE_DOWNLINK_CYCLE_TIME       = 1000,
    FILE_DOWNLINK_FILE_QUEUE_DEPTH = 10,
    HEALTH_WATCHDOG_CODE           = 0x123,
    COMM_PRIORITY                  = 100,
    // bufferManager constants
    FRAMER_BUFFER_SIZE = FW_MAX(FW_COM_BUFFER_MAX_SIZE, FW_FILE_BUFFER_MAX_SIZE + sizeof(U32)) +
                         HASH_DIGEST_LENGTH + Svc::FpFrameHeader::SIZE,
    FRAMER_BUFFER_COUNT     = 30,
    DEFRAMER_BUFFER_SIZE    = FW_MAX(FW_COM_BUFFER_MAX_SIZE, FW_FILE_BUFFER_MAX_SIZE + sizeof(U32)),
    DEFRAMER_BUFFER_COUNT   = 30,
    COM_DRIVER_BUFFER_SIZE  = 3000,
    COM_DRIVER_BUFFER_COUNT = 30,
    BUFFER_MANAGER_ID       = 200
};

// Ping entries are autocoded, however; this code is not properly exported. Thus, it is copied here.
Svc::Health::PingEntry pingEntries[] = {
    {PingEntries::rpi_blockDrv::WARN, PingEntries::rpi_blockDrv::FATAL, "blockDrv"},
    {PingEntries::rpi_cmdDisp::WARN, PingEntries::rpi_cmdDisp::FATAL, "cmdDisp"},
    {PingEntries::rpi_fileDownlink::WARN, PingEntries::rpi_fileDownlink::FATAL, "fileDownlink"},
    {PingEntries::rpi_fileManager::WARN, PingEntries::rpi_fileManager::FATAL, "fileManager"},
    {PingEntries::rpi_prmDb::WARN, PingEntries::rpi_prmDb::FATAL, "prmDb"},
    {PingEntries::rpi_rateGroup1::WARN, PingEntries::rpi_rateGroup1::FATAL, "rateGroup1"},
    {PingEntries::rpi_rateGroup2::WARN, PingEntries::rpi_rateGroup2::FATAL, "rateGroup2"},
    {PingEntries::rpi_rateGroup3::WARN, PingEntries::rpi_rateGroup3::FATAL, "rateGroup3"},
};

/**
 * \brief configure/setup components in project-specific way
 *
 * This is a *helper* function which configures/sets up each component requiring project specific
 * input. This includes allocating resources, passing-in arguments, etc. This function may be
 * inlined into the topology setup function if desired, but is extracted here for clarity.
 */
void configureTopology() {
    // Buffer managers need a configured set of buckets and an allocator used to allocate memory for
    // those buckets.
    Svc::BufferManager::BufferBins upBuffMgrBins;
    memset(&upBuffMgrBins, 0, sizeof(upBuffMgrBins));
    upBuffMgrBins.bins[0].bufferSize = FRAMER_BUFFER_SIZE;
    upBuffMgrBins.bins[0].numBuffers = FRAMER_BUFFER_COUNT;
    upBuffMgrBins.bins[1].bufferSize = DEFRAMER_BUFFER_SIZE;
    upBuffMgrBins.bins[1].numBuffers = DEFRAMER_BUFFER_COUNT;
    upBuffMgrBins.bins[2].bufferSize = COM_DRIVER_BUFFER_SIZE;
    upBuffMgrBins.bins[2].numBuffers = COM_DRIVER_BUFFER_COUNT;
    upBuffMgrBins.bins[3].bufferSize = 1024 * 1024 * 32;
    upBuffMgrBins.bins[3].numBuffers = 3;
    rpi_bufferManager.setup(BUFFER_MANAGER_ID, 0, mallocator, upBuffMgrBins);

    rpi_hubFramer.setup(framing);
    rpi_hubDeframer.setup(deframing);

    // Rate group driver needs a divisor list
    rpi_rateGroupDriver.configure(rateGroupDivisors);

    // Rate groups require context arrays.
    rpi_rateGroup1.configure(rateGroup1Context, FW_NUM_ARRAY_ELEMENTS(rateGroup1Context));
    rpi_rateGroup2.configure(rateGroup2Context, FW_NUM_ARRAY_ELEMENTS(rateGroup2Context));
    rpi_rateGroup3.configure(rateGroup3Context, FW_NUM_ARRAY_ELEMENTS(rateGroup3Context));

    // File downlink requires some project-derived properties.
    rpi_fileDownlink.configure(FILE_DOWNLINK_TIMEOUT, FILE_DOWNLINK_COOLDOWN,
                               FILE_DOWNLINK_CYCLE_TIME, FILE_DOWNLINK_FILE_QUEUE_DEPTH);

    // Parameter database is configured with a database file name, and that file must be initially
    // read.
    rpi_prmDb.configure("PrmDb.dat");
    rpi_prmDb.readParamFile();

    // Health is supplied a set of ping entires.
    rpi_health.setPingEntries(pingEntries, FW_NUM_ARRAY_ELEMENTS(pingEntries),
                              HEALTH_WATCHDOG_CODE);

    // Note: Uncomment when using Svc:TlmPacketizer
    // tlmSend.setPacketList(RPIFSWPacketsPkts, RPIFSWPacketsIgnore, 1);
}

// Public functions for use in main program are namespaced with deployment name RPIFSW
namespace RpiFSW {
void setupTopology(const TopologyState& state) {
    // Autocoded initialization. Function provided by autocoder.
    initComponents(state);
    // Autocoded id setup. Function provided by autocoder.
    setBaseIds();
    // Autocoded connection wiring. Function provided by autocoder.
    connectComponents();
    // Autocoded command registration. Function provided by autocoder.
    configureTopology();
    // Autocoded parameter loading. Function provided by autocoder.
    loadParameters();
    // Autocoded task kick-off (active components). Function provided by autocoder.
    regCommands();
    // Project-specific component configuration. Function provided above. May be inlined, if
    // desired.

    startTasks(state);

    if (rpi_hubDriver.open("/dev/ttyS0", Drv::LinuxUartDriver::UartBaudRate::BAUD_115K,
                           Drv::LinuxUartDriver::NO_FLOW, Drv::LinuxUartDriver::PARITY_NONE,
                           1024)) {
        rpi_hubDriver.startReadThread(100, Default::STACK_SIZE);
        Fw::Logger::logMsg("UART port successfully opened\n");
    } else {
        Fw::Logger::logMsg("Failed to open UART port %s at speed %" PRIu32 "\n",
                           reinterpret_cast<POINTER_CAST>("/dev/ttyS0"),
                           reinterpret_cast<POINTER_CAST>("BAUD_115K"));
    }

    if (!rpi_camera.open(0)) {
        Fw::Logger::logMsg("Camera failed to open\n");
    }
    const char* const name = "/home/pi/images/saveImage";
    const char* const type = ".dat";
    rpi_saveImageBufferLogger.initLog(name, type, 1024 * 1024, 4);
}

// Variables used for cycle simulation
Os::Mutex cycleLock;
volatile bool cycleFlag = true;

void startSimulatedCycle(U32 milliseconds) {
    cycleLock.lock();
    bool cycling = cycleFlag;
    cycleLock.unLock();

    // Main loop
    while (cycling) {
        RpiFSW::rpi_blockDrv.callIsr();
        Os::Task::delay(milliseconds);

        cycleLock.lock();
        cycling = cycleFlag;
        cycleLock.unLock();
    }
}

void stopSimulatedCycle() {
    cycleLock.lock();
    cycleFlag = false;
    cycleLock.unLock();
}

void teardownTopology(const TopologyState& state) {
    // Autocoded (active component) task clean-up. Functions provided by topology autocoder.
    stopTasks(state);
    freeThreads(state);
}
};  // namespace RpiFSW