// ======================================================================
// \title  TeensyFSWTopologyDefs.hpp
// \brief required header file containing the required definitions for the
// topology autocoder
//
// ======================================================================
#ifndef TEENSYFSW_TEENSYFSWTOPOLOGYDEFS_HPP
#define TEENSYFSW_TEENSYFSWTOPOLOGYDEFS_HPP

#include "Fw/Types/MallocAllocator.hpp"
#include "Svc/FramingProtocol/FprimeProtocol.hpp"
#include "TeensyFSW/Top/FppConstantsAc.hpp"

// Definitions are placed within a namespace named after the deployment
namespace TeensyFSW {

/**
 * \brief required type definition to carry state
 *
 * The topology autocoder requires an object that carries state with the name
 * `TeensyFSW::TopologyState`. Only the type definition is required by the
 * autocoder and the contents of this object are otherwise opaque to the
 * autocoder. The contents are entirely up to the definition of the project.
 * This reference application specifies hostname and port fields, which are
 * derived by command line inputs.
 */
struct TopologyState {
    FwIndexType uartNumber;
    PlatformIntType uartBaud;
};

enum ARTEMIS_TEENSY_PINOUT  // Artemis OBC v4.24
{
    UART4_RXD   = 0,  /**< Physical 0: Serial1 Receive */
    UART4_TXD   = 1,  /**< Physical 1: Serial1 Transmit */
    T_GPIO2     = 2,  /**< Physical 2: */
    T_GPIO3     = 3,  /**< Physical 3: */
    T_GPIO4     = 4,  /**< Physical 4: */
    T_GPIO5     = 5,  /**< Physical 5: */
    T_GPIO6     = 6,  /**< Physical 6: */
    UART6_RX    = 7,  /**< Physical 7: Serial2 Receive */
    UART6_TX    = 8,  /**< Physical 8: Serial2 Transmit */
    T_CS1       = 9,  /**< Physical 9: */
    T_CS        = 10, /**< Physical 10: */
    SPI0_MOSI   = 11, /**< Physical 11: SPI MOSI */
    SPI0_MISO   = 12, /**< Physical 12: SPI MISO */
    SPI0_SCLK   = 13, /**< Physical 13: SPI Clock */
    AIN0        = 14, /**< Physical 14: Analog A0 */
    AIN1        = 15, /**< Physical 15: Analog A1 */
    SCL1_I2C    = 16, /**< Physical 16: I2C Wire1 SCL */
    SDA1_I2C    = 17, /**< Physical 17: I2C Wire1 SDA */
    I2C2_SDA    = 18, /**< Physical 18: I2C Wire SDA */
    I2C2_SCL    = 19, /**< Physical 19: I2C Wire SCL */
    AIN3        = 20, /**< Physical 20: Analog A6 */
    AIN4        = 21, /**< Physical 21: Analog A7 */
    AIN5        = 22, /**< Physical 22: Analog A8 */
    AIN6        = 23, /**< Physical 23: Analog A9 */
    I2C1_SCL    = 24, /**< Physical 24: I2C Wire2 SCL */
    I2C1_SDA    = 25, /**< Physical 25: I2C Wire2 SDA */
    SPI1_D1     = 26, /**< Physical 26: SPI1 MOSI */
    SPI1_SCLK   = 27, /**< Physical 27: SPI1 Clock */
    UART5_TXD   = 28, /**< Physical 28: Serial7 Transmit */
    UART5_RXD   = 29, /**< Physical 29: Serial7 Receive */
    RX_ON       = 30, /**< Physical 30: RFM23 RX_ON Pin */
    TX_ON       = 31, /**< Physical 31: RFM23 TX_ON Pin */
    RADIO_RESET = 32, /**< Physical 32: Radio Hardware Reset */
    GPS_RSTN    = 33, /**< Physical 33: */
    UART2_RXD   = 34, /**< Physical 34: Serial8 Receive */
    UART2_TXD   = 35, /**< Physical 35: Serial8 Transmit */
    RPI_ENABLE  = 36, /**< Physical 36: Switch to turn on RPi */
    SDN         = 37, /**< Physical 37: */
    SPI1_CS1    = 38, /**< Physical 38: SPI1 Chip Select */
    SPI1_D0     = 39, /**< Physical 39: SPI1 MISO */
    NIRQ        = 40, /**< Physical 40: Radio Interrupt */
    AIN2        = 41  /**< Physical 41: Analog A17 */
};

/**
 * \brief required ping constants
 *
 * The topology autocoder requires a WARN and FATAL constant definition for
 * each component that supports the health-ping interface. These are expressed
 * as enum constants placed in a namespace named for the component instance.
 * These are all placed in the PingEntries namespace.
 *
 * Each constant specifies how many missed pings are allowed before a
 * WARNING_HI/FATAL event is triggered. In the following example, the health
 * component will emit a WARNING_HI event if the component instance cmdDisp
 * does not respond for 3 pings and will FATAL if responses are not received
 * after a total of 5 pings.
 *
 * ```c++
 * namespace PingEntries {
 * namespace cmdDisp {
 *     enum { WARN = 3, FATAL = 5 };
 * }
 * }
 * ```
 */
namespace PingEntries {
namespace tlmSend {
enum { WARN = 3, FATAL = 5 };
}
namespace cmdDisp {
enum { WARN = 3, FATAL = 5 };
}
namespace eventLogger {
enum { WARN = 3, FATAL = 5 };
}
namespace rateGroup1 {
enum { WARN = 3, FATAL = 5 };
}
namespace rateGroup2 {
enum { WARN = 3, FATAL = 5 };
}
}  // namespace PingEntries
}  // namespace TeensyFSW
#endif
