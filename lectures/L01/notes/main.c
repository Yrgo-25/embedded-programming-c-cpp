/**
 * @brief GPIO driver demo.
 */
#include "driver/gpio.h"

/** LED pin. */
#define LED_PIN 9U

/**
 * @brief Create and initialize a LED instance.
 */
int main(void)
{
    gpio_t led;
    gpio_init(&led, LED_PIN, GPIO_DIRECTION_OUTPUT);
	
    while (1) { }
}

