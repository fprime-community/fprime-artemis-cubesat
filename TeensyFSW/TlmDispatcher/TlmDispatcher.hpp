// ======================================================================
// \title  TlmDispatcher.hpp
// \author root
// \brief  hpp file for TlmDispatcher component implementation class
// ======================================================================

#ifndef Components_TlmDispatcher_HPP
#define Components_TlmDispatcher_HPP

#include "TeensyFSW/TlmDispatcher/TlmDispatcherComponentAc.hpp"

namespace Components {

class TlmDispatcher : public TlmDispatcherComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct TlmDispatcher object
    TlmDispatcher(const char* const compName  //!< The component name
    );

    //! Destroy TlmDispatcher object
    ~TlmDispatcher();

    PRIVATE :

        // ----------------------------------------------------------------------
        // Handler implementations for user-defined typed input ports
        // ----------------------------------------------------------------------

        //! Handler implementation for comIn
        //!
        //! Port to
        void
        comIn_handler(NATIVE_INT_TYPE portNum,  //!< The port number
                      Fw::ComBuffer& data,      //!< Buffer containing packet data
                      U32 context               //!< Call context value; meaning chosen by user
        );
};

}  // namespace Components

#endif
