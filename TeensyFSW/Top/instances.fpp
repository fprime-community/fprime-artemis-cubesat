module TeensyFSW {

  # ----------------------------------------------------------------------
  # Defaults
  # ----------------------------------------------------------------------

  module Default {
    constant QUEUE_SIZE = 100
    constant STACK_SIZE = 64 * 1024
  }

  # ----------------------------------------------------------------------
  # Active component instances
  # ----------------------------------------------------------------------

  instance cmdDisp: Svc.CommandDispatcher base id 0x0100 \
    queue size Default.QUEUE_SIZE\
    stack size Default.STACK_SIZE \
    priority 101

  instance eventLogger: Svc.ActiveLogger base id 0x0200 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 98

  instance tlmSend: Svc.TlmChan base id 0x0300 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 97

  instance commQueue: Svc.ComQueue base id 0x0400 \
      queue size Default.QUEUE_SIZE \
      stack size Default.STACK_SIZE \
      priority 100

  instance fileDownlink: Svc.FileDownlink base id 0x0500 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 100

  # ----------------------------------------------------------------------
  # Queued component instances
  # ----------------------------------------------------------------------

  # ----------------------------------------------------------------------
  # Passive component instances
  # ----------------------------------------------------------------------

  instance rateGroup1: Svc.PassiveRateGroup base id 0x1000

  instance rateGroup2: Svc.PassiveRateGroup base id 0x1025

  instance rateGroup3: Svc.PassiveRateGroup base id 0x1050

  instance rateGroup4: Svc.PassiveRateGroup base id 0x1075

  instance rateGroup5: Svc.PassiveRateGroup base id 0x1100

  instance framer: Svc.Framer base id 0x1200

  instance fatalAdapter: Svc.AssertFatalAdapter base id 0x1300

  instance fatalHandler: Svc.FatalHandler base id 0x1400

  instance timeHandler: Arduino.ArduinoTime base id 0x1500

  instance rateGroupDriver: Svc.RateGroupDriver base id 0x1600

  instance staticMemory: Svc.StaticMemory base id 0x1700

  instance textLogger: Svc.PassiveTextLogger base id 0x1800

  instance deframer: Svc.Deframer base id 0x1900

  instance systemResources: Svc.SystemResources base id 0x1A00

  instance rateDriver: Arduino.HardwareRateDriver base id 0x1B00

  # LedBlinker

  instance blinker: Components.LedBlinker base id 0x2000

  instance gpioLed: Arduino.GpioDriver base id 0x2100

  # Radios

  instance rfm23: Radios.RFM23 base id 0x2200

  instance gpioRxOn: Arduino.GpioDriver base id 0x2300

  instance gpioTxOn: Arduino.GpioDriver base id 0x2400

  # Sensors

  instance imu: Sensors.LSM6DS base id 0x3000

  instance mag: Sensors.LIS3MDL base id 0x3100

  instance current_solar_panel_1: Sensors.INA219 base id 0x3200

  instance current_solar_panel_2: Sensors.INA219 base id 0x3300

  instance current_solar_panel_3: Sensors.INA219 base id 0x3400

  instance current_solar_panel_4: Sensors.INA219 base id 0x3500

  instance current_battery_board: Sensors.INA219 base id 0x3600

  instance temperature_obc: Sensors.TMP36 base id 0x3700

  instance temperature_pdu: Sensors.TMP36 base id 0x3800
  
  instance temperature_solar_panel_1: Sensors.TMP36 base id 0x3900

  instance temperature_solar_panel_2: Sensors.TMP36 base id 0x3A00

  instance temperature_solar_panel_3: Sensors.TMP36 base id 0x3B00

  instance temperature_solar_panel_4: Sensors.TMP36 base id 0x3C00

  instance temperature_battery_board: Sensors.TMP36 base id 0x3D00

  instance gps: Sensors.PA1010D base id 0x3E00
  
  # PDU

  instance pdu: Components.PDU base id 0x4000

  instance pduCommDriver: Arduino.StreamDriver base id 0x4100

  instance rpiGpioEnable: Arduino.GpioDriver base id 0x4200

  # Heater 

  instance heater: Components.Heater base id 0x4300

  # Comm Driver

  instance commDriver: Arduino.StreamDriver base id 0x4400
  
  instance commStub: Svc.ComStub base id 0x4500
  
  # Hub

  instance cmdSplitter: Svc.CmdSplitter base id 0x4600

  instance hub: Svc.GenericHub base id 0x4700
  
  instance hubCommDriver: Arduino.StreamDriver base id 0x4800

  instance hubFramer: Svc.Framer base id 0x4900

  instance hubDeframer: Svc.Deframer base id 0x4A00

  # Analog Pins

  instance Analog0: Arduino.AnalogDriver base id 0x5000

  instance Analog1: Arduino.AnalogDriver base id 0x5100

  instance Analog6: Arduino.AnalogDriver base id 0x5200

  instance Analog7: Arduino.AnalogDriver base id 0x5300

  instance Analog8: Arduino.AnalogDriver base id 0x5400

  instance Analog9: Arduino.AnalogDriver base id 0x5500
  
  instance Analog17: Arduino.AnalogDriver base id 0x5600

  # Mode Manager
  instance ModeManager: Components.ModeManager base id 0x6000
  
  # Telemetry Dispatcher
  instance TlmDispatcher: Components.TlmDispatcher base id 0x7000
}
