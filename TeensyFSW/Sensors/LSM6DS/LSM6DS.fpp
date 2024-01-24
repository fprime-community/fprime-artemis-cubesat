module Sensors {

  array IMUTlm = [3] F32

  @ Component for the Adafruit LSM6DS
  passive component LSM6DS {

    # ----------------------------------------------------------------------
    # General Ports
    # ----------------------------------------------------------------------

    @ Scheduler output port
    sync input port run: Svc.Sched

    # ----------------------------------------------------------------------
    # Events
    # ----------------------------------------------------------------------

    # ----------------------------------------------------------------------
    # Telemetry channels
    # ----------------------------------------------------------------------

    @ Accelerometer Data
    telemetry AccelData: IMUTlm

    @ Gyroscope Data
    telemetry GyroData: IMUTlm

    @ IMU Temperature Sensor
    telemetry Temperature: F32

    # ----------------------------------------------------------------------
    # Special ports
    # ----------------------------------------------------------------------

    @ Event port for emitting events
    event port Log

    @ Event port for emitting text events
    text event port LogText

    @ A port for getting the time
    time get port Time

    @ A port for emitting telemetry
    telemetry port tlmOut

  }

}
