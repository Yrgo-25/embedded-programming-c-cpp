/**
 * @brief LED driver for ATmega328P.
 */
#pragma once

#include <cstdint>

#include "driver/led/interface.h"

namespace driver::led
{
/**
 * @brief LED driver for ATmega328P.
 * 
 *        This class is non-copyable and non-movable.
 */
class Atmega328p final : public Interface
{
public:
    /**
     * @brief Constructor.
     * 
     * @param[in] pin The pin the LED is connected to.
     */
    explicit Atmega328p(std::uint8_t pin) noexcept;

    /**
     * @brief Destructor.
     */
    ~Atmega328p() noexcept override;

    /**
     * @brief Get the pin the LED is connected to.
     * 
     * @return The pin the LED is connected to.
     */
    std::uint8_t pin() const noexcept override;

    /**
     * @brief Check whether the LED is enabled.
     * 
     * @return True if the LED is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Enable/disable the LED.
     * 
     * @param[in] enable True if the LED is to be enabled, false otherwise.
     */
    void setEnabled(bool enable) noexcept override;

    /**
     * @brief Toggle the LED.
     */
    void toggle() noexcept override;

    Atmega328p()                             = delete; // No default constructor.
    Atmega328p(const Atmega328p&)            = delete; // No copy constructor.
    Atmega328p(Atmega328p&&)                 = delete; // No move constructor.
    Atmega328p& operator=(const Atmega328p&) = delete; // No copy assignment.
    Atmega328p& operator=(Atmega328p&&)      = delete; // No move assignment.

private:
    /** The pin the LED is connected to. */
    std::uint8_t myPin;

    /** Indicate whether the LED is enabled. */
    bool myIsEnabled;
};
} // namespace driver::led
