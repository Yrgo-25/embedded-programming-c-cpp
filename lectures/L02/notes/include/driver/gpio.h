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
typedef struct gpio gpio_t;

/**
 * @brief Create a new GPIO.
 * 
 * @param[in] pin Arduino pin number.
 * @param[in] direction Data direction.
 * 
 * @return The initialized GPIO on success, a nullptr otherwise.
 */
gpio_t* gpio_new(uint8_t pin, gpio_direction_t direction);

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
