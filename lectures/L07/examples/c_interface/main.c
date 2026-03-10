/**
 * @brief GPIO driver example with interfaces in C.
 */
#include "driver/gpio/atmega328p.h"
#include "driver/gpio/stub.h"

/** LED pin. */
#define LED_PIN 8U

/** Max value of the loop counter used to simulate button events. */
#define LOOP_COUNTER_MAX 60000UL

/**
 * @brief Simulate button events.
 * 
 * @param[out] button Pointer to the simulated button.
 */
void simulate_button_event(gpio_interface_t* button)
{
    static uint16_t loop_counter = 0U;

    // Simulate a button toggle every LOOP_COUNTER_MAX calls.
    if (LOOP_COUNTER_MAX <= ++loop_counter) 
    { 
        button->vptr->toggle(button); 
        loop_counter = 0U;
    }
}

/**
 * @brief Toggle an LED connected to ATmega328p at pressdown (rising edge) of a simulated button.
 * 
 * @return Unused (this example never returns).
 */
int main(void)
{
    // Create and initialize GPIO instances.
    gpio_interface_t* led    = gpio_atmega328p_new(LED_PIN, GPIO_DIRECTION_OUT);
    gpio_interface_t* button = gpio_stub_new();
    bool button_prev         = false;

    while (1) 
    {
        // Simulate button events every LOOP_COUNTER_MAX main-loop iterations.
        simulate_button_event(button);

        // Detect pressdown (rising edge).
        const bool button_current = button->vptr->read(button);
        const bool button_pressed = button_current && !button_prev;

        // Toggle the LED on button pressdown (rising edge).
        if (button_pressed) { led->vptr->toggle(led); }
        button_prev = button_current;
    }
    // Unreachable in this example (infinite loop). Shown for completeness.
    led->vptr->del(&led);
    button->vptr->del(&button);
    return 0;
}
