// ======================================================================
// \title  RFM23.cpp
// \author luke
// \brief  cpp file for RFM23 component implementation class
// ======================================================================

#include <Components/Radios/RFM23/RFM23.hpp>
#include <FpConfig.hpp>
#include <Fw/Logger/Logger.hpp>

namespace Radios {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

RFM23::RFM23(const char* const compName)
    : RFM23ComponentBase(compName),
      rfm23(RFM23_CS, RFM23_INT, hardware_spi1),
      radio_state(Fw::On::OFF),
      pkt_rx_count(0),
      pkt_tx_count(0) {}

RFM23::~RFM23() {}

void RFM23 ::recv() {
    this->gpioSetRxOn_out(0, Fw::Logic::LOW);
    this->gpioSetTxOn_out(0, Fw::Logic::HIGH);

    if (rfm23.available()) {
        U8 buf[RH_RF22_MAX_MESSAGE_LEN];
        U8 bytes_recv = RH_RF22_MAX_MESSAGE_LEN;

        if (rfm23.recv(buf, &bytes_recv)) {
            Fw::Buffer recvBuffer = this->allocate_out(0, bytes_recv);
            memcpy(recvBuffer.getData(), buf, bytes_recv);
            recvBuffer.setSize(bytes_recv);
            pkt_rx_count++;
            this->log_DIAGNOSTIC_PayloadMessageRX(recvBuffer.getSize());

            this->tlmWrite_NumPacketsReceived(pkt_rx_count);
            this->tlmWrite_RSSI(rfm23.lastRssi());

            this->comDataOut_out(0, recvBuffer, Drv::RecvStatus::RECV_OK);
        }
    }
}

bool RFM23::send(const U8* payload, NATIVE_UINT_TYPE len) {
    FW_ASSERT(payload != nullptr);
    this->gpioSetRxOn_out(0, Fw::Logic::HIGH);
    this->gpioSetTxOn_out(0, Fw::Logic::LOW);

    NATIVE_UINT_TYPE offset = 0;
    while (len > RH_RF22_MAX_MESSAGE_LEN)

    {
        if (!rfm23.send(&payload[offset], RH_RF22_MAX_MESSAGE_LEN)) {
            Fw::Logger::logMsg("1");
            return false;
        }
        // if (!rfm23.waitPacketSent(1000))
        // {
        //     Fw::Logger::logMsg("2");
        //     return false;
        // }
        delay(1000);
        rfm23.sleep();
        offset += RH_RF22_MAX_MESSAGE_LEN;
        len    -= RH_RF22_MAX_MESSAGE_LEN;
    }

    if (!rfm23.send(&payload[offset], len)) {
        Fw::Logger::logMsg("3");
        return false;
    }
    // if (!rfm23.waitPacketSent(1000))
    // {
    //     Fw::Logger::logMsg("4");
    //     return false;
    // }
    delay(1000);
    rfm23.sleep();
    pkt_tx_count++;
    this->log_DIAGNOSTIC_PayloadMessageTX(len);
    this->tlmWrite_NumPacketsSent(pkt_tx_count);

    Fw::Success radioSuccess = Fw::Success::SUCCESS;
    if (this->isConnected_comStatus_OutputPort(0)) {
        this->comStatus_out(0, radioSuccess);
    }

    return true;
}
// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

Drv::SendStatus RFM23::comDataIn_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& sendBuffer) {
    if ((radio_state == Fw::On::ON) && (sendBuffer.getSize() > 0) &&
        (not this->send(sendBuffer.getData(), sendBuffer.getSize()))) {
        radio_state = Fw::On::OFF;
    }
    deallocate_out(0, sendBuffer);
    return Drv::SendStatus::SEND_OK;  // Always send ok to deframer as it does not handle this
                                      // anyway
}

void RFM23::run_handler(const NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    if (radio_state == Fw::On::OFF) {
        // rfm23.reset();
        if (!rfm23.init()) {
            Fw::Logger::logMsg("radio init failed \n");
            return;
        }
        rfm23.setFrequency(RFM23_FREQ);
        rfm23.setTxPower(RH_RF22_RF23BP_TXPOW_30DBM);
        rfm23.sleep();
        rfm23.setModemConfig(RH_RF22::FSK_Rb2Fd5);
        rfm23.sleep();
        // rfm23.setModeIdle();
        Fw::Success radioSuccess = Fw::Success::SUCCESS;
        if (this->isConnected_comStatus_OutputPort(0)) {
            this->comStatus_out(0, radioSuccess);
        }
        Fw::Logger::logMsg("radio init success \n");
        radio_state = Fw::On::ON;
    }
    this->recv();
    this->tlmWrite_Status(radio_state);
    this->tlmWrite_temp(rfm23.temperatureRead());
}

}  // end namespace Radios
