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
      isInitialized(false),
      radio_state(Fw::On::OFF),
      pkt_rx_count(0),
      pkt_tx_count(0) {}

RFM23::~RFM23() {}

void RFM23 ::recv() {
    this->gpioSetRxOn_out(0, Fw::Logic::LOW);
    this->gpioSetTxOn_out(0, Fw::Logic::HIGH);

    if (this->radio_state == Fw::On::OFF) {
        return;
    }

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

    if (this->radio_state == Fw::On::OFF) {
        return false;
    }

    NATIVE_UINT_TYPE offset = 0;
    while (len > RH_RF22_MAX_MESSAGE_LEN)

    {
        if (!rfm23.send(&payload[offset], RH_RF22_MAX_MESSAGE_LEN)) {
            return false;
        }
        // if (!rfm23.waitPacketSent(1000))
        // {
        //     return false;
        // }
        delay(1000);
        rfm23.sleep();
        offset += RH_RF22_MAX_MESSAGE_LEN;
        len    -= RH_RF22_MAX_MESSAGE_LEN;
    }

    if (!rfm23.send(&payload[offset], len)) {
        return false;
    }
    // if (!rfm23.waitPacketSent(1000))
    // {
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
    if ((this->radio_state == Fw::On::ON) && (sendBuffer.getSize() > 0) &&
        (not this->send(sendBuffer.getData(), sendBuffer.getSize()))) {
        this->radio_state = Fw::On::OFF;
    }
    deallocate_out(0, sendBuffer);
    return Drv::SendStatus::SEND_OK;  // Always send ok to deframer as it does not handle this
                                      // anyway
}

void RFM23::run_handler(const NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    if (this->isInitialized == false) {
        // rfm23.reset();
        if (!rfm23.init()) {
            Fw::Logger::logMsg("radio init failed \n");
            return;
        }
        rfm23.setFrequency(RFM23_FREQ);
        rfm23.setTxPower(RH_RF22_RF23BP_TXPOW_30DBM);
        rfm23.sleep();
        rfm23.setModemConfig(RH_RF22::GFSK_Rb2Fd5);
        rfm23.sleep();
        // rfm23.setModeIdle();
        Fw::Success radioSuccess = Fw::Success::SUCCESS;
        if (this->isConnected_comStatus_OutputPort(0)) {
            this->comStatus_out(0, radioSuccess);
        }
        Fw::Logger::logMsg("radio init success \n");
        this->isInitialized = true;
        this->radio_state   = Fw::On::ON;
    } else if (this->radio_state == Fw::On::ON) {
        this->recv();
        this->tlmWrite_temp(rfm23.temperatureRead());
    }
    this->tlmWrite_Status(this->radio_state);
}

void RFM23::healthCheck_handler(const NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    if (this->isConnected_getOpMode_OutputPort(0)) {
        Components::OpModes opMode;
        this->getOpMode_out(0, opMode);
        if (opMode == Components::OpModes::Startup || opMode == Components::OpModes::Deployment) {
            this->offTime = 0;
            return;
        }
    }

    Components::PDUTlm states;
    this->PDUGetSwitch_out(0, states);
    if (states[1].getstate() == 0 && this->radio_state == Fw::On::ON) {
        this->radio_state = Fw::On::OFF;
        this->offTime     = 0;
    }
    if (this->radio_state == Fw::On::OFF && this->offTime > 30000) {
        this->log_WARNING_HI_RadioReset(30);
        this->PDUSetSwitch_out(0, Components::PDU_SW::RFM23_RADIO, Fw::On::ON);
        this->offTime = 0;
        this->isInitialized = false;
    }
}

}  // end namespace Radios
