#pragma once

#include <concepts>

namespace hal {

template <typename P>
concept Peripheral = requires {
  { P::Used } -> std::convertible_to<bool>;
} && (!P::Used || requires {
                       { P::instance() };
                     });

struct UnusedPeripheral {
  static constexpr auto Used = false;
};

class UsedPeripheral {
 public:
  static constexpr auto Used = true;

  UsedPeripheral()                                 = default;
  UsedPeripheral(const UsedPeripheral&)            = delete;
  UsedPeripheral(UsedPeripheral&&)                 = delete;
  UsedPeripheral& operator=(const UsedPeripheral&) = delete;
  UsedPeripheral& operator=(UsedPeripheral&&)      = delete;
};

}   // namespace hal