/**
 * @brief GPIO driver implementation details for ATmega328P.
 */
#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include "driver/gpio/atmega328p.h"
#include "utils/bit_operations.h"

namespace driver 
{
namespace gpio
{
namespace
{
/**
 * @brief Structure of indexes for callbacks associated with the I/O ports.
 */
struct CbIndex
{
    /** Index for callback associated with I/O port B. */
    static constexpr uint8_t PortB{0U};

    /** Index for callback associated with I/O port C. */
    static constexpr uint8_t PortC{1U};

    /** Index for callback associated with I/O port D. */
    static constexpr uint8_t PortD{2U};
};

/**
 * @brief Structure of pin offsets, i.e. the discrepancy between the Arduino and the ATmega328p
 *        pin numbers, for each I/O port.
 */
struct PinOffset
{
    /** Pin offset for I/O port B. */
    static constexpr uint8_t PortB{8U};

    /** Pin offset for I/O port C. */
    static constexpr uint8_t PortC{14U};

    /** Pin offset for I/O port D. */
    static constexpr uint8_t PortD{0U};
};

/** The number of available I/O ports. */
constexpr uint8_t IoPortCount{3U};

/** The number of available GPIO pins. */
constexpr uint8_t PinCount{20U};

/** Callback alias. */
typedef void (*callback_t)();

/** Pointers to callbacks. */
callback_t myCallbacks[IoPortCount]{};

/** Pin registry (1 = reserved, 0 = free). */
uint32_t myPinRegistry{};

constexpr bool inRange(uint8_t number, uint8_t min, uint8_t max) noexcept;
constexpr bool isPinFree(uint8_t id) noexcept;
constexpr bool isDirectionValid(Direction direction) noexcept;
Hardware* findHw(Atmega328p::IoPort ioPort) noexcept;
void invokeCallback(uint8_t index) noexcept;

} // namespace

/**
 * @brief GPIO hardware structure.
 */
struct Hardware 
{
    /** Reference to data direction register (DDRx). */
    volatile uint8_t& ddrx;

    /** Reference to port (output) register (PORTx). */
    volatile uint8_t& portx;

    /** Reference to pin (input) register (PINx). */
    volatile uint8_t& pinx;

    /** Reference to pin change interrupt mask register (PCMSKx). */
    volatile uint8_t& pcmskx;

