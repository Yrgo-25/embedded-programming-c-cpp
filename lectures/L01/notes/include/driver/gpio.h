/**
 * @brief GPIO driver for ATmega328p.
 */
#ifndef GPIO_H_
#define GPIO_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Enumeration of GPIO directions.
 */
typedef enum gpio_direction
{
    GPIO_DIRECTION_INPUT,        ///< GPIO input without internal pull-up resistor enabled.
    GPIO_DIRECTION_INPUT_PULLUP, ///< GPIO input with internal pull-up resistor enabled.
    GPIO_DIRECTION_OUTPUT,       ///< GPIO output.
} gpio_direction_t;

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

/**
 * @brief Initialize GPIO.
 * 
 * @param[in, out] self The GPIO to initialize.
 * @param[in] pin Arduino pin number.
 * @param[in] direction Data direction.
 * 
 * @return True if the initialization succeeded, false otherwise.
 */
bool gpio_init(gpio_t* self, uint8_t pin, gpio_direction_t direction);

/**
 * @brief Deinitialize GPIO.
 * 
 *        Release allocated resources for the GPIO.
 * 
 * @param[in, out] self Pointer to the GPIO.
 */
void gpio_deinit(gpio_t* self);

/**
 * @brief Set state of the GPIO.
 * 
 * @note This operation is only permitted for outputs.
 * 
 * @param[in, out] self Pointer to the GPIO. 
 * @param[in] state GPIO state (true = enabled, false = disabled).
 */
void gpio_write(gpio_t* self, bool state);

/**
 * @brief Read the state of the GPIO.
 * 
 * @param[in] self Pointer to the GPIO.
 * 
 * @return True if the GPIO is enabled, false otherwise.
 */
bool gpio_read(const gpio_t* self);

#endif /* GPIO_H_ */
