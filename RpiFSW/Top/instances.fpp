module RpiFSW {
  constant RPI_TOPOLOGY_BASE = 0x10000000
  # ----------------------------------------------------------------------
  # Defaults
  # ----------------------------------------------------------------------

  module Default {
    constant QUEUE_SIZE = 10
    constant STACK_SIZE = 64 * 1024
  }

  # ----------------------------------------------------------------------
  # Active component instances
  # ----------------------------------------------------------------------

  instance rpi_blockDrv: Drv.BlockDriver base id RPI_TOPOLOGY_BASE + 0x0100 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 140

  instance rpi_rateGroup1: Svc.ActiveRateGroup base id RPI_TOPOLOGY_BASE + 0x0200 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 120

  instance rpi_rateGroup2: Svc.ActiveRateGroup base id RPI_TOPOLOGY_BASE + 0x0300 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 119

  instance rpi_rateGroup3: Svc.ActiveRateGroup base id RPI_TOPOLOGY_BASE + 0x0400 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 118

  instance rpi_cmdDisp: Svc.CommandDispatcher base id RPI_TOPOLOGY_BASE +  0x0500 \
    queue size 20 \
    stack size Default.STACK_SIZE \
    priority 101

  instance rpi_fileDownlink: Svc.FileDownlink base id RPI_TOPOLOGY_BASE + 0x0700 \
    queue size 30 \
    stack size Default.STACK_SIZE \
    priority 100

  instance rpi_fileManager: Svc.FileManager base id RPI_TOPOLOGY_BASE + 0x0800 \
    queue size 30 \
    stack size Default.STACK_SIZE \
    priority 100

  instance rpi_prmDb: Svc.PrmDb base id RPI_TOPOLOGY_BASE + 0x0D00 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 96

  instance rpi_camera: Payload.Camera base id RPI_TOPOLOGY_BASE + 0x1000 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 100

  instance rpi_saveImageBufferLogger: Svc.BufferLogger base id RPI_TOPOLOGY_BASE + 0x1100 \
    queue size 30 \
    stack size Default.STACK_SIZE \
    priority 100 \

  # ----------------------------------------------------------------------
  # Queued component instances
  # ----------------------------------------------------------------------

  instance rpi_health: Svc.Health base id RPI_TOPOLOGY_BASE + 0x2000 \
    queue size 25

  # ----------------------------------------------------------------------
  # Passive component instances
  # ----------------------------------------------------------------------

  instance rpi_fatalAdapter: Svc.AssertFatalAdapter base id RPI_TOPOLOGY_BASE + 0x4200

  instance rpi_fatalHandler: Svc.FatalHandler base id RPI_TOPOLOGY_BASE + 0x4300

  instance rpi_bufferManager: Svc.BufferManager base id RPI_TOPOLOGY_BASE + 0x4400

  instance rpi_linuxTime: Svc.PosixTime base id RPI_TOPOLOGY_BASE + 0x4500

  instance rpi_rateGroupDriver: Svc.RateGroupDriver base id RPI_TOPOLOGY_BASE + 0x4600

  instance rpi_staticMemory: Svc.StaticMemory base id RPI_TOPOLOGY_BASE + 0x4700

  instance rpi_textLogger: Svc.PassiveTextLogger base id RPI_TOPOLOGY_BASE + 0x4800

  instance rpi_systemResources: Svc.SystemResources base id RPI_TOPOLOGY_BASE + 0x4A00


  instance rpi_hub: Svc.GenericHub base id RPI_TOPOLOGY_BASE + 0x100000

  instance rpi_hubFramer: Svc.Framer base id RPI_TOPOLOGY_BASE + 0x100100

  instance rpi_hubDeframer: Svc.Deframer base id RPI_TOPOLOGY_BASE + 0x100200

  instance rpi_hubDriver: Drv.LinuxUartDriver base id RPI_TOPOLOGY_BASE + 0x100300

}