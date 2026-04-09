/**
 * @brief GPIO interface.
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
    Input,       // Input without internal pull-up resistor enabled (tri-state).
    InputPullup, // Input with internal pull-up resistor enabled.
    Output,      // Output.
    Count,       // Number of supported data directions.
};

/** @todo Add GPIO interface here! */

} // namespace gpio
} // namespace driver
