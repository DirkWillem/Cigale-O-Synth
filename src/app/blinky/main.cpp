#include <stm32g0xx_hal.h>

#include <stm32g0/pin.h>
#include <stm32g0/uart.h>

void ClockInit() {
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

class Usart2 : public stm32g0::UartImpl<Usart2, stm32g0::UartId::Usart2> {
  friend class stm32g0::UartImpl<Usart2, stm32g0::UartId::Usart2>;

 protected:
  Usart2()
      : stm32g0::UartImpl<Usart2, stm32g0::UartId::Usart2>{
            {PIN(A, 2), PIN(A, 3)}, 115200} {}
};

extern "C" [[noreturn]] int main() {
  HAL_Init();
  ClockInit();

  stm32g0::Gpo led{PIN(A, 5)};

  auto& usart2 = Usart2::instance();

  while (true) {
    led.Toggle();
    HAL_Delay(500);
    usart2.Write("Hello, World!\n");
  }
}