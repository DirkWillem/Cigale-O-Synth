#pragma once

#include <hal/callback.h>

#include <stm32g0/dma.h>
#include <stm32g0/pin.h>
#include <stm32g0/uart.h>

namespace stm32g0 {

using DmaChans =
    hal::DmaChannels<UartTxDma<UartId::Usart2>, UartRxDma<UartId::Usart2>>;

template <>
class Dma<DmaImplMarker> : public DmaImpl<Dma<DmaImplMarker>, DmaChans> {};

using AppDma = Dma<DmaImplMarker>;

static_assert(hal::Dma<AppDma>);

template <>
class Uart<UartId::Usart2>
    : public hal::RegisterableUartReceiveCallback
    , public UartImpl<Uart<UartId::Usart2>, UartId::Usart2,
                      hal::UartOperatingMode::Dma> {
  using Base = UartImpl<Usart2, UartId::Usart2, hal::UartOperatingMode::Dma>;
  friend class UartImpl<Usart2, UartId::Usart2, hal::UartOperatingMode::Dma>;

 protected:
  Uart()
      : UartImpl<Usart2, UartId::Usart2, hal::UartOperatingMode::Dma>{
            AppDma::instance(), {PIN(A, 2), PIN(A, 3)}, 115200} {}
};

}   // namespace stm32g0

using PcUart = stm32g0::Uart<stm32g0::UartId::Usart2>;
