#pragma once

#include <hal/callback.h>

#include <stm32g0/dma.h>
#include <stm32g0/i2s.h>
#include <stm32g0/pin.h>
#include <stm32g0/uart.h>

namespace stm32g0 {

using DmaChans =
    hal::DmaChannels<UartTxDma<UartId::Usart2>, UartRxDma<UartId::Usart2>,
                     I2sTxDma<I2sId::I2s1>>;

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

template <>
class I2s<I2sId::I2s1>
    : public I2sImpl<I2s<I2sId::I2s1>, I2sId::I2s1,
                     hal::I2sOperatingMode::DmaCircular> {
  friend class I2sImpl<I2s<I2sId::I2s1>, I2sId::I2s1,
                       hal::I2sOperatingMode::DmaCircular>;

 protected:
  I2s()
      : I2sImpl<I2s<I2sId::I2s1>, I2sId::I2s1, hal::I2sOperatingMode::DmaCircular>{
            AppDma::instance(),
            {PIN(A, 1), PIN(A, 7), PIN(A, 4)},
            stm32g0::I2sAudioFrequency::KHz8} {}
};

//template <>
//class I2s<I2sId::I2s1>
//    : public I2sImpl<I2s<I2sId::I2s1>, I2sId::I2s1,
//                     hal::I2sOperatingMode::Poll> {
//  friend class I2sImpl<I2s<I2sId::I2s1>, I2sId::I2s1,
//                       hal::I2sOperatingMode::Poll>;
//
// protected:
//  I2s()
//      : I2sImpl<I2s<I2sId::I2s1>, I2sId::I2s1, hal::I2sOperatingMode::Poll>{
//            {PIN(A, 1), PIN(A, 7), PIN(A, 4)},
//            stm32g0::I2sAudioFrequency::KHz8} {}
//};


}   // namespace stm32g0

using PcUart = stm32g0::Uart<stm32g0::UartId::Usart2>;
using AmpI2s = stm32g0::I2s<stm32g0::I2sId::I2s1>;
