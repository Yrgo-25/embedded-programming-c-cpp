# Bilaga B

## Övningsuppgifter - Timer-driver
I denna övningsuppgift ska du skapa en klass `Timer` i namnrymden `driver`.

Klassen ska delas upp i flera filer. Skapa därför följande filer i samma projekt som föregående:

```text
include/driver/timer.h
source/driver/timer.cpp
```

**OBS!**
* Alla metoddeklarationer ska skrivas i `driver/timer.h`.
* Alla metoddefinitioner ska implementeras i `driver/timer.cpp`.

I headerfilen ska klassen deklareras enligt strukturen nedan:

```cpp
#pragma once

#include <cstdint>

namespace driver
{
class Timer final
{
public:

private:
};
} // namespace driver
```

#### Uppgifter
**a)** Lägg till tre privata medlemsvariabler:
* Den första medlemsvariabeln ska:
    * Heta `myTimeout_ms`.
    * Representera timeout-tiden i millisekunder.
    * Ha typen `std::uint32_t`.
    * Endast vara läsbar efter initiering.
* Den andra medlemsvariabeln ska:
    * Heta `myCounter_ms`.
    * Representera timerns interna räknare.
    * Ha typen `std::uint32_t`.
* Den tredje medlemsvariabeln ska:
    * Heta `myRunning`.
    * Representera om timern är aktiv eller inte (`true/false`).
    * Vara `true` när timern är igång.

**b)** Lägg till en konstruktor i headerfilen som:
* Tar emot ett timeout-värde i millisekunder (`std::uint32_t`).
* Använder en initieringslista för att initiera medlemsvariablerna.
* Initierar räknaren till `0`.
* Initierar timern till stoppad.
* Markeras `explicit` samt `noexcept`.

**c)** Lägg till en destruktor i headerfilen:
* Markera denna `noexcept`.
* Implementera denna i `driver/timer.cpp` så att:
    * Timern stoppas om den var påslagen.
    * Om timern stoppas ska nedanstående utskrift skrivas ut:

```text
Stopping timer before deletion!
```

**d)** Deklarera följande metoder i headerfilen och implementera dem i `driver/timer.cpp`:
* Metoden `start()` ska:
    * Starta timern.
    * Inte ta några parametrar.
    * Inte returnera något värde.
    * Markeras `noexcept`.
* Metoden `stop()` ska:
    * Stoppa timern.
    * Inte ta några parametrar.
    * Inte returnera något värde.
    * Markeras `noexcept`.
* Metoden `tick()` ska:
    * Simulera att 1 millisekund har passerat.
    * Inkrementera räknaren om timern är igång, annars inte göra någonting.
    * Inte returnera något värde.
    * Markeras `noexcept`.
* Metoden `timeout()` ska:
    * Returnera `true` när räknaren har nått timeout-värdet, annars `false`.
    * Återställa räknaren till `0` när timeout inträffar.
    * Markeras `noexcept`.

**e)** Radera följande metoder:
* Default-konstruktorn.
* Kopieringskonstruktorn.
* Flyttkonstruktorn.
* Kopieringstilldelningsoperatorn.
* Flytttilldelningsoperatorn.

**f)** I `main.cpp`, skapa en instans av klassen:

```cpp
driver::Timer timer{1000U};
```

Testa klassen genom att:
* Starta timern.
* Köra en loop i 3000 iterationer:
    * I varje iteration:
        * Anropa `tick()`.
        * Kontrollera `timeout()`.
        * Skriv ut ett meddelande om timeout har inträffat:

```text
Timeout after 1000 ms!
```

**OBS!** I ovanstående exempelutskrift antas att timeouten är satt till 1000 millisekunder.

Använd `std::printf()` från `<cstdio>` för ovanstående utskrift.

Exempel på utdata:

```text
Timeout after 1000 ms!
Timeout after 1000 ms!
Timeout after 1000 ms!
```

---
