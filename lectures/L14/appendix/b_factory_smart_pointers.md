# Bilaga B

## Driver-factory med smarta pekare (Abstract Factory)
I denna bilaga vidareutvecklar vi factory‑exemplet från 
[föregående lektion](../../L13/appendix/factory_raw_pointers.md).
Vi ersätter råa pekare med smarta pekare och använder ett mer fullständigt **Abstract Factory‑mönster**.

Syftet är att:
* Undvika manuell minneshantering.
* Tydliggöra ägarskap.
* Göra systemet säkrare och mer robust.

---

### Vad är Abstract Factory?
**Abstract Factory** är ett designmönster där:
* En gemensam factory‑interface definierar hur objekt skapas.
* Flera konkreta factories implementerar detta interface.
* Varje factory skapar objekt för en specifik miljö.

I vårt fall:

| Factory | Vad den skapar |
|--------|----------------|
| `Esp32s3` | Riktiga hårdvarudrivers |
| `Stub` | Simulerade drivers för test |

Systemlogiken använder bara factory‑interfacet och vet inte vilken konkret factory som används.

---

### Varför byter vi till smarta pekare?
I föregående exempel med råa pekare skapade factoryn objekt med operatorn `new`:

```cpp
return new gpio::Esp32s3(pin);
```

Systemlogiken var tvungen att radera objekten manuellt med operatorn `delete`:

```cpp
delete myLed;
```

Problem:
* Lätt att glömma `delete` → minnesläcka.
* Oklart vem som äger objektet.
* Svårare att skriva säker kod.

Lösning: **`std::unique_ptr`** från headerfilen `<memory>`.

En `std::unique_ptr`:
* Representerar exklusivt ägande.
* Raderar objektet automatiskt.
* Kan inte kopieras, bara flyttas.

Om vi använder smarta pekare kan factoryn i stället skapa objekt via anrop av metoden `std::make_unique()` från `<memory>`: 

```cpp
return std::make_unique<gpio::Esp32s3>(pin);
```

`std::make_unique`:
* Skapar objektet.
* Returnerar en `std::unique_ptr`.
* Säkerhetställer säker och tydlig konstruktion.

---

### Undantag och `noexcept`
I C++ kan minnesallokering misslyckas. När vi använder `new` eller `std::make_unique()` kan ett undantag (`std::bad_alloc`) kastas om systemet inte kan allokera minne.

I inbyggda system används ofta inte undantag, eller så vill man ha ett förutsägbart beteende vid fel. Därför används ofta `noexcept` i driver- och factory-interface ändå.

Exempel:

```cpp
virtual std::unique_ptr<gpio::Interface> gpio(std::uint8_t pin) noexcept = 0;
```

Om en funktion som är markerad med `noexcept` ändå kastar ett undantag avslutas programmet direkt.

Detta ger ett tydligt och förutsägbart felbeteende. I många inbyggda system är detta önskvärt, eftersom:
* Minnesbrist ofta är ett kritiskt fel.
* Systemet ändå inte kan fortsätta säkert.

Därför används `noexcept` i våra driver- och factory-interface. Om ett minnesfel ändå inträffar kommer programmet att termineras, vilket ger ett tydligt och förutsägbart felbeteende.

---

### Exempel på implementation med abstract factory

#### Steg 1: Factory‑interface

```cpp
#pragma once

#include <cstdint>
#include <memory>

namespace driver
{
/** GPIO driver interface. */
namespace gpio { class Interface; }
} // namespace driver

namespace driver::factory
{
class Interface
{
public:
    virtual ~Interface() noexcept = default;

    virtual std::unique_ptr<gpio::Interface> gpio(std::uint8_t pin) noexcept = 0;
};
} // namespace driver::factory
```

Nu returnerar factoryn en `std::unique_ptr` i stället för en rå pekare.

---

### Steg 2: Riktig factory

