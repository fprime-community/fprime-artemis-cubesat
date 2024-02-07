// ======================================================================
// \title  ModeManager.hpp
// \author root
// \brief  hpp file for ModeManager component implementation class
// ======================================================================

#ifndef TeensyFSW_ModeManager_HPP
#define TeensyFSW_ModeManager_HPP

#include <elapsedMillis.h>
#include <FprimeArduino.hpp>
#include "TeensyFSW/ModeManager/ModeManagerComponentAc.hpp"

namespace Components {

class ModeManager : public ModeManagerComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct ModeManager object
    ModeManager(const char* const compName  //!< The component name
    );

    //! Destroy ModeManager object
    ~ModeManager();

  private:
    Components::OpModes currentMode;

    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for getOpMode
    void getOpMode_handler(NATIVE_INT_TYPE portNum,  //!< The port number
                           Components::OpModes& mode);

    //! Handler implementation for run
    //!
    //! Port: receiving calls from the rate group
    void run_handler(NATIVE_INT_TYPE portNum,  //!< The port number
                     NATIVE_UINT_TYPE context  //!< The call order
    );

    //! Handler implementation for run
    //!
    //! Port: receiving calls from the rate group
    void tlmSend_handler(NATIVE_INT_TYPE portNum,  //!< The port number
                         NATIVE_UINT_TYPE context  //!< The call order
    );

    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command SetMode
    void SetMode_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                            U32 cmdSeq,           //!< The command sequence number
                            Components::OpModes mode);

    bool started;
    Components::OpModes prevOpMode;

    elapsedMillis timeout;
};

}  // namespace Components

#endif
