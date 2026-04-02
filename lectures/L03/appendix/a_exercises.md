# Bilaga A

## Övningsuppgifter
Lägg till funktionalitet i GPIO-drivern från föregående lektion.

### Toggle-funktion
Lägg till en funktion för att toggla en GPIO om detta är en utport:

```c
void gpio_toggle(gpio_t* self);
```

* Om `self` är `NULL` eller GPIO:n inte är en utport, avsluta funktionen.
* Annars, toggla lysdioden genom att skriva till motsvarande bit i `PINx` (detta togglar motsvarande bit i `PORTx`).

---

### Pinregister
Implementera ett 32-bitars pinregister för att hålla reda på vilka GPIO-pinnar som redan används:

```c
static uint32_t pin_reg = 0U;
```

* Registret ska vara gemensamt för hela GPIO-drivern.
* Varje fysisk pin får endast reserveras av en GPIO-enhet åt gången.
* För en given pin `x` gäller att bit `x` i pinregistret:
    * Ettställs när pinnen reserveras.
    * Nollställs när pinnen frigörs.
* `gpio_new()` ska kontrollera om pinnen redan är reserverad innan en ny GPIO-enhet skapas.
    * Om pinnen redan används ska `gpio_new()` returnera en nullpekare.
    * När en GPIO-enhet skapas korrekt ska motsvarande pin markeras som reserverad i registret.

#### Tips
Använd en funktion `is_pin_free()` för att kontrollera om en given pin är fri:

```c
static inline bool is_pin_free(const uint8_t pin);
```

Denna funktion bör kontrollera pin-numret:
* Om pin-numret är giltigt (`PIN_COUNT > pin`) ska `true` retureras om motsvarande bit i pinregistret är noll, annars `false`.
* Om pin-numret inte är giltigt (`PIN_COUNT <= pin`) ska `false` returneras.
* Detta kan åstadkommas med villkorsoperatorn i kombination med den makroliknande funktionen `READ()`:

```c
return PIN_COUNT > pin ? !READ(pin_reg, pin) : false;
```

För att reservera respektive frigöra en pin kan makroliknande funktioner `SET()` samt `CLEAR()` användas:

```c
SET(pin_reg, pin);   // Reserverar pinnen.
CLEAR(pin_reg, pin); // Frigör pinnen.
```

---

### Destruktor
Implementera funktionen `gpio_delete()` så att denna frigör resurser för en GPIO-enhet:

```c
/**
 * @brief Delete GPIO instance.
 * 
 *        Release resources allocated for the GPIO.
 *        Set the associated pointer to null.
 * 
 * @param[in, out] self Reference to the GPIO instance.
 */
void gpio_delete(gpio_t** self);
```

* En dubbelpekare ska skickas så att allokerade resuser frigörs, samtidigt som pekaren till GPIO-instansen i sig sätts till `NULL`.
* Funktionen ska hantera ogiltiga pekare:
    * Om `self == NULL` eller `*self == NULL` ska funktionen avslutas utan att göra något.
* Hårdvara associerad med GPIO-instansen ska återställas till startläget:
    * Associerade bitar i GPIO-registren (DDRx, PORTx och eventuellt andra relevanta register) ska nollställas.
    * Om GPIO-enheten har avbrott aktiverade ska dessa avaktiveras.
* Pinnen ska avregistreras från pin registry så att den kan användas igen.
* Funktionen ska frigöra dynamiskt allokerat minne.
* Pekaren som skickas in ska sättas till `NULL` efter att enheten har tagits bort.

---

### Callback
* En callback kan läggas till när en GPIO-enhet skapas vid anrop av `gpio_new()`.
* En callback ska lagras per I/O-port (`PORTB`, `PORTC` samt `PORTD`):
    * Varje port har maximalt en aktiv callback.
    * Om en ny callback anges för samma port ska den skriva över den gamla.
    * Om en nullpekare skickas ska ingen callback ändras.
* Avbrott kan aktiveras för en given GPIO-enhet via en ny funktion med namnet `gpio_enable_pci()`.
* Man ska kunna inaktivera avbrott för en given GPIO-enhet eller hela I/O-porten via en ny funktion med namnet `gpio_disable_pci()`:
    * Ett ingående argument `bool disable_port` ska indikera om avbrott ska inaktiveras på hela porten eller bara på aktuell pin.

Nedanstående kodsnutt demonstrerar användning av callbacks. Denna kodsnutt ska fungera efter att implementationen är färdig:

```c
#include "driver/gpio.h"

#define BUTTON_PIN 13U // Button pin (PORTB5).
#define LED_PIN    8U  // LED pin (PORTB0).

/** Create GPIO devices with file global access. */
static gpio *button, *led;

/**
 * @brief Button callback invoked on button event.
 */
static void button_event(void)
{
    // Toggle the LED if the button is pressed (rising edge).
    if (gpio_read(button)) { gpio_toggle(led); }
}

int main(void)
{
    // Connect the button to pin 13, use `button_event()` as callback.
    button = gpio_new(BUTTON_PIN, GPIO_DIRECTION_INPUT, button_event);

    // Connect the LED to pin 8, don't use a callback.
    led = gpio_new(LED_PIN, GPIO_DIRECTION_OUT, NULL);

    // Enable button interrupts - the callback will be invoked on button event.
    gpio_enable_pci(button);

    // Keep the program running.
    while (1) {}
    return 0;
}
```

---
