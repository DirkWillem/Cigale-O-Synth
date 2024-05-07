#include <stm32g0xx_hal.h>

#include "peripherals/nucleo64.h"

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
    uart.Receive(buf);
  }
 private:
  std::array<std::byte, 128> buf{};
  Uart&                      uart;

  hal::MethodCallback<Echo, std::span<std::byte>> rx_callback;
};

void ClockInit() {
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  // Initialize oscillators
  RCC_OscInitTypeDef osc_init{
      .OscillatorType      = RCC_OSCILLATORTYPE_HSI,
      .HSEState            = RCC_HSE_OFF,
      .LSEState            = RCC_LSE_OFF,
      .HSIState            = RCC_HSI_ON,
      .HSIDiv              = RCC_HSI_DIV1,
      .HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT,
      .LSIState            = RCC_LSI_OFF,
      .HSI48State          = RCC_HSI48_OFF,
      .PLL                 = {.PLLState = RCC_PLL_OFF},
  };
  HAL_RCC_OscConfig(&osc_init);

  // Initialize Core, AHB and APB clocks
  RCC_ClkInitTypeDef clk_init{
      .ClockType =
          RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_SYSCLK,
      .SYSCLKSource   = RCC_SYSCLKSOURCE_HSI,
      .AHBCLKDivider  = RCC_HCLK_DIV1,
      .APB1CLKDivider = RCC_SYSCLK_DIV1,
  };
  HAL_RCC_ClockConfig(&clk_init, FLASH_LATENCY_0);
}

extern "C" [[noreturn]] int main() {
  HAL_Init();
  ClockInit();

  stm32g0::Gpo led{PIN(A, 5)};

  std::array<std::byte, 128> buf{};
  auto&                      usart2 = PcUart::instance();

  Echo echo{usart2};

  while (true) {
    led.Toggle();
    HAL_Delay(500);
  }
}

#include <stm32g0/interrupts.h>
