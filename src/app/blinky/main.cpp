#include <initialize.h>
#include <peripherals.h>

template <typename Uart>
  requires hal::Uart<Uart> && hal::UartRegisterReceiveCallback<Uart>
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

  stm32g0::Gpo led{PIN(A, 5)};
  auto& inst = PcUart::instance();

  Echo echo{inst};


  while (true) {
    led.Toggle();
    HAL_Delay(500);
  }
}

#include <interrupts_callbacks.h>