# **P02** - Systemlogik i C++

## Syfte
Ni ska i grupper om 1-2 bygga systemlogik i C++ med tydlig separering mellan:
* **Interfaces**: Abstraktioner som beskriver vad en driver kan göra.
* **Riktiga drivers** för ATmega328p.
* **Stubbar**: Testklasser som simulerar hårdvara.

Skelettkod finns bifogat [här](./code/README.md). 

Systemlogiken ska implementeras i en klass som håller referenser till drivernas interfaces (ett design-mönster med namnet **dependency injection**).

**Viktigt:** Callbacks behövs endast i de riktiga driver-implementationerna.
I stubbarna simuleras events direkt via testkod, och därför används inga callbacks där.

Via denna systemlogik ska systemet kunna:
* Köras med riktig hårdvara (demonstreras för läraren). 
* Köras med simulerad hårdvara via stubbar (er lärare har skapat komponenttester med Google Test, som kommer testa systemlogiken).

---

## Kravspecifikation

### Översikt
Ni ska implementera och använda följande tre delar:
1. **Interfaces**:
    * `driver::gpio::Interface`: Interface för GPIO-drivers.
    * `driver::timer::Interface`: Interface för timer-drivers.
2. **Riktiga drivers**:
    * `driver::gpio::Atmega328p`: GPIO-driver för ATmega328p, är redan implementerad och ska användas.
    * `driver::timer::Atmega328p`: Timer-driver för ATmega328p, är redan implementerad och ska användas.

    **OBS!** Driver-implementationerna ska inte skrivas om från grunden. Mindre justeringar (t.ex. metodnamn) är tillåtna vid behov för att passa era interfaces.
3. **Stubbar**:
    * `driver::gpio::Stub`: GPIO-stub för simulering/testning.
    * `driver::timer::Stub`: Timer-stub för simulering/testning.

Därefter ska ni implementera en systemklass (t.ex. `system::logic::Logic`) som får in drivers via referenser i konstruktorn och som innehåller all logik (ingen logik i `main` mer än att koppla ihop objekt).

---

## Funktionellt beteende (systemlogik)
Systemet ska uppfylla detta beteende:
* En knapp togglar ett "systemläge":
  * **System ON:** En lysdiod togglas var 100:e ms.
  * **System OFF:** Lysdioden ska vara släckt (och förbli släckt).
* I testmiljö simuleras interrupt/händelser genom att knapptryckningar och timeouts simuleras via stubbarna. I komponenttester kommer sedan handle-metoderna anropas direkt för att simulera events.
* I hårdvaruläge anropas samma handle-metoder via driver-lagret (ISR → driver → handler), men systemlogiken ska vara identisk i båda fallen.

**OBS!** Själva callback-mekanismen (t.ex. ISR → callback) ligger i driver/stub. Systemlogiken behöver inte känna till hur callbacks implementeras.

**OBS!** Systemlogiken får inte känna till register, AVR-specifika headers eller pin-mapping. Allt sådant ska ligga i drivers, inte i logiken.

---

## Filstruktur (rekommenderad)
Ni får gärna använda exakt denna struktur.

```
code
├── include
│   ├── driver
│   │   ├── gpio
│   │   │   ├── atmega328p.h
│   │   │   ├── interface.h
│   │   │   └── stub.h
│   │   └── timer
│   │       ├── atmega328p.h
│   │       ├── interface.h
│   │       └── stub.h
│   ├── system
│   │   └── logic
│   │       └── logic.h
│   └── utils
│       ├── bit_operations.h
│       └── type_traits.h
│
├── source
│   ├── driver
│   │   ├── gpio
│   │   │   └── atmega328p.cpp
│   │   └── timer
│   │       └── atmega328p.cpp
│   ├── system
│   │   └── logic
│   │       └── logic.cpp
│   ├── env
│   │   └── env.cpp
│   └── main.cpp
│
├── test
│   └── logic
│       └── test.cpp
```

**OBS!** För enkelhets skull ska stubbarna implementeras helt i headerfilerna.  
Stubbarna ska vara helt plattformsoberoende och får inte inkludera hårdvaruspecifika headers (t.ex. AVR-libc).


---

