#include <initialize.h>

#include <stm32g0xx_hal.h>

void HalInit() {
  HAL_Init();
}

void ClockInit() {
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_SYSCFG_StrobeDBattpinsConfig(SYSCFG_CFGR1_UCPD1_STROBE | SYSCFG_CFGR1_UCPD2_STROBE);

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