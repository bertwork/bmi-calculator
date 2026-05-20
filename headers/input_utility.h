#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <string_view>
#include <type_traits>

template <typename T>
void getInput(std::string_view prompt, T &out, T min, T max) {
  static_assert(std::is_arithmetic_v<T>,
                "getInput only supports numeric types");

  while (true) {
    std::cout << prompt;

    double raw;

    if (std::cin >> raw) {

      // Reject decimals for integer types
      if constexpr (std::is_integral_v<T>) {
        if (std::floor(raw) != raw) {
          std::cout << "  Invalid input. Enter a whole number between " << min
                    << " and " << max << ".\n";

          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          continue;
        }
      }

      // Validate range BEFORE casting
      if (raw >= static_cast<double>(min) && raw <= static_cast<double>(max)) {

        out = static_cast<T>(raw);

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        return;
      }
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "  Invalid input. Enter a value between " << min << " and "
              << max << ".\n";
  }
}