## Interfaces (krav)
**OBS!** Interfacen ska motsvara den funktionalitet som används i `Atmega328p`-implementationerna. Utgå från dessa klasser när ni definierar era interfaces.

Ni ska minst ha följande två interfaces:

### `driver::gpio::Interface`
* Skrivning och läsning av state.
* Aktivering/inaktivering av interrupts.
* Kontroll av status (initiering, state och interrupt-status).

**OBS!** Event/callback för knapp kan finnas i den konkreta `ATmega328p`-drivern och i stubben, men ska inte ingå i interfacet.

Exempel i enlighet med `driver::gpio::Atmega328p` (ni får ändra namn, men behåll kärnbeteendet):

```cpp
#pragma once

namespace driver
{
namespace gpio
{
class Interface
{
public:
    virtual ~Interface() noexcept = default;
    virtual bool isInitialized() const noexcept = 0;
    virtual bool read() const noexcept = 0;
    virtual void write(bool value) noexcept = 0;
    virtual bool isPciEnabled() const noexcept = 0;
    virtual void enablePci(bool enable) noexcept = 0;
    virtual void enablePciOnPort() noexcept = 0;
};
} // namespace gpio
} // namespace driver
```

### `driver::timer::Interface`
* Start, stopp och toggling av timern.
* Kontroll om timern är igång.
* Konfigurering av timeouts.
* Kontroll om timern har löpt ut.

**OBS!** Timeout-callback ska inte ingå i interfacet.

Exempel:

```cpp
#pragma once

#include <cstdint>

namespace driver
{
namespace timer
{
class Interface
{
public:
    virtual ~Interface() noexcept = default;

    virtual bool isInitialized() const noexcept = 0;
    virtual bool isRunning() const noexcept = 0;
    virtual bool hasTimedOut() noexcept = 0;

    virtual void start() noexcept = 0;
    virtual void stop() noexcept = 0;
    virtual void toggle() noexcept = 0;
    virtual void restart() noexcept = 0;
    virtual void reset() noexcept = 0;

    virtual void setTimeout_ms(std::uint32_t ms) noexcept = 0;
    virtual std::uint32_t timeout_ms() const noexcept = 0;
};
} // namespace timer
} // namespace driver
```

---

## Callback-koppling
Callbacks ska inte ligga i interfacen, men ska finnas i:
* `driver::gpio::Atmega328p`.
* `driver::timer::Atmega328p`.

Kopplingen görs i `main` (hårdvara). I komponenttesterna anropas handle-metoderna manuellt, därför behövs ingen callback-koppling där.

* I hårdvara anropas:
    * `system::logic::Logic::handleButtonEvent()` vid button event.
    * `system::logic::Logic::handleTimerEvent()` när en timeout inträffas.

Systemlogiken arbetar fortfarande enbart mot interfacen och innehåller ingen hårdvarukod.

**OBS!** I komponenttesterna simuleras button event samt timeout, därefter anropas motsvarande handle-metoder manuellt. Därmed behövs inga callbacks i stubbarna. 

---

## Riktiga drivers (krav)

De riktiga driver-implementationerna för ATmega328p tillhandahålls i projektet och ska användas utan modifikation.

* `driver::gpio::Atmega328p` motsvarar GPIO-driver för ATmega328p.
* `driver::timer::Atmega328p` motsvarar timer-driver för ATmega328p.

**Krav:**
* Systemlogiken ska inte inkludera AVR headers.
* Endast driver-lagret får vara hårdvarunära.
* De tillhandahållna driver-klasserna ska användas som de är.

---

## Stubbar (krav)

### `driver::gpio::Stub`
Stubben ska kunna:
* Hålla interna states (t.ex. `myValue` och `myInitialized`).
* Skriva och läsa till GPIO-enheten oavsett datariktning.
* Indikera om GPIO-enheten är initierad.
* Sätta initieringstillstånd (t.ex. för att simulera att timern inte initierades korrekt).

### `driver::timer::Stub`
Stubben ska kunna:
* Hålla interna states t.ex. (`myEnabled` och `myInitialized`).
* Starta, stoppa och toggla timern.
* Indikera om timern påslagen samt om timeout har ägt rum.
* Indikera om timern är initierad.
* Sätta och läsa timeout-värden.
* Sätta initieringstillstånd (t.ex. för att simulera att GPIO-enheten inte initierades korrekt).

