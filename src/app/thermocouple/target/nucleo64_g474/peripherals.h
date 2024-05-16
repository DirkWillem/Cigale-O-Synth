#pragma once

#include <hal/callback.h>

#include <stm32g4/dma.h>
#include <stm32g4/pin.h>
#include <stm32g4/spi.h>
#include <stm32g4/uart.h>

namespace pinout {

static constexpr auto ThermocoupleSpiInst     = stm32g4::SpiId::Spi2;
static constexpr auto ThermocoupleSpiSck      = PIN(B, 13);
static constexpr auto ThermocoupleSpiMiso     = PIN(B, 14);
static constexpr auto ThermocoupleSpiSlaveSel = PIN(B, 15);

}   // namespace pinout

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
};

template <>
class Spi<pinout::ThermocoupleSpiInst>
    : public SpiImpl<Spi<pinout::ThermocoupleSpiInst>,
                     pinout::ThermocoupleSpiInst, 16, hal::SpiMode::Master,
                     hal::SpiTransmissionType::RxOnly> {
  friend class SpiImpl<Spi<pinout::ThermocoupleSpiInst>,
                       pinout::ThermocoupleSpiInst, 16, hal::SpiMode::Master,
                       hal::SpiTransmissionType::RxOnly>;

 protected:
  Spi()
      : SpiImpl<Spi<pinout::ThermocoupleSpiInst>, pinout::ThermocoupleSpiInst,
                16, hal::SpiMode::Master, hal::SpiTransmissionType::RxOnly>{
            {pinout::ThermocoupleSpiMiso, pinout::ThermocoupleSpiSck},
            SpiBaudPrescaler::Prescale256} {}
};

}   // namespace stm32g4

using PcUart          = stm32g4::Uart<stm32g4::UartId::LpUart1>;
using ThermocoupleSpi = stm32g4::Spi<pinout::ThermocoupleSpiInst>;
