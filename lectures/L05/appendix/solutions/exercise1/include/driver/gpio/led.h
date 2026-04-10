/**
 * @brief LED driver.
 */
#pragma once

#include <cstdint>

namespace driver::gpio
{
/**
 * @brief LED driver.
 * 
 *        This class is non-copyable and non-movable.
 */
class Led final
{
public:
    /**
     * @brief Constructor.
     * 
     * @param[in] pin LED pin.
     * @param[in] initialState Initial state (default = off).
     */
    explicit Led(const std::uint8_t pin, const bool initialState = false) noexcept
        : myPin{pin}
        , myState{initialState}
    {}

    /**
     * @brief Destructor.
     */
    ~Led() noexcept = default;

    /**
     * @brief Check if the LED is on.
     * 
     * @return True if the LED is on, false otherwise.
     */
    bool isOn() const noexcept { return myState; }

    /**
     * @brief Enable the LED.
     */
    void on() noexcept { myState = true; }

    /**
     * @brief Disable the LED.
     */
    void off() noexcept { myState = false; }

    /** 
     * @brief Toggle the LED.
     */
    void toggle() noexcept { myState = !myState; }

    Led()                      = delete; // No default constructor.
    Led(const Led&)            = delete; // No copy constructor.
    Led(Led&&)                 = delete; // No move constructor.
    Led& operator=(const Led&) = delete; // No copy assignment.
    Led& operator=(Led&&)      = delete; // No move assignment.


private:
    /** LED pin. */
    const std::uint8_t myPin;

    /** LED state. */
    bool myState;
};
} // namespace driver::gpio