---

## Systemlogik (krav)
Ni ska implementera en klass (t.ex. `system::logic::Logic`) som tar in drivers via referenser:
* En knapp: `driver::gpio::Interface` (input)
* En lysdiod: `driver::gpio::Interface` (output)
* En timer: `driver::timer::Interface`

I denna klass ska metoden `run(stop)` köra en loop tills `stop == true`:
* Systemet är i övrigt interrupt-baserat, där händelser från knappar och timers signaleras via callbacks i driver-lagret som anropar motsvarande handler-metoder i systemlogiken. Loopen i metoden `run()` ska därför vara tom och endast hålla programmet igång.
* Stop-flaggan används enbart av komponent-testerna för att avsluta körningen när testet är klart.
* * I `run()` får ni vid behov lägga in en kort delay för att undvika busy-loop. Under testning bör denna delay vara avstängd, t.ex. via makrot `TEST`.

Exempel:

```cpp
#pragma once

namespace driver
{
/** GPIO interface. */
namespace gpio { class Interface; }

/** Timer interface. */
namespace timer { class Interface; }
} // namespace driver

namespace system
{
namespace logic
{
class Logic final
{
public:
    explicit Logic(driver::gpio::Interface& button, 
                   driver::gpio::Interface& led, 
                   driver::timer::Interface& timer) noexcept;
    ~Logic() noexcept;

    bool isInitialized() const noexcept;
    void run(const bool& stop) noexcept;

    void handleButtonEvent() noexcept;
    void handleTimerEvent() noexcept;

    Logic()                        = delete;
    Logic(const Logic&)            = delete;
    Logic(Logic&&)                 = delete;
    Logic& operator=(const Logic&) = delete;
    Logic& operator=(Logic&&)      = delete;

private:
    driver::gpio::Interface& myButton;
    driver::gpio::Interface& myLed;
    driver::timer::Interface& myTimer;
};
} // namespace logic
} // namespace system
```

**Krav:**
* Klassen ska hålla referenser (inte skapa drivers själv).
* Driver-typerna ska fördeklareras i headerfilen.

---

## Redovisning
Projektet redovisas för lärare under lektionstid:
* Demo i hårdvara.
* Demo i test (via Google Test med stubbar).

---

### Testfall
Ni ska kunna visa minst följande scenarion, både med hårdvara samt med era stubbar:
1. Startläge: System OFF, lysdioden släckt.
2. Knapptryck: System ON, timer startas.
3. Timer-timeout när ON: Lysdioden togglar.
4. Timer-timeout när OFF: Lysdioden togglar inte, utan förblir släckt.
5. Stäng av: System OFF och lysdioden släcks direkt.

---

## Utvärdering
1. Varför använder vi interfaces mellan systemlogik och drivers?
2. Vad är skillnaden mellan en stub och en riktig driver?
3. Varför injicerar vi drivers som referenser i konstruktorn?
4. Vad vinner vi på callbacks i stället för polling i systemlogiken?
5. Vilka delar ska vara hårdvaruberoende – och vilka ska vara hårdvaruoberoende?

---

## Bedömningskriterier

### G (Godkänd)
* Interfaces för minst GPIO + Timer.
* En stub per interface.
* Systemlogik i klass med referensinjektion.
* Visar testfall ovan.

### VG (Väl Godkänd)
* Allt för G, plus:
  * Tillägg av ytterligare en timer, tryckknapp samt lysdiod:
    * Vid nedtryckning av tryckknappen ska timern togglas. 
    * När timern är på ska den toggla motsvarande lysdiod.
    * När timern är av ska lysdioden vara släckt.
  * Tillägg av en debounce-timer, som skyddar mot knappstudsar:
    * Vid button event ska interrupt inaktiveras på tryckknapparnas I/O-port. När detta sker ska debounce-timern startas.
    * Debounce-timern ska löpa ut efter 300 ms. När detta sker ska interrupts för tryckknapparna återaktiveras.
  * Konsekvent kodstil och filorganisation.
  * Doxygen på publika headers.
  * Testbar design.

### Bidrag till kursens slutpoäng
* Betyget **G** ger 1 poäng till kurssammanställningen.
* Betyget **VG** ger 2 poäng till kurssammanställningen.

---
