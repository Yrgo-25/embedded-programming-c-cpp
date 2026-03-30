/**
 * @brief Implementation details for ATmega328p GPIO driver.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <avr/io.h>

#include "driver/gpio.h"
#include "driver/utils.h"

/** Number of available pins. */
#define PIN_COUNT 20U

/** Pin offset for I/O port B. */
#define PIN_OFFSET_B 8U

/** Pin offset for I/O port C. */
#define PIN_OFFSET_C 14U

/** Pin offset for I/O port D. */
#define PIN_OFFSET_D 0U

/**
 * @brief GPIO driver structure.
 */
typedef struct gpio
{
    /** Pointer to the data direction register. */
    volatile uint8_t* ddrx;

    /** Pointer to the port register. */
    volatile uint8_t* portx;

    /** Pointer to the pin register. */
    volatile uint8_t* pinx;

    /** Pin number on the associated port. */
    uint8_t pin;

    /** Pin ID (for reserving the pin). */
    uint8_t id;
} gpio_t;

// -----------------------------------------------------------------------------
gpio_t* gpio_new(const uint8_t pin, const gpio_direction_t direction)
{
    // Check the pin number, return a nullptr if invalid (>= PIN_COUNT).
    if (PIN_COUNT <= pin) { return NULL; }

    // Check the data direction, return a nullptr if invalid.
    // Cast to uint8_t to avoid negative integers.
    if ((uint8_t)(GPIO_DIRECTION_OUTPUT) < (uint8_t)(direction)) { return NULL; }

    // Allocate memory for a new GPIO, return a nullptr on failure.
    //! @note sizeof(gpio_t) == 8 bytes.
    gpio_t* self = (gpio_t*)(malloc(sizeof(gpio_t)));
    if (NULL == self) { return NULL; }

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

    // Set GPIO direction.
    switch (direction)
    {
        case GPIO_DIRECTION_INPUT_PULLUP:
        {
            SET(*self->portx, self->pin);
            CLEAR(*self->ddrx, self->pin);
            break;
        }
        case GPIO_DIRECTION_OUTPUT:
        {
            CLEAR(*self->portx, self->pin);
            SET(*self->ddrx, self->pin);
            break;
        }
        default:
        {
            CLEAR(*self->portx, self->pin);
            CLEAR(*self->ddrx, self->pin);
            break;
        }
    }
    //! @todo Set the bit in the pin registry (to reserve the pin).
    return self;
}

// -----------------------------------------------------------------------------
void gpio_deinit(gpio_t* self)
{
    //! @todo Replace with destructor function.
    
    if (NULL == self) { return; }

    // Clear/reset the registers after usage.
    CLEAR(*self->ddrx, self->pin);
    CLEAR(*self->portx, self->pin);

    self->ddrx  = NULL;
    self->portx = NULL;
    self->pinx  = NULL;

    //! @todo Clear the bit in the pin registry (to release the pin).
}

// -----------------------------------------------------------------------------
void gpio_write(gpio_t* self, const bool state)
{
    // Terminate the function is the GPIO is invalid.
    if (NULL == self) { return; }

    // Check data direction, terminate the function if not configured as output.
    if (!READ(*(self->ddrx), self->pin)) { return; }

    // Check the desired state, set/clear the bit in PORTx.
    if (state) { SET(*self->portx, self->pin); }
    else { CLEAR(*self->portx, self->pin); }
}

// -----------------------------------------------------------------------------
bool gpio_read(const gpio_t* self)
{
    // Read PINx if the GPIO is valid, otherwise return false.
    return NULL != self ? READ(*self->pinx, self->pin) : false;
}
