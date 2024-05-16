#include <initialize.h>

#include <stm32g4xx_hal.h>

void HalInit() {
  HAL_Init();
}

void ClockInit() {
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  RCC_OscInitTypeDef rcc_osc_init = {
      .OscillatorType      = RCC_OSCILLATORTYPE_HSI,
      .HSIState            = RCC_HSI_ON,
      .HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT,
      .PLL =
          {
              .PLLState  = RCC_PLL_ON,
              .PLLSource = RCC_PLLSOURCE_HSI,
              .PLLM      = RCC_PLLM_DIV4,
              .PLLN      = 85,
              .PLLP      = RCC_PLLP_DIV2,
              .PLLQ      = RCC_PLLQ_DIV2,
              .PLLR      = RCC_PLLR_DIV2,
          },
  };
  if (HAL_RCC_OscConfig(&rcc_osc_init) != HAL_OK) {
    return;
  }

  RCC_ClkInitTypeDef rcc_clk_init = {
      .ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                   | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
      .SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK,
      .AHBCLKDivider  = RCC_SYSCLK_DIV1,
      .APB1CLKDivider = RCC_HCLK_DIV1,
      .APB2CLKDivider = RCC_HCLK_DIV2,
  };

  if (HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_LATENCY_4) != HAL_OK) {
    return;
  }
}