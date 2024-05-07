#include <stm32g0xx_hal.h>

#include <stm32g0/dma.h>
#include <stm32g0/pin.h>
#include <stm32g0/uart.h>

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

using DmaChans = hal::DmaChannels<stm32g0::UartTxDma<stm32g0::UartId::Usart2>,
                                  stm32g0::UartRxDma<stm32g0::UartId::Usart2>>;

template <>
class stm32g0::Dma<stm32g0::DmaImplMarker>
    : public stm32g0::DmaImpl<stm32g0::Dma<stm32g0::DmaImplMarker>, DmaChans> {
};

using AppDma = stm32g0::Dma<stm32g0::DmaImplMarker>;

static_assert(hal::Dma<AppDma>);

template <>
class stm32g0::Uart<stm32g0::UartId::Usart2>
    : public stm32g0::UartImpl<Usart2, stm32g0::UartId::Usart2,
                               hal::UartOperatingMode::Dma> {
  using Base = stm32g0::UartImpl<Usart2, stm32g0::UartId::Usart2,
                                 hal::UartOperatingMode::Dma>;
  friend class stm32g0::UartImpl<Usart2, stm32g0::UartId::Usart2,
                                 hal::UartOperatingMode::Dma>;

 protected:
  Uart()
      : Base{AppDma::instance(), {PIN(A, 2), PIN(A, 3)}, 115200} {}
};

extern "C" [[noreturn]] int main() {
  HAL_Init();
  ClockInit();

  stm32g0::Gpo led{PIN(A, 5)};

  auto& usart2 = stm32g0::Usart2::instance();

  while (true) {
    led.Toggle();
    HAL_Delay(500);
    usart2.Write("Hello, DMA!\r\n");
  }
}

#include <stm32g0/interrupts.h>
