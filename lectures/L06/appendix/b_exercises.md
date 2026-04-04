# Bilaga B

## Övningsuppgifter
Ni ska implementera en GPIO-driver för ATmega328p via en klass `driver::gpio::Atmega328p`:
* Klassen ska delas upp i en headerfil `driver/gpio/atmega328p.h` samt en källkodsfil `driver/gpio/atmega328p.cpp`.
* Klassen ska motsvara GPIO-drivern skriven i C under L01–L03, men utan callbacks.
* Referens:
    * [driver/gpio.h](../../L03/notes/include/driver/gpio.h).
    * [driver/gpio.c](../../L03/notes/source/driver/gpio.c).

---

### Filstruktur
Använd följande filstruktur:

```
include/
    driver/
        gpio/
            types.h
            atmega328p.h
source/
    driver/
        gpio/
            atmega328p.cpp
    main.cpp
```

---

### Enumerationsklass för datariktningar
* Datariktningar ska implementeras via en enumerationsklass `driver::gpio::Direction` i filen `driver/gpio/types.h`. Följande datariktningar ska implementeras:
    * `Input`: Inport utan intern pullup-resistor aktiverad.
    * `InputPullup`: Inport med intern pullup-resistor aktiverad.
    * `Output`: Utport.

---

### Metoder
Klassen ska inneha följande metoder:
* `Atmega328p(std::uint8_t pin, Direction direction)`: 
    * Initierar en ny GPIO-instans med given datariktning, förutsatt att givet pin-nummer är giltigt (`0 <= pin < 20`) och pinnen inte är reserverad av en annan GPIO-instans.
    * Ska markeras `explicit` samt `noexcept`.
* `~Atmega328p()`: 
    * Återställer GPIO-registrerna samt släpper reserverad pin **om instansen är initierad**.
    * Ska markeras `noexcept`.
* `isInitialized()`: 
    * Indikerar ifall GPIO-instansen är initierad via ett bool-värde, som beror på resultatet från initieringen i konstruktorn:
        * Om instansen är initierad ska `true` returneras.
        * Om instansen inte är initierad ska `false` returneras.
    * Ska markeras `const` samt `noexcept`.
* `read()`: 
    * Indikerar GPIO-instansens tillstånd via ett bool-värde.
    * Om instansen är initierad ska aktuell insignal läsas och returneras.
    * Om instansen inte är initierad ska `false` returneras.
    * Ska markeras `const` samt `noexcept`.
* `write(bool state)`:
    * Uppdaterar GPIO-instansens tillstånd om det är en utport, annars görs ingenting.
    * Ska markeras `noexcept`.
* `toggle()`: 
    * Togglar GPIO-instansens tillstånd om det är en utport, annars görs ingenting.
    * Ska markeras `const` samt `noexcept`.

---

### Medlemsvariabler
Klassen ska inneha följande medlemsvariabler:
* `myDirReg`:
    * Pekare till associerat datariktningsregister.
    * Ska ha datatypen `volatile std::uint8_t*`.
* `myPortReg`:
    * Pekare till associerat portregister.
    * Ska ha datatypen `volatile std::uint8_t*`.
* `myPinReg`: 
    * Pekare till associerat pin-register.
    * Ska ha datatypen `volatile std::uint8_t*`.
* `myPin`: 
    * Pin på associerad I/O-port.
    * Ska ha datatypen `std::uint8_t`.
* `myId`:
    * Pin ID för att reservera pins i ett internt pinregister.
    * Ska ha datatypen `std::uint8_t`.
    * Kan/bör markeras `const` om ID:t initieras direkt i konstruktorn.

### Testkod
I `main.cpp`, skapa två GPIO-instanser via klassen `driver::gpio::Atmega328p`:
* `led`: 
    * Lysdiod ansluten till pin 8.
    * Ska konfigureras som utport.
* `button`:
    * Tryckknapp ansluten till pin 13.
    * Ska konfigureras som inport med aktiverad pullup-resistor.

Skapa också en lokal variabel `bool buttonPrev`, som håller tryckknappens föregående tillstånd.

Se till att programmet körs kontinuerligt via en while-loop:
* Varje varv, kontrollera tryckknappens nuvarande insignal.
* Om tryckknappen nu är nedtryckt (insignalen är hög), men inte var det förra varvet ska lysdioden togglas.
* Tryckknappens föregående tillstånd ska uppdateras i slutet av varje varv i while-loopen (inför nästa jämförelse).

---
