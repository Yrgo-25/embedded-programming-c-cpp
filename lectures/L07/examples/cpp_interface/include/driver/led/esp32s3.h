/**
 * @brief LED driver for ESP32-S3.
 */
#pragma once

#include <cstdint>

#include "driver/led/interface.h"

namespace driver::led
{
/**
 * @brief LED driver for ESP32-S3.
 * 
 *        This class is non-copyable and non-movable.
 */
class Esp32s3 final : public Interface
{
public:
    /**
     * @brief Constructor.
     * 
     * @param[in] pin The pin the LED is connected to.
     * @param[in] initialState Initial state of the LED (default = off).
     */
    explicit Esp32s3(std::uint8_t pin, bool initialState = false) noexcept;

    /**
     * @brief Destructor.
     */
    ~Esp32s3() noexcept override;

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

    Esp32s3()                          = delete; // No default constructor.
    Esp32s3(const Esp32s3&)            = delete; // No copy constructor.
    Esp32s3(Esp32s3&&)                 = delete; // No move constructor.
    Esp32s3& operator=(const Esp32s3&) = delete; // No copy assignment.
    Esp32s3& operator=(Esp32s3&&)      = delete; // No move assignment.

private:
    /**
     * @brief Initialize the LED.
     */
    void init(bool initialState) noexcept;

    /** The pin the LED is connected to. */
    std::uint8_t myPin;

    /** Indicate whether the LED is enabled. */
    bool myIsEnabled;
};
} // namespace driver::led