    /** Control bit in the pin change interrupt control register (PCIEx). */
    const uint8_t pcix;
};

/** Hardware structure for I/O port B. */
struct Hardware myHwPortB
{
    .ddrx   = DDRB,
    .portx  = PORTB,
    .pinx   = PINB,
    .pcmskx = PCMSK0,
    .pcix   = PCIE0,
};

/** Hardware structure for I/O port C. */
struct Hardware myHwPortC
{
    .ddrx   = DDRC,
    .portx  = PORTC,
    .pinx   = PINC,
    .pcmskx = PCMSK1,
    .pcix   = PCIE1,
};

/** Hardware structure for I/O port D. */
struct Hardware myHwPortD
{
    .ddrx   = DDRD,
    .portx  = PORTD,
    .pinx   = PIND,
    .pcmskx = PCMSK2,
    .pcix   = PCIE2,
};

// -----------------------------------------------------------------------------
Atmega328p::Atmega328p(const uint8_t pin, const Direction direction, void (*callback)()) noexcept
    : myHw{nullptr}
    , myDirection{direction}
    , myIoPort{getIoPort(pin)}
    , myId{pin}
    , myPin{getPhysicalPin()}
{ 
    // Reserve hardware if the pin is free and the data direction is valid.
    // Put the GPIO in safe sstate on failure.
    if (isPinFree(myId) && isDirectionValid(myDirection))
    {
        // Register the given callback for the associated I/O port if specified.
        if (initHw() && (nullptr != callback))
        {
            if (PORTB == myHw->portx)      { myCallbacks[CbIndex::PortB] = callback; }
            else if (PORTC == myHw->portx) { myCallbacks[CbIndex::PortC] = callback; } 
            else if (PORTD == myHw->portx) { myCallbacks[CbIndex::PortD] = callback; }
        }
    }
}

// -----------------------------------------------------------------------------
Atmega328p::~Atmega328p() noexcept 
{   
    // Skip cleanup if the GPIO is uninitialized.
    if (!isInitialized()) { return; }

    // Free resources used for the GPIO before deletion.
    enablePci(false);
    bit::clear(myHw->ddrx, myPin);
    bit::clear(myHw->portx, myPin);
    bit::clear(myPinRegistry, myId);
    myHw = nullptr; 
}

// -----------------------------------------------------------------------------
bool Atmega328p::isInitialized() const noexcept { return nullptr != myHw; }

// -----------------------------------------------------------------------------
bool Atmega328p::read() const noexcept 
{ 
    // Only read input if the GPIO is initialized.
    return isInitialized() ? bit::read(myHw->pinx, myPin) : false;
}

// -----------------------------------------------------------------------------
void Atmega328p::write(const bool output) noexcept
{
    // Only write output if the GPIO is initialized and configured as output.
    if (!isInitialized() || (myDirection != Direction::Output)) { return; }

    // Set/clear the output as specified.
    if (output) { bit::set(myHw->portx, myPin); }
    else { bit::clear(myHw->portx, myPin); }
}

// -----------------------------------------------------------------------------
void Atmega328p::toggle() noexcept 
{ 
    // Only toggle output if the GPIO is initialized and configured as output.
    if (!isInitialized() || (myDirection != Direction::Output)) { return; }

    // The hardware will toggle the output when writing to the pin register.
    bit::set(myHw->pinx, myPin); 
}

// -----------------------------------------------------------------------------
void Atmega328p::enablePci(const bool enable) noexcept
{
    // Only enable interrupts if the GPIO is initialized.
    if (!isInitialized()) { return; }

    // Enable/disable interrupts on the associated pin as specified.
    if (enable)
    {
        sei();
        bit::set(PCICR, myHw->pcix);
        bit::set(myHw->pcmskx, myPin);
    }
    else { bit::clear(myHw->pcmskx, myPin); }
}

// -----------------------------------------------------------------------------
void Atmega328p::enablePciOnPort(const bool enable) noexcept 
{ 
    // Only enable interrupts on the associated port if the GPIO is initialized.
    if (!isInitialized()) { return; }

    // Enable/disable interrupts on the associated port as specified.
    if (enable) { bit::set(PCICR, myHw->pcix); }
    else { bit::clear(PCICR, myHw->pcix); }
}

// -----------------------------------------------------------------------------
Atmega328p::IoPort Atmega328p::getIoPort(const uint8_t id) const noexcept
{
    // Return the port associated with the given ID, or an invalid enum on failure.
    if (inRange(id, Port::B0, Port::B5))      { return IoPort::B; }
    else if (inRange(id, Port::C0, Port::C5)) { return IoPort::C; }
    else if (inRange(id, Port::D0, Port::D7)) { return IoPort::D; }
    return IoPort::Count;
}

// -----------------------------------------------------------------------------
uint8_t Atmega328p::getPhysicalPin() const noexcept
{
    // Return the physical pin associated with the ID, or -1 on failure.
    switch (myIoPort)
    {
        case IoPort::B:
            return myId - PinOffset::PortB;
        case IoPort::C:
            return myId - PinOffset::PortC;
        case IoPort::D:
            return myId - PinOffset::PortD;
        default:
            return static_cast<uint8_t>(-1);
    }
}

// -----------------------------------------------------------------------------
bool Atmega328p::initHw() noexcept
{
    // Find the associated hardware, set up on success.
    myHw = findHw(myIoPort);
    if (nullptr == myHw) { return false; }

    // Mark the pin as reserved.
    bit::set(myPinRegistry, myId); 

    // Set data direction as specified.
    switch (myDirection)
    {
        // Enable the interupt pull-up resistor if specified.
        case Direction::InputPullup:
        {
            bit::set(myHw->portx, myPin);
            break;
        }
        // Set the GPIO to output if specified.
        case Direction::Output:
        {
            bit::set(myHw->ddrx, myPin);
            break;
        }
        // Do nothing as default - operate as tri-state input.
        default:
        {
            break;
        }
    }
    return true;
}

// -----------------------------------------------------------------------------
ISR(PCINT0_vect) { invokeCallback(CbIndex::PortB); }

// -----------------------------------------------------------------------------
ISR(PCINT1_vect) { invokeCallback(CbIndex::PortC); }

// -----------------------------------------------------------------------------
ISR(PCINT2_vect) { invokeCallback(CbIndex::PortD); }

namespace
{
// -----------------------------------------------------------------------------
constexpr bool inRange(const uint8_t number, const uint8_t min, const uint8_t max) noexcept
{
    return ((min <= number) && (max >= number));
}

// -----------------------------------------------------------------------------
constexpr bool isPinFree(const uint8_t id) noexcept { return PinCount > id; }

// -----------------------------------------------------------------------------
constexpr bool isDirectionValid(const Direction direction) noexcept
{
    return static_cast<uint8_t>(Direction::Count) > static_cast<uint8_t>(direction);
}

// -----------------------------------------------------------------------------
Hardware* findHw(const Atmega328p::IoPort ioPort) noexcept
{
    // Return the hardware associated with the ID, or a nullptr on failure.
    switch (ioPort)
    {
        case Atmega328p::IoPort::B:
            return &myHwPortB;
        case Atmega328p::IoPort::C:
            return &myHwPortC;
        case Atmega328p::IoPort::D:
            return &myHwPortD;
        default:
            return nullptr;
    }
}

// -----------------------------------------------------------------------------
void invokeCallback(uint8_t index) noexcept
{
    if ((IoPortCount > index) && (nullptr != myCallbacks[index]))
    {
        myCallbacks[index]();
    }
}
} // namespace
} // namespace gpio
} // namespace driver
