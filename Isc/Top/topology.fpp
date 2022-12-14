module Isc {

  # ----------------------------------------------------------------------
  # Symbolic constants for port numbers
  # ----------------------------------------------------------------------

    enum Ports_RateGroups {
      rateGroup1
    }

    enum Ports_StaticMemory {
      downlink
      uplink
    }

  topology Isc {

    # ----------------------------------------------------------------------
    # Instances used in the topology
    # ----------------------------------------------------------------------

    instance rateDriver
    instance blinker
    instance ledPin
    instance tlmSend
    instance cmdDisp
    instance cmdSeq
    instance comm
    instance commDriver
    instance commQueue
    instance framer
    instance eventLogger
    instance fatalAdapter
    instance fatalHandler
    instance fileDownlink
    instance fileManager
    instance fileUplink
    instance fileUplinkBufferManager
    instance systemTime
    instance rateGroup1Comp
    instance rateGroupDriverComp
    instance staticMemory
    instance textLogger
    instance deframer
    instance systemResources

    # ----------------------------------------------------------------------
    # Pattern graph specifiers
    # ----------------------------------------------------------------------

    command connections instance cmdDisp

    event connections instance eventLogger

    telemetry connections instance tlmSend

    text event connections instance textLogger

    time connections instance systemTime

    # ----------------------------------------------------------------------
    # Direct graph specifiers
    # ----------------------------------------------------------------------

    connections Blinker {
      blinker.gpio -> ledPin.gpioWrite
    }

    connections Downlink {

      tlmSend.PktSend -> commQueue.comQueueIn[0]
      eventLogger.PktSend -> commQueue.comQueueIn[1]
      fileDownlink.bufferSendOut -> commQueue.buffQueueIn[0]

      commQueue.comQueueSend -> framer.comIn
      commQueue.buffQueueSend -> framer.bufferIn

      framer.framedAllocate -> staticMemory.bufferAllocate[0]
      framer.framedOut -> comm.comDataIn
      framer.bufferDeallocate -> fileDownlink.bufferReturn

      commDriver.deallocate -> staticMemory.bufferDeallocate[0]
      comm.drvDataOut -> commDriver.send

      comm.comStatus -> commQueue.comStatusIn
    }

    connections FaultProtection {
      eventLogger.FatalAnnounce -> fatalHandler.FatalReceive
    }

    connections RateGroups {
      rateDriver.CycleOut -> rateGroupDriverComp.CycleIn
      #rateDriver.error -> ledPin.gpioWrite

      # Rate group 1
      rateGroupDriverComp.CycleOut[Ports_RateGroups.rateGroup1] -> rateGroup1Comp.CycleIn
      rateGroup1Comp.RateGroupMemberOut[0] -> commDriver.schedIn
      rateGroup1Comp.RateGroupMemberOut[1] -> blinker.run
      rateGroup1Comp.RateGroupMemberOut[2] -> tlmSend.Run
      rateGroup1Comp.RateGroupMemberOut[3] -> fileDownlink.Run
      rateGroup1Comp.RateGroupMemberOut[4] -> systemResources.run
      rateGroup1Comp.RateGroupMemberOut[5] -> cmdSeq.schedIn
      rateGroup1Comp.RateGroupMemberOut[6] -> fileUplinkBufferManager.schedIn

    }

    connections Sequencer {
      cmdSeq.comCmdOut -> cmdDisp.seqCmdBuff
      cmdDisp.seqCmdStatus -> cmdSeq.cmdResponseIn
    }

    connections Uplink {

      commDriver.allocate -> staticMemory.bufferAllocate[1]
      commDriver.$recv -> comm.drvDataIn
      comm.comDataOut -> deframer.framedIn
      deframer.framedDeallocate -> staticMemory.bufferDeallocate[1]

      deframer.comOut -> cmdDisp.seqCmdBuff
      cmdDisp.seqCmdStatus -> deframer.cmdResponseIn

      deframer.bufferAllocate -> fileUplinkBufferManager.bufferGetCallee
      deframer.bufferOut -> fileUplink.bufferSendIn
      deframer.bufferDeallocate -> fileUplinkBufferManager.bufferSendIn
      fileUplink.bufferSendOut -> fileUplinkBufferManager.bufferSendIn

      commDriver.ready -> comm.drvConnected

     }
  }

}
