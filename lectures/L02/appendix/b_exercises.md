# Bilaga B

## Övningsuppgifter
Lägg till funktionalitet i GPIO-drivern från föregående lektion så att:
* En callback kan läggas till när en GPIO-enhet skapas vid anrop av `gpio_new()`.
* En callback ska lagras per I/O-port (`PORTB`, `PORTC` samt `PORTD`):
    * Varje port har maximalt en aktiv callback.
    * Om en ny callback anges för samma port ska den skriva över den gamla.
    * Om en nullpekare skickas ska ingen callback ändras.
* Avbrott kan aktiveras för en given GPIO-enhet via en ny funktion med namnet `gpio_enable_interrupts()`.
* Man ska kunna kontrollera om avbrott är aktiverade via en funktion med namnet `gpio_is_interrupts_enabled()`.

Nedanstående kodsnutt demonstrerar användning av callbacks. Denna kodsnutt ska fungera efter
att implementationen är färdig:

```c
#include "driver/gpio.h"

#define BUTTON_PIN 13U // Button pin (PORTB5).
#define LED_PIN    8U  // LED pin (PORTB0).

/** Create GPIO devices with file global access. */
gpio *button, *led;

/**
 * @brief Button callback invoked on button event.
 */
void button_event(void)
{
    // Toggle the LED if the button is pressed (rising edge).
    if (gpio_read(button)) { gpio_toggle(led); }
}

int main(void)
{
    // Connect the button to pin 13, use `button_event()` as callback.
    button = gpio_new(BUTTON_PIN, GPIO_DIRECTION_INPUT, button_event);

    // Enable button interrupts - the callback will be invoked on button event.
    gpio_enable_interrupts(button, true);

    // Connect the LED to pin 8, don't use a callback.
    led = gpio_new(LED_PIN, GPIO_DIRECTION_OUT, NULL);

    // Keep the programming running.
    while (1)
    {
    }
    return 0;
}
```

---