module ArtemisSensors {

  struct IMU_Measurement {
    gyro_x: F32
    gyro_y: F32
    gyro_z: F32
    accel_x: F32
    accel_y: F32
    accel_z: F32
    temp: F32
  }

  @ A rate group active component with input and output scheduler ports
  passive component LSM6DS {

    # ----------------------------------------------------------------------
    # General Ports
    # ----------------------------------------------------------------------

    @ Scheduler output port
    sync input port SchedIn: Svc.Sched

    # ----------------------------------------------------------------------
    # Events
    # ----------------------------------------------------------------------

    @ Signal Generator Settings Changed
    event SomeEvent(Arg: U32) \
    severity activity low \
    id 0 \
    format "SomeArg {}"


    # ----------------------------------------------------------------------
    # Telemetry channels
    # ----------------------------------------------------------------------

    telemetry IMU_Telem: IMU_Measurement id 0

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
    telemetry port Tlm

  }

}
