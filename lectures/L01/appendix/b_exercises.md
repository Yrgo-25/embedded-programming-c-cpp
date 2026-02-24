
# Bilaga B

## Övningsuppgifter

**1.** Betrakta nedanstående struct `led`:

```c
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct led
{
    const uint8_t pin;
    bool state;
} led_t;

bool led_is_on(const led_t* self) 
{ 
    return NULL != self ? self->state : false; 
}

void led_on(led_t* self) 
{
    if (NULL != self) { self->state = true; }
}

void led_off(led_t* self)
{
    if (NULL != self) { self->state = false; }
}

void led_toggle(led_t* self)
{
    if (NULL != self) { self->state = !self->state; }
}
```

Svara på följande:
* Vad har strukten för syfte?
* Vad kan man göra med en lysdiod via denna strukt?
* Varför används en pekare (`led_t* self`) i funktionerna i stället för att skicka strukten direkt?
* Vad händer när man skriver:

```cpp
led_t led1 = {12U, true};
```

---

**2.** Skriv en enkel strukt `button` med:
* Alias `button_t`.
* Två attribut:
    * `pin`: Lagrar pin-numret.
    * `state`: Tryckknappens insignal i form av ett bool-värde.
* Tre associerade funktioner:
    * `button_pin()`: Returnerar tryckknappens pin-nummer.
    * `button_read()`: Returnerar tryckknappens insignal.
    * `button_set_pressed(bool)`: Simulerar knapptryckning (`true` indikerar nedtryckning).

---
