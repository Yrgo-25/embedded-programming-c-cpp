/**
 * @brief Implementation details for ATmega328p GPIO driver.
 */
#include <stdint.h>

#include <avr/io.h>

#include "driver/gpio/atmega328p.h"
#include "driver/gpio/types.h"
#include "driver/utils.h"

namespace driver
{
namespace gpio
{
namespace
{
/**
 * @brief Structure of pin offsets.
 */
struct PinOffset
{
    /** Pin offset for I/O port B. */
    static constexpr uint8_t B{8U};

    /** Pin offset for I/O port C. */
    static constexpr uint8_t C{14U};

    /** Pin offset for I/O port D. */
    static constexpr uint8_t D{0U};
};

/**
 * @brief Structure of port offsets.
 */
struct PortOffset
{
    /** Port offset for I/O port B. */
    static constexpr uint8_t B{0U};

    /** Port offset for I/O port C. */
    static constexpr uint8_t C{1U};

    /** Port offset for I/O port D. */
    static constexpr uint8_t D{2U};
};

/** Number of available pins. */
constexpr uint8_t PinCount{20U};

/** Number of available ports. */
constexpr uint8_t PortCount{3U};

/** Pin registry (1 = pin is reserved, 0 = pin is free). */
uint32_t pinRegistry{};

// -----------------------------------------------------------------------------
bool isPinFree(const uint8_t pin) noexcept
{
    // Return true if the pin number is valid and the pin is free.
    return PinCount > pin ? !READ(pinRegistry, pin) : false;
}

// -----------------------------------------------------------------------------
constexpr bool isDirectionValid(const Direction direction) noexcept
{
    // Cast to uint8_t to avoid negative integers.
    return static_cast<uint8_t>(Direction::Output) >= static_cast<uint8_t>(direction);
}
} // namespace

// -----------------------------------------------------------------------------
Atmega328p::Atmega328p(const uint8_t pin, const Direction direction) noexcept
    : myDirReg{nullptr}
    , myPortReg{nullptr}
    , myPinReg{nullptr}
    , myPin{}
    , myId{pin}
{
    // Check the pin, terminate the function if reserved or invalid.
    if (!isPinFree(pin)) { return; }

    // Check the data direction, terminate the function if invalid.
    if (!isDirectionValid(direction)) { return; }

    // Initialize GPIO.
    setRegs(pin);
    setDirection(direction);

    // Register pin.
    SET(pinRegistry, myId);
}

// -----------------------------------------------------------------------------
Atmega328p::~Atmega328p() noexcept
{
    // Release allocated resources if the GPIO is initialized.
    if (!isInitialized()) { return; }

    // Reset/clear used hardware (DDRx and PORTx).
    CLEAR(*myDirReg, myPin);
    CLEAR(*myPortReg, myPin);

    // Release the pin from the pin registry.
    CLEAR(pinRegistry, myId);
}

// -----------------------------------------------------------------------------
bool Atmega328p::isInitialized() const noexcept { return nullptr != myDirReg; }

// -----------------------------------------------------------------------------
void Atmega328p::write(const bool state) noexcept
{
    // Terminate the function is the GPIO is uninitialized.
    if (!isInitialized()) { return; }

    // Check data direction, terminate the function if not configured as output.
    if (!READ(*myDirReg, myPin)) { return; }

    // Check the desired state, set/clear the bit in PORTx.
    if (state) { SET(*myPortReg, myPin); }
    else { CLEAR(*myPortReg, myPin); }
}

// -----------------------------------------------------------------------------
bool Atmega328p::read() const noexcept
{
    // Read PINx if the GPIO is initialized, else return false.
    return isInitialized() ? READ(*myPinReg, myPin) : false;
}

// -----------------------------------------------------------------------------
void Atmega328p::toggle()
{
    // Terminate the function is the GPIO is uninitialized.
    if (!isInitialized()) { return; }

    // Check data direction, terminate the function if not configured as output.
    if (!READ(*myDirReg, myPin)) { return; }

    // Toggle the output by setting the pin in PINx (the hardware will toggle the output).
    SET(*myPinReg, myPin);
}

// -----------------------------------------------------------------------------
void Atmega328p::setRegs(const uint8_t pin) noexcept
{
    // PIN 0 - 7 => I/O port D, pin = ID.
    if (PinOffset::B > pin)
    {
        myDirReg  = &DDRD;
        myPortReg = &PORTD;
        myPinReg  = &PIND;
        myPin     = pin - PinOffset::D;
    }
    // PIN 8 - 13 => I/O port B, pin = ID - 8.
    else if (PinOffset::C > pin)
    {
        myDirReg  = &DDRB;
        myPortReg = &PORTB;
        myPinReg  = &PINB;
        myPin     = pin - PinOffset::B;
    }

    // PIN 14 - 19 => I/O port C, pin = ID - 14.
    else if (PinCount > pin)
    {
        myDirReg  = &DDRC;
        myPortReg = &PORTC;
        myPinReg  = &PINC;
        myPin     = pin - PinOffset::C;
    }
}

// -----------------------------------------------------------------------------
void Atmega328p::setDirection(const Direction direction) noexcept
{
    // Set GPIO direction.
    switch (direction)
    {
        case Direction::InputPullup:
        {
            // Configure as input by clearing the corresponding bit in DDRx.
            // Enable pull-up resistor by setting the corresponding bit in PORTx.
            CLEAR(*myDirReg, myPin);
            SET(*myPortReg, myPin);
            break;
        }
        case Direction::Output:
        {
            // Configure as output by setting the corresponding bit in DDRx.
            SET(*myDirReg, myPin);
            CLEAR(*myPortReg, myPin);
            break;
        }
        default:
        {
            // Configure as input by clearing the corresponding bit in DDRx.
            CLEAR(*myPortReg, myPin);
            CLEAR(*myDirReg, myPin);
            break;
        }
    }
}
} // namespace driver
} // namespace gpio
