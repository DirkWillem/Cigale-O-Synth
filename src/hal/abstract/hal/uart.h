#pragma once

namespace hal {

enum class UartOperatingMode { Poll, Interrupt, Dma };

enum class UartFlowControl { None };

enum class UartParity { Even, Odd, None };

enum class UartStopBits { Half, One, OneAndHalf, Two };

}   // namespace hal