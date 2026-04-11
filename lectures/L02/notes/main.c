/**
 * @brief GPIO driver demo.
 */
#include "driver/gpio.h"

/** LED pin. */
#define LED_PIN 9U

/** Button pin. */
#define BUTTON_PIN 13U

/**
 * @brief Initialize GPIO and start the application.
 */
int main(void)
{
    // Initialize GPIOs.
    gpio_t* led    = gpio_new(LED_PIN, GPIO_DIRECTION_OUTPUT);
    gpio_t* button = gpio_new(BUTTON_PIN, GPIO_DIRECTION_INPUT_PULLUP);
    
    while (1) 
    { 
        const bool state = gpio_read(button);
        gpio_write(led, state);
    }
}

