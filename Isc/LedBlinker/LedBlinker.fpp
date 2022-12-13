module ArduinoBlink {

  passive component LedBlinker {

    sync input port run: [1] Svc.Sched

    output port gpio: [1] Drv.GpioWrite

    @ A port for getting the time
    time get port timeGetOut

    @ A port for emitting telemetry
    telemetry port tlmOut

    @ A port for emitting events
    event port eventOut

    @ A port for emitting text events
    text event port textEventOut

    @ Number of blinks since startup
    telemetry numBlinks: U32 id 0
  }

}
