/**
 * @brief GPIO driver for ATmega328p.
 */
#pragma once

#include <stdint.h>

#include "driver/gpio/types.h"

namespace driver
{
namespace gpio
{
/**
 * @brief GPIO driver for ATmega328p.
 * 
 *        This class is non-copyable and non-movable.
 */
class Atmega328p final
{
public:
    /**
     * @brief Constructor.
     * 
     * @param[in] pin Arduino pin number.
     * @param[in] direction Data direction.
     */
    explicit Atmega328p(uint8_t pin, Direction direction) noexcept;

    /**
     * @brief Destructor.
     */
    ~Atmega328p() noexcept;

    /**
     * @brief Check if the GPIO is initialized.
     * 
     * @return True if the GPIO is initialized, false otherwise.
     */
    bool isInitialized() const noexcept;
     
    /**
     * @brief Set state of the GPIO.
     * 
     * @note This operation is only supported for outputs.
     *
     * @param[in] state GPIO state (true = enabled, false = disabled).
     */
    void write(bool state) noexcept;

    /**
     * @brief Read the state of the GPIO.
     * 
     * @return True if the GPIO is enabled, false otherwise.
     */
    bool read() const noexcept;

    /**
     * @brief Toggle state of the GPIO.
     * 
     * @note This operation is only supported for outputs.
     */
    void toggle() noexcept;

    Atmega328p()                       = delete; // No default destructor.
    Atmega328p(const Atmega328p&)            = delete; // No copy constructor.
    Atmega328p(Atmega328p&&)                 = delete; // No move constructor.
    Atmega328p& operator=(const Atmega328p&) = delete; // No copy assignment.
    Atmega328p& operator=(Atmega328p&&)      = delete; // No move assignment.

private:
    void setRegs(uint8_t pin) noexcept;
    void setDirection(Direction direction) noexcept;

    /** Pointer to the data direction register. */
    volatile uint8_t* myDirReg;

    /** Pointer to the port register. */
    volatile uint8_t* myPortReg;

    /** Pointer to the pin register. */
    volatile uint8_t* myPinReg;

    /** Pin number on the associated port. */
    uint8_t myPin;

    /** Pin ID (for reserving the pin). */
    const uint8_t myId;
};
} // namespace gpio
} // namespace driver
