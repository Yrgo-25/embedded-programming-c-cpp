/**
 * @brief GPIO types.
 */
#pragma once

#include <stdint.h>

namespace driver
{
namespace gpio
{
/**
 * @brief Enumeration of GPIO directions.
 */
enum class Direction : uint8_t
{
    Input,       ///< GPIO input without internal pull-up resistor enabled.
    InputPullup, ///< GPIO input with internal pull-up resistor enabled.
    Output,      ///< GPIO output.
};
} // namespace gpio
} // namespace driver
