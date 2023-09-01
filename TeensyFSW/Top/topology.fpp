module TeensyFSW {

  # ----------------------------------------------------------------------
  # Symbolic constants for port numbers
  # ----------------------------------------------------------------------

    enum Ports_RateGroups {
      rateGroup1
      rateGroup2
    }

    enum Ports_StaticMemory {
      framer
      deframer
      deframing
      hub
      hubFramer
      hubDeframer
      hubCommDriver
      pduDownlink
      pduUplink
    }

  topology TeensyFSW {

    # ----------------------------------------------------------------------
    # Instances used in the topology
    # ----------------------------------------------------------------------

    instance cmdDisp
    instance commQueue
    instance deframer
    instance eventLogger
    instance fatalAdapter
    instance fatalHandler
    instance framer
    instance rateDriver
    instance rateGroup1
    instance rateGroup2
    instance rateGroupDriver
    instance staticMemory
    instance systemResources
    instance textLogger
    instance timeHandler
    instance tlmSend

    # LedBlinker
    instance blinker
    instance gpioLed

    # Radios
    instance rfm23
    instance gpioRxOn
    instance gpioTxOn

    # Sensors
    instance imu
    instance mag
    instance solar_panel_1
    instance solar_panel_2
    instance solar_panel_3
    instance solar_panel_4
    instance battery_board
    
    # PDU
    instance pdu
    instance pduCommDriver
    instance rpiGpioEnable

    # Hub
    instance cmdSplitter
    instance hub
    instance hubCommDriver
    instance hubFramer
    instance hubDeframer

    # Comm Driver
    instance commDriver
    instance commStub

    # ----------------------------------------------------------------------
    # Pattern graph specifiers
    # ----------------------------------------------------------------------

    command connections instance cmdDisp

    event connections instance eventLogger

    telemetry connections instance tlmSend

    text event connections instance textLogger

    time connections instance timeHandler

    # ----------------------------------------------------------------------
    # Direct graph specifiers
    # ----------------------------------------------------------------------

    connections RateGroups {
      # Block driver
      rateDriver.CycleOut -> rateGroupDriver.CycleIn

      # Rate group 1
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup1] -> rateGroup1.CycleIn
      # rateGroup1.RateGroupMemberOut[0] -> rfm23.run
      rateGroup1.RateGroupMemberOut[0] -> commDriver.schedIn
      rateGroup1.RateGroupMemberOut[1] -> blinker.run
      rateGroup1.RateGroupMemberOut[2] -> hubCommDriver.schedIn
      rateGroup1.RateGroupMemberOut[3] -> pduCommDriver.schedIn

      # Rate group 2
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup2] -> rateGroup2.CycleIn
      rateGroup2.RateGroupMemberOut[0] -> systemResources.run
      rateGroup2.RateGroupMemberOut[1] -> tlmSend.Run
      rateGroup2.RateGroupMemberOut[2] -> imu.run
      rateGroup2.RateGroupMemberOut[3] -> mag.run
      rateGroup2.RateGroupMemberOut[4] -> solar_panel_1.run
      rateGroup2.RateGroupMemberOut[5] -> solar_panel_2.run
      rateGroup2.RateGroupMemberOut[6] -> solar_panel_3.run
      rateGroup2.RateGroupMemberOut[7] -> solar_panel_4.run
      rateGroup2.RateGroupMemberOut[8] -> battery_board.run
    }

    connections FaultProtection {
      eventLogger.FatalAnnounce -> fatalHandler.FatalReceive
    }

    connections Downlink {

      tlmSend.PktSend -> commQueue.comQueueIn[0]
      eventLogger.PktSend -> commQueue.comQueueIn[1]

      # tlmSend.PktSend -> framer.comIn
      # eventLogger.PktSend -> framer.comIn

      commQueue.comQueueSend -> framer.comIn

      framer.framedAllocate -> staticMemory.bufferAllocate[Ports_StaticMemory.framer]
      # framer.framedOut -> rfm23.comDataIn
      # rfm23.deallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.framer]
      # rfm23.comStatus -> commQueue.comStatusIn
      framer.framedOut -> commStub.comDataIn
      commDriver.deallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.framer]
      commStub.drvDataOut -> commDriver.send
      commDriver.ready -> commStub.drvConnected
      commStub.comStatus -> commQueue.comStatusIn

    }

    connections Uplink {

      # rfm23.allocate -> staticMemory.bufferAllocate[Ports_StaticMemory.deframer]
      # rfm23.comDataOut -> deframer.framedIn
      commDriver.allocate -> staticMemory.bufferAllocate[Ports_StaticMemory.deframer]
      commDriver.$recv -> commStub.drvDataIn
      commStub.comDataOut -> deframer.framedIn
      deframer.framedDeallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.deframer]

      deframer.comOut -> cmdSplitter.CmdBuff
      cmdSplitter.LocalCmd -> cmdDisp.seqCmdBuff
      cmdDisp.seqCmdStatus -> cmdSplitter.seqCmdStatus
      cmdSplitter.forwardSeqCmdStatus -> deframer.cmdResponseIn

      deframer.bufferAllocate -> staticMemory.bufferAllocate[Ports_StaticMemory.deframing]
      deframer.bufferDeallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.deframing]
      
    }

    connections HubToDriver {
      # Hub -> Framer -> Uart Driver
      hub.dataOutAllocate -> staticMemory.bufferAllocate[Ports_StaticMemory.hub]
      hub.dataOut -> hubFramer.bufferIn
      hubFramer.bufferDeallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.hub]
      hubFramer.framedAllocate -> staticMemory.bufferAllocate[Ports_StaticMemory.hubFramer]
      hubFramer.framedOut -> hubCommDriver.send
      hubCommDriver.deallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.hubFramer]

      # Uart Driver -> Deframer -> Hub
      hubCommDriver.allocate -> staticMemory.bufferAllocate[Ports_StaticMemory.hubCommDriver]
      hubCommDriver.$recv -> hubDeframer.framedIn
      hubDeframer.framedDeallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.hubCommDriver]
      hubDeframer.bufferAllocate -> staticMemory.bufferAllocate[Ports_StaticMemory.hubDeframer]
      hubDeframer.bufferOut -> hub.dataIn
      hub.dataInDeallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.hubDeframer]
    }


    connections HubToDeployment {
      hub.LogSend -> eventLogger.LogRecv
      hub.TlmSend -> tlmSend.TlmRecv

      cmdSplitter.RemoteCmd -> hub.portIn[0]
      hub.portOut[0] -> cmdSplitter.seqCmdStatus

    }

    connections LedBlinker {
      blinker.gpioSet -> gpioLed.gpioWrite
    }

    connections Radio {
      # rfm23.gpioSetRxOn -> gpioRxOn.gpioWrite
      # rfm23.gpioSetTxOn -> gpioTxOn.gpioWrite
    }

    connections PDU {
      pduCommDriver.allocate -> staticMemory.bufferAllocate[Ports_StaticMemory.pduDownlink]
      pduCommDriver.$recv -> pdu.comDataIn  
      pdu.deallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.pduDownlink]

      pdu.allocate -> staticMemory.bufferAllocate[Ports_StaticMemory.pduUplink]
      pdu.comDataOut -> pduCommDriver.send
      pduCommDriver.deallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.pduUplink]

      pdu.rpiGpioSet -> rpiGpioEnable.gpioWrite
      pdu.rpiGpioRead -> rpiGpioEnable.gpioRead
    }

  }

}
