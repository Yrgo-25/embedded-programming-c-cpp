/**
 * @brief LED driver interface.
 */
#pragma once

#include <cstdint>

namespace driver::led
{
/**
 * @brief LED driver interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Get the pin the LED is connected to.
     * 
     * @return The pin the LED is connected to.
     */
    virtual std::uint8_t pin() const noexcept = 0;

    /**
     * @brief Check whether the LED is enabled.
     * 
     * @return True if the LED is enabled, false otherwise.
     */
    virtual bool isEnabled() const noexcept = 0;

    /**
     * @brief Enable/disable the LED.
     * 
     * @param[in] enable True if the LED is to be enabled, false otherwise.
     */
    virtual void setEnabled(bool enable) noexcept = 0;

    /**
     * @brief Toggle the LED.
     */
    virtual void toggle() noexcept = 0;
};
} // namespace driver::led
