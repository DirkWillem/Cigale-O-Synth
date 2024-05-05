#pragma once

#include <cassert>

#include <stm32g0xx_hal.h>

#include <hal/peripheral.h>
#include <hal/uart.h>

#include <stm32g0/mappings/uart_pin_mapping.h>
#include <stm32g0/pin.h>

namespace stm32g0 {

namespace detail {

template <UartId Id, hal::UartFlowControl FC>
struct UartPinoutHelper;

template <UartId Id>
struct UartPinoutHelper<Id, hal::UartFlowControl::None> {
  struct Pinout {
    consteval Pinout(PinId tx, PinId rx,
                     hal::PinPull pull_tx = hal::PinPull::NoPull,
                     hal::PinPull pull_rx = hal::PinPull::NoPull) noexcept
        : tx{tx}
        , rx{rx}
        , pull_tx{pull_tx}
        , pull_rx{pull_rx} {
      assert(("TX pin must be valid",
              hal::FindPinAFMapping(UartTxPinMappings, Id, tx).has_value()));
      assert(("RX pin must be valid",
              hal::FindPinAFMapping(UartRxPinMappings, Id, rx).has_value()));
    }

    PinId tx;
    PinId rx;

    hal::PinPull pull_tx;
    hal::PinPull pull_rx;
  };
};

void SetupUartNoFc(UartId id, UART_HandleTypeDef& huart, unsigned baud,
                   hal::UartParity   parity,
                   hal::UartStopBits stop_bits) noexcept;

void InitializeUartForPollMode(UART_HandleTypeDef& huart) noexcept;

}   // namespace detail

template <typename Impl, UartId Id,
          hal::UartFlowControl   FC = hal::UartFlowControl::None,
          hal::UartOperatingMode OM = hal::UartOperatingMode::Poll>
/**
 * Implementation for UART
 * @tparam Id UART Id
 * @tparam FC UART Flow Control
 * @tparam OM UART Operating Mode
 */
class UartImpl : public hal::UsedPeripheral {
 public:
  using Pinout = detail::UartPinoutHelper<Id, FC>::Pinout;

  /**
   * Writes a string to the UART
   * @param sv View of the string to write
   */
  void Write(std::string_view sv)
    requires(OM == hal::UartOperatingMode::Poll)
  {
    HAL_UART_Transmit(&huart, reinterpret_cast<const uint8_t*>(sv.data()),
                      sv.size(), 500);
  }

  /**
   * Singleton constructor
   * @return Singleton instance
   */
  [[nodiscard]] static Impl& instance() noexcept {
    static Impl inst{};
    return inst;
  }

 protected:
  /**
   * Constructor
   * @param pinout UART pinout
   * @param baud UART baud rate
   * @param parity UART parity
   * @param stop_bits UART stop bits
   */
  UartImpl(Pinout pinout, unsigned baud,
           hal::UartParity   parity    = hal::UartParity::None,
           hal::UartStopBits stop_bits = hal::UartStopBits::One)
    requires(FC == hal::UartFlowControl::None)
      : huart{} {
    // Set up tx and rx pins
    Pin::InitializeAlternate(
        pinout.tx, hal::FindPinAFMapping(UartTxPinMappings, Id, pinout.tx)->af,
        pinout.pull_tx);
    Pin::InitializeAlternate(
        pinout.rx, hal::FindPinAFMapping(UartRxPinMappings, Id, pinout.rx)->af,
        pinout.pull_rx);

    // Initialize UART
    detail::SetupUartNoFc(Id, huart, baud, parity, stop_bits);

    // Set up UART for the requested operation mode
    if constexpr (OM == hal::UartOperatingMode::Poll) {
      detail::InitializeUartForPollMode(huart);
    }
  }

 private:
  UART_HandleTypeDef huart;
};

template <UartId Id>
class Uart : public hal::UnusedPeripheral {};

}   // namespace stm32g0