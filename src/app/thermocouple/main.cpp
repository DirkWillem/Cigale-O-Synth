#include <initialize.h>
#include <peripherals.h>

#include <cmath>

#include <fp/fix.h>

template <typename Uart>
  requires hal::AsyncUart<Uart> && hal::UartRegisterReceiveCallback<Uart>
class Echo {
 public:
  explicit Echo(Uart& uart)
      : uart{uart}
      , rx_callback{this, &Echo::ReceiveCallback} {
    uart.RegisterUartReceiveCallback(rx_callback);
    uart.Receive(buf);
  }

  void ReceiveCallback(std::span<std::byte> data) {
    uart.Write(data);
    HAL_Delay(5);
    uart.Write(data);
    uart.Receive(buf);
  }

 private:
  std::array<std::byte, 128> buf{};
  Uart&                      uart;

  hal::MethodCallback<Echo, std::span<std::byte>> rx_callback;
};

extern "C" [[noreturn]] int main() {
  HalInit();
  ClockInit();

  static_assert(std::convertible_to<decltype(stm32g4::AppDma::instance()),
                                    stm32g4::AppDma&>);

  stm32g4::Gpo led{PIN(A, 5)};
  stm32g4::Gpo thermocouple_ssel{pinout::ThermocoupleSpiSlaveSel};

  auto& thermocouple_spi = ThermocoupleSpi::instance();

  uint16_t data{0};

  while (true) {
    thermocouple_ssel.Write(false);
    thermocouple_spi.Receive({&data, 1}, 50);
    thermocouple_ssel.Write(true);
    led.Toggle();

    data >>= 3;

    const auto temperature = fp::Fix<false, 16, 2>{data};
    const volatile auto temperature_f = static_cast<float>(temperature);
    HAL_Delay(100);
  }
}

#include <interrupts_callbacks.h>