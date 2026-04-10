/**
 * @brief Buzzer driver.
 */
#pragma once

#include <cstdint>

namespace driver
{
/**
 * @brief Buzzer driver.
 * 
 *        This class is non-copyable and non-movable.
 */
class Buzzer final
{
public:
    /**
     * @brief Constructor.
     * 
     * @param[in] pin Buzzer pin.
     * @param[in] initialState Initial state (default = off).
     */
    explicit Buzzer(std::uint8_t pin, bool initialState = false) noexcept;

    /**
     * @brief Destructor.
     */
    ~Buzzer() noexcept;

    /**
     * @brief Get the buzzer pin.
     * 
     * @return The buzzer pin number.
     */
    std::uint8_t pin() const noexcept;

    /**
     * @brief Check if the buzzer is enabled.
     * 
     * @return True if the buzzer is enabled, false otherwise.
     */
    bool isEnabled() const noexcept;

    /**
     * @brief Enable the buzzer.
     */
    void enable() noexcept;

    /**
     * @brief Disable the buzzer.
     */
    void disable() noexcept;

    /**
     * @brief Toggle the buzzer.
     */
    void toggle() noexcept;

    Buzzer()                         = delete; // No default constructor.
    Buzzer(const Buzzer&)            = delete; // No copy constructor.
    Buzzer(Buzzer&&)                 = delete; // No move constructor.
    Buzzer& operator=(const Buzzer&) = delete; // No copy assignment.
    Buzzer& operator=(Buzzer&&)      = delete; // No move assignment.

private:
    /** Buzzer pin. */
    const std::uint8_t myPin;

    /** Buzzer state. */
    bool myEnabled;
};
} // namespace driver
