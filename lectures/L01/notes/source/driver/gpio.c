/**
 * @brief Implementation details for ATmega328p GPIO driver.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <avr/io.h>

#include "driver/gpio.h"

/** Number of available pins. */
#define PIN_COUNT 20U

/** Pin offset for I/O port B. */
#define PIN_OFFSET_B 8U

/** Pin offset for I/O port C. */
#define PIN_OFFSET_C 14U

/** Pin offset for I/O port D. */
#define PIN_OFFSET_D 0U

// -----------------------------------------------------------------------------
bool gpio_init(gpio_t* self, const uint8_t pin, const gpio_direction_t direction)
{
    // Check the GPIO, return false if invalid.
    if (NULL == self) { return false; }

    // Check the pin number, return false if invalid (>= PIN_COUNT).
    if (PIN_COUNT <= pin) { return false; }

    // Check the data direction, return false if invalid.
    // Cast to uint8_t to avoid negative integers.
    if ((uint8_t)(GPIO_DIRECTION_OUTPUT) < (uint8_t)(direction)) { return false; }

    // Store pin ID (for pin registration).
    self->id = pin;

    // PIN 0 - 7 => I/O port D.
    if (PIN_OFFSET_B > pin)
    {
        self->ddrx  = &DDRD;
        self->portx = &PORTD;
        self->pinx  = &PIND;
        self->pin   = pin - PIN_OFFSET_D;
    }
    // PIN 8 - 13 => I/O port B, pin = ID - 8.
    else if (PIN_OFFSET_C > pin)
    {
        self->ddrx  = &DDRB;
        self->portx = &PORTB;
        self->pinx  = &PINB;
        self->pin   = pin - PIN_OFFSET_B;
    }

    // PIN 14 - 19 => I/O port C., pin = ID - 14.
    else if (PIN_COUNT > pin)
    {
        self->ddrx  = &DDRC;
        self->portx = &PORTC;
        self->pinx  = &PINC;
        self->pin   = pin - PIN_OFFSET_C;
    }

    /** @todo: Set data direction. */
    return true;
}

// -----------------------------------------------------------------------------
void gpio_deinit(gpio_t* self)
{

}

// -----------------------------------------------------------------------------
void gpio_write(gpio_t* self, bool state)
{

}

// -----------------------------------------------------------------------------
bool gpio_read(const gpio_t* self)
{
    // Placeholder.
    return true;
}
