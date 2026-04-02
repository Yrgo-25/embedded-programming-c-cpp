# L02 – Anteckningar

## Beskrivning
Fortsatt implementering av en GPIO-driver via en strukt med namnet `gpio_t`.
Implementationen kommer att slutföras under nästa föreläsning.

---

## Filer
* [driver/gpio.h](./include/driver/gpio.h):
    * Innehåller GPIO-driverns publika interface i form av funktioner samt enumerationen `gpio_direction_t`.
    * Strukturen `gpio_t` är fördeklarerad i denna fil för att hålla medlemsvariablerna interna (strukturen är därmed `opaque`).
* [driver/gpio.c](./source/driver/gpio.c):
    * Innehåller implementationsdetaljer för GPIO-drivern, såsom funktionsdefinitioner samt definition av strukturen `gpio`.
* [driver/utils.h](./include/driver/utils.h):
    * Innehåller funktionsliknande makron för att sätta, nollställa, toggla och läsa pinnar i register.
* [main.c](./main.c):
    * Innehåller ett demonstrationsprogram där GPIO-enheter initieras och används.
    * Ytterligare funktionalitet, såsom callbacks, kommer läggas till senare.


---
