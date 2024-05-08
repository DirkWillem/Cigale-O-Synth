#pragma once

#include <hal/callback.h>

#include <stm32g4/dma.h>
#include <stm32g4/pin.h>
#include <stm32g4/uart.h>

namespace stm32g4 {

using DmaChans =
    hal::DmaChannels<UartTxDma<UartId::LpUart1>, UartRxDma<UartId::LpUart1>>;

template <>
class Dma<DmaImplMarker> : public DmaImpl<Dma<DmaImplMarker>, DmaChans> {};

using AppDma = Dma<DmaImplMarker>;

static_assert(hal::Dma<AppDma>);

template <>
class Uart<UartId::LpUart1>
    : public hal::RegisterableUartReceiveCallback
    , public UartImpl<Uart<UartId::LpUart1>, UartId::LpUart1,
                      hal::UartOperatingMode::Dma> {
  using Base = UartImpl<LpUart1, UartId::LpUart1, hal::UartOperatingMode::Dma>;
  friend class UartImpl<LpUart1, UartId::LpUart1, hal::UartOperatingMode::Dma>;

 protected:
  Uart()
      : UartImpl<LpUart1, UartId::LpUart1, hal::UartOperatingMode::Dma>{
            AppDma::instance(), {PIN(A, 2), PIN(A, 3)}, 115200} {}

  //  Uart()
  //      : UartImpl<LpUart1, UartId::LpUart1,
  //      hal::UartOperatingMode::Interrupt>{
  //            {PIN(A, 2), PIN(A, 3)}, 115200} {}
};

}   // namespace stm32g4

using PcUart = stm32g4::Uart<stm32g4::UartId::LpUart1>;
