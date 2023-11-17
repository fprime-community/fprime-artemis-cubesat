module RpiFSW {

  # ----------------------------------------------------------------------
  # Symbolic constants for port numbers
  # ----------------------------------------------------------------------

  enum Ports_RateGroups {
    rateGroup1
    rateGroup2
    rateGroup3
  }

  topology RpiFSW {

    # ----------------------------------------------------------------------
    # Instances used in the topology
    # ----------------------------------------------------------------------

    instance rpi_health
    instance rpi_blockDrv
    instance rpi_camera
    instance rpi_bufferManager
    instance rpi_fatalAdapter
    instance rpi_fatalHandler
    instance rpi_fileDownlink
    instance rpi_fileManager
    instance rpi_linuxTime
    instance rpi_prmDb
    instance rpi_rateGroup1
    instance rpi_rateGroup2
    instance rpi_rateGroup3
    instance rpi_rateGroupDriver
    instance rpi_textLogger
    instance rpi_saveImageBufferLogger
    instance rpi_systemResources
    instance rpi_cmdDisp

    instance rpi_hub
    instance rpi_hubDeframer
    instance rpi_hubDriver
    instance rpi_hubFramer

    # ----------------------------------------------------------------------
    # Pattern graph specifiers
    # ----------------------------------------------------------------------

    param connections instance rpi_prmDb

    text event connections instance rpi_textLogger

    event connections instance rpi_hub

    telemetry connections instance rpi_hub

    time connections instance rpi_linuxTime

    health connections instance rpi_health

    command connections instance rpi_cmdDisp

    # ----------------------------------------------------------------------
    # Direct graph specifiers
    # ----------------------------------------------------------------------

    connections RateGroups {
      # Block driver
      rpi_blockDrv.CycleOut -> rpi_rateGroupDriver.CycleIn

      # Rate group 1
      rpi_rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup1] -> rpi_rateGroup1.CycleIn
      rpi_rateGroup1.RateGroupMemberOut[0] -> rpi_fileDownlink.Run

      # Rate group 2
      rpi_rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup2] -> rpi_rateGroup2.CycleIn
      rpi_rateGroup2.RateGroupMemberOut[0] -> rpi_bufferManager.schedIn
 
      # Rate group 3
      rpi_rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup3] -> rpi_rateGroup3.CycleIn
      rpi_rateGroup3.RateGroupMemberOut[0] -> rpi_health.Run
      rpi_rateGroup3.RateGroupMemberOut[1] -> rpi_blockDrv.Sched
      rpi_rateGroup3.RateGroupMemberOut[2] -> rpi_systemResources.run
    }

    connections Camera {
         rpi_camera.allocate -> rpi_bufferManager.bufferGetCallee
         rpi_camera.deallocate -> rpi_bufferManager.bufferSendIn
         rpi_camera.$save -> rpi_saveImageBufferLogger.bufferSendIn
         rpi_saveImageBufferLogger.bufferSendOut -> rpi_bufferManager.bufferSendIn
    }

    connections HubToDriver {
      # Hub -> Framer -> Uart Driver
      rpi_hub.dataOutAllocate -> rpi_bufferManager.bufferGetCallee
      rpi_hub.dataOut -> rpi_hubFramer.bufferIn
      rpi_hubFramer.bufferDeallocate -> rpi_bufferManager.bufferSendIn
      rpi_hubFramer.framedAllocate -> rpi_bufferManager.bufferGetCallee
      rpi_hubFramer.framedOut -> rpi_hubDriver.send
      rpi_hubDriver.deallocate -> rpi_bufferManager.bufferSendIn

      # Uart Driver -> Deframer -> Hub
      rpi_hubDriver.allocate -> rpi_bufferManager.bufferGetCallee
      rpi_hubDriver.$recv -> rpi_hubDeframer.framedIn
      rpi_hubDeframer.framedDeallocate -> rpi_bufferManager.bufferSendIn
      rpi_hubDeframer.bufferAllocate -> rpi_bufferManager.bufferGetCallee
      rpi_hubDeframer.bufferOut -> rpi_hub.dataIn
      rpi_hub.dataInDeallocate -> rpi_bufferManager.bufferSendIn
    }

    connections HubToDeployment {
      rpi_hub.portOut[0] -> rpi_cmdDisp.seqCmdBuff
      rpi_cmdDisp.seqCmdStatus -> rpi_hub.portIn[0]

      rpi_fileDownlink.bufferSendOut -> rpi_hub.portIn[1]
      rpi_hub.portOut[1] -> rpi_fileDownlink.bufferReturn
    }

  }

}