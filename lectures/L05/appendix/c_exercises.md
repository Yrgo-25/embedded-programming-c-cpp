# Bilaga C

## Övningsuppgifter

---

## Övningsset 1 - Första klassen

### Övning 1.1 - Enkel LED‑klass
I ett nytt program, skapa en klass `Led` i namnrymden `driver::gpio`:

```cpp
namespace driver::gpio
{
class Led final
{
public:

private:
};
} // namespace driver::gpio
```

#### Uppgifter
**a)** Lägg till två privata medlemsvariabler:
* Den första medlemsvariabeln ska:
    * Heta `myPin`.
    * Representera pin-numret som LED:en är kopplad till.
    * Ha typen `std::uint8_t`.
    * Endast vara läsbar efter initiering.
* Den andra medlemsvariabeln ska:
    * Heta `myState`.
    * Representera LED:ens tillstånd (`true/false`).
    * Vara `true` när LED:en är aktiverad.

**b)** Lägg till en konstruktor som:
* Tar emot ett pin-nummer (`std::uint8_t`).
* Tar emot ett initialt tillstånd (`bool`) med defaultvärdet `false`.
* Använder en initieringslista för att initiera medlemsvariablerna.
* Markeras `explicit` samt `noexcept`.

**c)** Lägg till en destruktor, markera denna `noexcept` samt `default`.

**d)** Implementera följande metoder direkt i klassen:
* Metoden `isOn()` ska:
    * Returnera `true` om LED är aktiverad, annars `false`.
    * Markeras `const` samt `noexcept`.
* Metoden `on()` ska:
    * Inte ta några parametrar.
    * Sätta LED‑tillståndet till `true`.
    * Inte returnera något värde.
    * Markeras `noexcept`.
* Metoden `off()` ska:
    * Inte ta några parametrar.
    * Sätta LED‑tillståndet till `false`.
    * Inte returnera något värde.
    * Markeras `noexcept`.
* Metoden `toggle()` ska:
    * Toggla LED‑tillståndet.
    * Inte ta några parametrar.
    * Inte returnera något värde.
    * Markeras `noexcept`.

**e)** Radera följande metoder:
* Default-konstruktorn.
* Kopieringskonstruktorn.
* Flyttkonstruktorn.
* Kopieringstilldelningsoperatorn.
* Flytttilldelningsoperatorn.

**f)** Skapa en instans av klassen:

```cpp
driver::gpio::Led led{13U};
```

Testa klassen genom att:
* Skriva ut tillståndet.
* Anropa `on()`.
* Skriva ut igen.
* Anropa `toggle()`.

Använd `std::printf()` från `<cstdio>` för utskrifterna.

Exempel på utdata:

```text
Initial state: Off
After on(): On
After toggle(): Off
```

---

### Övning 1.2 - Knappklass
I samma program, skapa en klass `Button` i namnrymden `driver::gpio`:

```cpp
namespace driver::gpio
{
class Button final
{
public:

private:
};
} // namespace driver::gpio
```

#### Uppgifter
**a)** Lägg till två privata medlemsvariabler:
* Den första medlemsvariabeln ska:
    * Heta `myPin`.
    * Representera pin-numret som knappen är kopplad till.
    * Ha typen `std::uint8_t`.
    * Endast vara läsbar efter initiering.
* Den andra medlemsvariabeln ska:
    * Heta `myPressed`.
    * Representera knappens tillstånd (`true/false`).
    * Vara `true` när knappen är nedtryckt.

**b)** Lägg till en konstruktor som:
* Tar emot ett pin-nummer (`std::uint8_t`).
* Använder en initieringslista för att initiera medlemsvariablerna.
* Markeras `explicit` samt `noexcept`.

**c)** Lägg till en destruktor, markera denna `noexcept` samt `default`.

**d)** Implementera följande metoder direkt i klassen: 
* Metoden `pin()` ska:
    * Returnera pin-numret.
    * Markeras `const` samt `noexcept`.
* Metoden `isPressed()` ska:
    * Returnera `true` om knappen är nedtryckt, annars `false`.
    * Markeras `const` samt `noexcept`.
* Metoden `setPressed()` ska:
    * Ta emot ett argument av typen `bool`.
    * Uppdatera knappens interna tillstånd.
    * Inte returnera något värde.
    * Markeras `noexcept`.

**e)** Radera följande metoder:
* Default-konstruktorn.
* Kopieringskonstruktorn.
* Flyttkonstruktorn.
* Kopieringstilldelningsoperatorn.
* Flytttilldelningsoperatorn.

**f)** Skapa en instans:

```cpp
driver::gpio::Button button{2U};
```

Simulera ett knapptryck:

```cpp
button.setPressed(true);
```

Skriv ut pin-numret samt om knappen är nedtryckt med `std::printf()` från `<cstdio>`.

Exempel på utdata:

