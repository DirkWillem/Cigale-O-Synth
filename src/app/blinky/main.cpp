#include <initialize.h>
#include <peripherals.h>

#include <stm32g0/i2s.h>

#include <cmath>

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

  int16_t     signal[1024];
  std::size_t nsamples = sizeof(signal) / sizeof(signal[0]);

  //  stm32g0::I2sImpl<int, stm32g0::I2sId::I2s1, hal::I2sOperatingMode::Dma,
  //  false>
  //      i2s1{{PIN(A, 1), PIN(A, 7), PIN(A, 4)},
  //      stm32g0::I2sAudioFrequency::KHz8};

  auto& i2s1 = AmpI2s::instance();

  constexpr auto Pi  = 3.14159265358979323846F;
  constexpr auto Tau = 2 * Pi;

  int i = 0;
  while (i < nsamples) {
    double t      = ((double)i / 2.0) / ((double)nsamples);
    signal[i]     = 32767 / 2 * sin(5000.F * Tau * t);   // left
    signal[i + 1] = signal[i];                           // right
    i += 2;
  }

  stm32g0::Gpo led{PIN(A, 5)};
  stm32g0::Gpo amp_sd{PIN(A, 0)};
  stm32g0::Gpi btn{PIN(C, 13)};
  //  auto&        inst = PcUart::instance();
  //
  //  Echo echo{inst};

  amp_sd.Write(true);
  i2s1.StartTransmit({reinterpret_cast<uint16_t*>(signal), nsamples});

  while (true) {
    //    i2s1.Transmit({reinterpret_cast<uint16_t*>(signal), nsamples});
    led.Toggle();
    HAL_Delay(100);
  }
}

#include <interrupts_callbacks.h>