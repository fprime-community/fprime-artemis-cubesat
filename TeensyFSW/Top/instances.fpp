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

  # ----------------------------------------------------------------------
  # Queued component instances
  # ----------------------------------------------------------------------

  # ----------------------------------------------------------------------
  # Passive component instances
  # ----------------------------------------------------------------------

  instance rateGroup1: Svc.PassiveRateGroup base id 0x1000

  instance rateGroup2: Svc.PassiveRateGroup base id 0x1100

  instance framer: Svc.Framer base id 0x4100

  instance fatalAdapter: Svc.AssertFatalAdapter base id 0x4200

  instance fatalHandler: Svc.FatalHandler base id 0x4300

  instance timeHandler: Svc.Time base id 0x4400 \
    type "Svc::ArduinoTimeImpl" \
    at "../../lib/arduino/fprime-arduino/Arduino/ArduinoTime/ArduinoTimeImpl.hpp"

  instance rateGroupDriver: Svc.RateGroupDriver base id 0x4500

  instance staticMemory: Svc.StaticMemory base id 0x4600

  instance textLogger: Svc.PassiveTextLogger base id 0x4700

  instance deframer: Svc.Deframer base id 0x4800

  instance systemResources: Svc.SystemResources base id 0x4900

  instance rateDriver: Arduino.HardwareRateDriver base id 0x4A00

  # LedBlinker

  instance blinker: Components.LedBlinker base id 0x10000

  instance gpioLed: Arduino.GpioDriver base id 0x10100

  # Radios

  instance rfm23: Radios.RFM23 base id 0x20000

  instance gpioRxOn: Arduino.GpioDriver base id 0x20100

  instance gpioTxOn: Arduino.GpioDriver base id 0x20200

  # Sensors

  instance imu: Sensors.LSM6DS base id 0x30000

  instance mag: Sensors.LIS3MDL base id 0x30100

  instance current_solar_panel_1: Sensors.INA219 base id 0x30200

  instance current_solar_panel_2: Sensors.INA219 base id 0x30300

  instance current_solar_panel_3: Sensors.INA219 base id 0x30400

  instance current_solar_panel_4: Sensors.INA219 base id 0x30500

  instance current_battery_board: Sensors.INA219 base id 0x30600

  instance temperature_obc: Sensors.TMP36 base id 0x30700

  instance temperature_pdu: Sensors.TMP36 base id 0x30800
  
  instance temperature_solar_panel_1: Sensors.TMP36 base id 0x30900

  instance temperature_solar_panel_2: Sensors.TMP36 base id 0x31000

  instance temperature_solar_panel_3: Sensors.TMP36 base id 0x31100

  instance temperature_solar_panel_4: Sensors.TMP36 base id 0x31200

  instance temperature_battery_board: Sensors.TMP36 base id 0x31300
  
  # PDU

  instance pdu: Components.PDU base id 0x40000

  instance pduCommDriver: Arduino.StreamDriver base id 0x40100

  instance rpiGpioEnable: Arduino.GpioDriver base id 0x40200

  # Comm Driver

  instance commDriver: Arduino.StreamDriver base id 0x50000
  
  instance commStub: Svc.ComStub base id 0x50100
  
  # Hub

  instance cmdSplitter: Svc.CmdSplitter base id 0x100000

  instance hub: Svc.GenericHub base id 0x100100
  
  instance hubCommDriver: Arduino.StreamDriver base id 0x100200

  instance hubFramer: Svc.Framer base id 0x100300

  instance hubDeframer: Svc.Deframer base id 0x100400

  # Analog Pins

  instance Analog0: Arduino.AnalogDriver base id 0x200000

  instance Analog1: Arduino.AnalogDriver base id 0x200100

  instance Analog6: Arduino.AnalogDriver base id 0x200200

  instance Analog7: Arduino.AnalogDriver base id 0x200300

  instance Analog8: Arduino.AnalogDriver base id 0x200400

  instance Analog9: Arduino.AnalogDriver base id 0x200500
  
  instance Analog17: Arduino.AnalogDriver base id 0x206000

}
