// ======================================================================
// \title  RFM23.hpp
// \author luke
// \brief  hpp file for RFM23 component implementation class
// ======================================================================

#ifndef RFM23_HPP
#define RFM23_HPP

#include <RHHardwareSPI1.h>
#include <RH_RF22.h>
#include <elapsedMillis.h>
#include <FprimeArduino.hpp>
#include "Components/Radios/RFM23/RFM23ComponentAc.hpp"

namespace Radios {

class RFM23 : public RFM23ComponentBase {
  public:
    static const NATIVE_INT_TYPE RFM23_FREQ = 433;
    static const NATIVE_INT_TYPE RFM23_CS   = 38;
    static const NATIVE_INT_TYPE RFM23_INT  = 40;
    static const NATIVE_INT_TYPE SPI_MISO   = 39;
    static const NATIVE_INT_TYPE SPI_MOSI   = 26;
    static const NATIVE_INT_TYPE SPI_SCK    = 27;
    static const NATIVE_INT_TYPE TX_ON      = 31;
    static const NATIVE_INT_TYPE RX_ON      = 30;

    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object RFM23
    //!
    RFM23(const char* const compName /*!< The component name*/
    );

    //! Destroy object RFM23
    //!
    ~RFM23();

    bool send(const U8* payload, NATIVE_UINT_TYPE len);
    void recv();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for comDataIn
    //!
    Drv::SendStatus comDataIn_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                                      Fw::Buffer& sendBuffer);

    //! Handler implementation for run
    //!
    void run_handler(const NATIVE_INT_TYPE portNum, /*!< The port number */
                     NATIVE_UINT_TYPE context       /*!< The call order */
    );

    //! Handler implementation for health_check
    //!
    void healthCheck_handler(const NATIVE_INT_TYPE portNum, /*!< The port number */
                             NATIVE_UINT_TYPE context       /*!< The call order */
    );

    RH_RF22 rfm23;
    bool isInitialized;
    Fw::On radio_state;
    Fw::On prev_radio_state;
    U16 pkt_rx_count;
    U16 pkt_tx_count;
    elapsedMillis offTime;
};

}  // end namespace Radios

#endif
