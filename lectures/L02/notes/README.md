# L02 – Anteckningar

## Beskrivning
Fortsatt implementering av en GPIO-driver via en strukt med namnet `gpio_t`.
Implementationen kommer att slutföras under nästa föreläsning.

---

## Filer
* [include/driver/gpio.h](./include/driver/gpio.h) – innehåller GPIO-driverns publika interface i form av funktioner samt enumerationen `gpio_direction_t`. Strukten `gpio_t` fördeklareras i denna fil för att hålla struktens medlemsvariabler interna (strukten är därmed `opaque`).
* [source/driver/gpio.c](./source/driver/gpio.c) – innehåller implementationsdetaljer för GPIO-drivern.
* [include/driver/utils.h](./include/driver/utils.h) innehåller funktionsliknande makron för att sätta, nollställa, toggla och läsa pins i register.
* [main.c](./main.c) – innehåller ett demo där GPIO-enheter initieras och används. Ytterligare funktionalitet kommer att läggas till senare.

---