```text
Button pin: 2
Pressed: Yes
```

---

## Övningsset 2 - Samverkan mellan klasser

### Övning 2.1 - Styr LED med en knapp
Använd klasserna från tidigare övningsuppgifter.

#### Uppgifter
**a)** Skapa en LED på pin `13`.  
**b)** Skapa en knapp på pin `2`.  
**c)** Om knappen inte är nedtryckt ska LED vara släckt.  
**d)** Om knappen är nedtryckt ska LED tändas.  
**e)** Skriv ut LED‑tillståndet.  

Exempel på utdata:

```text
Button released -> LED: Off
Button pressed  -> LED: On
```

---

### Övning 2.2 - Blinkfunktion
Skapa en loop som togglar LED‑tillståndet.

#### Uppgifter
**a)** Kör en loop i sex iterationer.  
**b)** I varje iteration:
* Anropa `toggle()`.
* Skriv ut LED‑tillståndet.

Exempel på utdata:

```text
Blink 1: On
Blink 2: Off
Blink 3: On
Blink 4: Off
Blink 5: On
Blink 6: Off
```

---

## Övningsset 3 - Klass uppdelad i flera filer

### Övning 3.1 - Buzzer‑klass
I denna övningsuppgift ska du skapa en klass `Buzzer` i namnrymden `driver`.  
Klassen ska delas upp i flera filer. Skapa därför följande katalogstruktur:


```text
Makefile
include/
    driver/
        buzzer.h
source/
    driver/
        buzzer.cpp
    main.cpp
```

**OBS!**
* Alla metoddeklarationer ska skrivas i `driver/buzzer.h`.
* Alla metoddefinitioner ska implementeras i `driver/buzzer.cpp`.

---

I headerfilen `driver/buzzer.h` ska klassen deklareras enligt strukturen nedan:

```cpp
#pragma once

#include <cstdint>

namespace driver
{
class Buzzer final
{
public:

private:
};
} // namespace driver
```

#### Uppgifter
**a)** Lägg till två privata medlemsvariabler:
* Den första medlemsvariabeln ska:
    * Representera pin-numret som buzzern är kopplad till.
    * Ha typen `std::uint8_t`.
    * Heta `myPin`.
    * Endast vara läsbar efter initiering.
* Den andra medlemsvariabeln ska:
    * Representera buzzerns tillstånd (`true`/`false`).
    * Heta `myEnabled`.
    * Vara `true` när buzzern är aktiverad.

**b)** Lägg till en konstruktor i headerfilen som:
* Tar emot ett pin-nummer (`std::uint8_t`).
* Tar emot ett initialt tillstånd (`bool`) med defaultvärdet `false`.
* Markeras `explicit` samt `noexcept`.

**c)** Lägg till en destruktor i headerfilen:
* Markera denna `noexcept`.
* Implementera denna i `driver/buzzer.cpp` så att nedanstående utskrift genereras när buzzern raderas:

```text
Releasing resources allocated for buzzer at pin 8!
```

**OBS!** I ovanstående exempelutskrift antas att buzzern är ansluten till pin 8.

**d)** Deklarera följande metoder i headerfilen och implementera dem i `driver/buzzer.cpp`:
* Metoden `pin()` ska:
    * Returnera pin-numret som buzzern är kopplad till.
    * Markeras `const` samt `noexcept`.
* Metoden `isEnabled()` ska:
    * Returnera `true` om buzzern är aktiverad, annars `false`.
    * Inte ta några parametrar.
    * Markeras `const` samt `noexcept`.
* Metoden `enable()` ska:
    * Sätta buzzerns tillstånd till `true`.
    * Inte ta några parametrar.
    * Inte returnera något värde.
    * Markeras `noexcept`.
* Metoden `disable()` ska:
    * Sätta buzzerns tillstånd till `false`.
    * Inte ta några parametrar.
    * Inte returnera något värde.
    * Markeras `noexcept`.
* Metoden `toggle()` ska:
    * Toggla buzzerns tillstånd.
    * Inte ta några parametrar.
    * Inte returnera något värde.
    * Markeras `noexcept`.

**e)** Radera följande metoder i headerfilen:
* Default-konstruktorn.
* Kopieringskonstruktorn.
* Flyttkonstruktorn.
* Kopieringstilldelningsoperatorn.
* Flytttilldelningsoperatorn.

**f)** I `main.cpp`, skapa en instans av klassen:

```cpp
driver::Buzzer buzzer{8U};
```

Testa klassen genom att:
* Skriva ut tillståndet.
* Anropa `enable()`.
* Skriva ut igen.
* Anropa `toggle()`.
* Skriva ut igen.

Använd `std::printf()` från `<cstdio>` för utskrifterna.

Exempel på utdata:

```text
Initial state: Disabled
After enable(): Enabled
After toggle(): Disabled
Releasing resources allocated for buzzer at pin 8!
```

---