```cpp
#pragma once

#include <memory>

#include "driver/factory/interface.h"
#include "driver/gpio/esp32s3.h"

namespace driver::factory
{
class Esp32s3 final : public Interface
{
public:
    Esp32s3() noexcept = default;
    ~Esp32s3() noexcept override = default;

    std::unique_ptr<gpio::Interface> gpio(std::uint8_t pin) noexcept override
    {
        return std::make_unique<gpio::Esp32s3>(pin);
    }

    Esp32s3(const Esp32s3&)            = delete;
    Esp32s3(Esp32s3&&)                 = delete;
    Esp32s3& operator=(const Esp32s3&) = delete;
    Esp32s3& operator=(Esp32s3&&)      = delete;
};
} // namespace driver::factory
```

---

#### Steg 3: Stub‑factory

```cpp
#pragma once

#include <cstdint>
#include <memory>

#include "driver/factory/interface.h"
#include "driver/gpio/stub.h"

namespace driver::factory
{
class Stub final : public Interface
{
public:
    Stub() noexcept = default;
    ~Stub() noexcept override = default;

    std::unique_ptr<gpio::Interface> gpio(std::uint8_t pin) noexcept override
    {
        (void)(pin);
        return std::make_unique<gpio::Stub>();
    }

    Stub(const Stub&)            = delete;
    Stub(Stub&&)                 = delete;
    Stub& operator=(const Stub&) = delete;
    Stub& operator=(Stub&&)      = delete;
};
} // namespace driver::factory
```

---

#### Steg 4: Systemlogik med `unique_ptr`

```cpp
#pragma once

#include <cstdint>
#include <memory>

#include "driver/factory/interface.h"

namespace system::logic
{
class Logic final
{
public:
    explicit Logic(driver::factory::Interface& factory,
                   std::uint8_t ledPin,
                   std::uint8_t buttonPin) noexcept 
        : myLed{factory.gpio(ledPin)}
        , myButton{factory.gpio(buttonPin)}
    {}

    void run(const bool& stop) noexcept
    {
        bool buttonPrev{false};

        while (!stop)
        {
            // Read the current button input.
            const bool buttonCurrent{myButton->read()};

            // Toggle the LED on pressdown (rising edge).
            if (buttonCurrent && !buttonPrev) 
            { 
                myLed->toggle(); 
            }
            // Store the current button input for next comparison.
            buttonPrev = buttonCurrent;
        }
    }

private:
    std::unique_ptr<driver::gpio::Interface> myLed;
    std::unique_ptr<driver::gpio::Interface> myButton;
};
} // namespace system::logic
```

Notera:
* Ingen destruktor behövs.
* Minnet frigörs automatiskt.

---

#### Steg 5: main
Precis som i exempelfabriken med råa pekare skapar vi en fabrik i funktionen `main` och passerar sedan denna till systemlogiken för att skapa objekt.

Nedan visas hur en factory hade kunnat användas för att köra systemlogiken på ESP32-S3 genom att
passera en instans av `driver::factory::Esp32s3` till systemlogiken, följt av att metoden `run()` anropas.

```cpp
#include <cstdint>

#include "driver/factory/esp32s3.h"
#include "system/logic/logic.h"

int main()
{
    constexpr std::uint8_t ledPin{2U};
    constexpr std::uint8_t buttonPin{3U};

    // Create system logic and initialize the system.
    driver::factory::Esp32s3 factory{};
    Logic logic{factory, ledPin, buttonPin};

    // Run the system continuously.
    bool stop{false};
    logic.run(stop);
    return 0;
}
```

Som visades i exempelfabriken med råa pekare räcker det med att byta till stubb-fabriken `driver::factory::Stub` för att köra med stubb-drivers:

```cpp
#include <cstdint>

#include "driver/factory/stub.h"
#include "system/logic/logic.h"

int main()
{
    constexpr std::uint8_t ledPin{2U};
    constexpr std::uint8_t buttonPin{3U};

    // Create system logic and initialize the system.
    driver::factory::Stub factory{};
    Logic logic{factory, ledPin, buttonPin};

    // Run the system continuously.
    bool stop{false};
    logic.run(stop);
    return 0;
}
```

---

### Sammanfattning
I denna version använder vi:
* **Interfaces** för alla drivers.
* **Abstract Factory** för att skapa rätt typ av drivers.
* **`std::unique_ptr`** för säkert ägarskap.

Fördelar:
* Ingen manuell `delete`.
* Inga minnesläckor.
* Tydligt ägarskap.
* Samma systemlogik fungerar:
  * På riktig hårdvara.
  * I simulering.

---
