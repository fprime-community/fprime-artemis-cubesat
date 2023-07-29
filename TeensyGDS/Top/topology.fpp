module TeensyGDS {

  # ----------------------------------------------------------------------
  # Symbolic constants for port numbers
  # ----------------------------------------------------------------------

    enum Ports_RateGroups {
      rateGroup1
      rateGroup2
    }

    enum Ports_StaticMemory {
      downlink
      uplink
    }

  topology TeensyGDS {

    # ----------------------------------------------------------------------
    # Instances used in the topology
    # ----------------------------------------------------------------------

    instance commDriver
    instance eventLogger
    instance rateDriver
    instance rateGroup1
    instance rateGroup2
    instance rateGroupDriver
    instance staticMemory
    instance streamCrossoverUplink
    instance streamCrossoverDownlink
    instance timeHandler

    # LedBlinker
    instance blinker
    instance gpioLed

    # Radios
    instance rfm23
    instance gpioRxOn
    instance gpioTxOn

    # ----------------------------------------------------------------------
    # Pattern graph specifiers
    # ----------------------------------------------------------------------

    event connections instance eventLogger

    time connections instance timeHandler

    # ----------------------------------------------------------------------
    # Direct graph specifiers
    # ----------------------------------------------------------------------

    connections RateGroups {
      # Block driver
      rateDriver.CycleOut -> rateGroupDriver.CycleIn

      # Rate group 1
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup1] -> rateGroup1.CycleIn
      rateGroup1.RateGroupMemberOut[0] -> commDriver.schedIn
      rateGroup1.RateGroupMemberOut[1] -> rfm23.run

      # Rate group 2
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup2] -> rateGroup2.CycleIn
      rateGroup2.RateGroupMemberOut[0] -> blinker.run
    }

    connections Downlink {

      rfm23.allocate -> staticMemory.bufferAllocate[Ports_StaticMemory.downlink]
      rfm23.comDataOut -> streamCrossoverDownlink.streamIn
      streamCrossoverDownlink.streamOut -> commDriver.send
      commDriver.deallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.downlink]

      streamCrossoverDownlink.errorDeallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.downlink]

    }
    
    connections Uplink {

      commDriver.allocate -> staticMemory.bufferAllocate[Ports_StaticMemory.uplink]
      commDriver.$recv -> streamCrossoverUplink.streamIn
      streamCrossoverUplink.streamOut -> rfm23.comDataIn
      rfm23.deallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.uplink]
      
      streamCrossoverUplink.errorDeallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.uplink]
      
    }

    connections LedBlinker {
      blinker.gpioSet -> gpioLed.gpioWrite
    }

    connections Radio {
      rfm23.gpioSetRxOn -> gpioRxOn.gpioWrite
      rfm23.gpioSetTxOn -> gpioTxOn.gpioWrite
    }

  }

}
