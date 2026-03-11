# Bilaga A

## En enkel driver-factory med råa pekare
Denna bilaga visar ett förenklat exempel på hur en factory kan användas
för att skapa drivers i ett inbyggt system.

Syftet är att visa:
* Vad en factory är.
* Varför vi använder den.
* Hur den gör systemlogiken oberoende av hårdvara.

---

### Råa pekare och dynamisk minnesallokering
I detta exempel används så kallade råa pekare för att tydliggöra vad som händer. Råa pekare får vi när vi använder operatorerna `new` och `delete` för att allokera respektive frigöra objekt:
* Dessa operatorer fyller en liknande funktion som `malloc()` och `free()` i C,
men i C++ anropar `new` även objektets konstruktor och `delete` dess destruktor.
* Precis som när vi använder `malloc()` och `free()` är vi själva ansvariga för att frigöra allokerade resurser. 
* I modern C++ används därmed i stället smarta pekare, som medför att resurserna frigörs via pekarens destruktor om denna går ur scope, vilket förhindrar minnesläckor. 
* Smarta pekare kommer behandlas i nästa lektion, där vi ersätter dessa råa pekare med `std::unique_ptr`.

---

### Bakgrund
Anta att vi har implementerat följande drivers i form av klasser:
* GPIO:
    * `driver::gpio::Interface` med följande konkreta implementationer:
        * `driver::gpio::Atmega328p`: GPIO-driver för mikroprocessor ATmega328p.
        * `driver::gpio::Esp32s3`: GPIO-driver för mikroprocessor ESP32-S3.
        * `driver::gpio::Stub`: GPIO stubb-driver.

Anta att vi har skapat följande klass `system::logic::Logic` för att hantera system-logiken:
* Systemlogiken använder sig av en lysdiod och en knapp, där lysdioden togglas via stigande flank på tryckknappen.
* Interfaces används så att använd hårdvara kan väljas generiskt, dvs. de konkreta typerna väljs av användaren.
* Metoden `run()` används för att köra systemlogiken kontinuerligt.

```cpp
namespace system::logic
{
class Logic final
{
public:
    explicit Logic(driver::gpio::Interface& led,
                   driver::gpio::Interface& button) noexcept
        : myLed{led}
        , myButton{button}
    {}

    ~Logic() noexcept 
    { 
        myLed.write(false); 
    }

    void run() noexcept
    {
        bool buttonPrev{false};

        while (1)
        {
            const bool buttonCurrent{myButton.read()};
            if (buttonCurrent && !buttonPrev) 
            { 
                myLed.toggle(); 
            }
            buttonPrev = buttonCurrent;
        }
    }

    Logic()                        = delete;
    Logic(const Logic&)            = delete;
    Logic(Logic&&)                 = delete;
    Logic& operator=(const Logic&) = delete;
    Logic& operator=(Logic&&)      = delete;

private:
    driver::gpio::Interface& myLed;
    driver::gpio::Interface& myButton;
};
} // namespace system::logic
```

Ovanstående exempel kan implementeras genom att drivers av valfria konkreta typer skapas innan systemlogiken skapas, såsom visas nedan:

```cpp
int main()
{
    constexpr std::uint8_t ledPin{2U};
    constexpr std::uint8_t buttonPin{3U};

    // Create ESP32-S3 drivers (ignoring data direction for simplicity).
    driver::gpio::Esp32s3 led{ledPin};
    driver::gpio::Esp32s3 button{buttonPin};

    // Create and run system logic.
    system::logic::Logic logic{led, button};
    logic.run();
}
```

Ovanstående metod är mycket effektiv om man vill begränsa dynamisk minnesallokering.

Ofta vill man dock skapa flera drivers av samma typ, exempelvis drivers för en viss mikroprocessor. Därför är det vanligt att implementera klasser som skapar sådana instanser.
En sådan klass kallas **factory** och dess enda uppgift är att skapa objekt.

Anta att följande factories har implementerats:
* `driver::factory::Interface` med följande konkreta implementationer:
    * `driver::factory::Atmega328p`: Skapar ATmega328p-instanser.
    * `driver::factory::Esp32s3`: Skapar ESP32-S3-instanser.
    * `driver::factory::Stub`: Skapar stubb-instanser.

Anta att konstruktorn för `system::logic::Logic` hade tagit emot en referens till en valfri factory:

```cpp
explicit Logic(driver::factory::Interface& factory, 
               const std::uint8_t ledPin, 
               const std::uint8_t buttonPin) noexcept;
```

Förutsatt att medlemsvariablerna hade implementerats som pekare i stället för referenser:

```cpp
driver::gpio::Interface* myLed;
driver::gpio::Interface* myButton;
```

och factoryn hade skapat råa pekare hade instanserna kunnat skapas såsom visas nedan:

```cpp
explicit Logic(driver::factory::Interface& factory, 
               const std::uint8_t ledPin, 
               const std::uint8_t buttonPin) noexcept
        : myLed{factory.gpio(ledPin)}
        , myButton{factory.gpio(buttonPin)}
    {}
```

Som exempel hade man kunnat skapa `Esp32s3`-instanser genom att passera en
`driver::factory::Esp32s3` till systemlogiken när denna skapas:

```cpp
    constexpr std::uint8_t ledPin{2U};
    constexpr std::uint8_t buttonPin{3U};

    // Create ESP32-S3 factory.
    driver::factory::Esp32s3 factory{};

    // Create and run system logic.
    system::logic::Logic logic{factory, ledPin, buttonPin};
    logic.run();
```

Detta gör att skapandet av drivers flyttas bort från systemlogiken.
I stället ansvarar factoryn för att välja och skapa rätt konkret implementation.
Systemlogiken kan därmed arbeta mot interfaces utan att känna till den underliggande hårdvaran.

---

### Dynamisk minnesallokering i factory
När instanser skapas på detta sätt behöver de normalt allokeras dynamiskt,
så länge factoryn inte i stället returnerar en referens till en singleton-klass. 

Detta kan antingen göras med:
* Råa pekare (`new` samt `delete`).
* Smarta pekare (`std::make_unique()`).

I stället för att själv allokera en instans av `driver::gpio::Esp32s3` dynamiskt:

```cpp
auto* gpio = new driver::gpio::Esp32s3(pin);
```

låter vi en factory göra detta:

```cpp
auto* gpio = factory.gpio(pin);
```

Detta innebär att:
* Systemlogiken vet inte vilken konkret klass som används.
* Den vet bara att den får ett objekt som uppfyller ett visst interface.

---

### Varför använder vi factory?
I många inbyggda system fyller en factory en liknande roll som ett hårdvaruabstraktionslager (HAL) eller ett board support package (BSP); den centraliserar skapandet av hårdvaruspecifika objekt.

I ett inbyggt system vill vi ofta kunna:

| Situation | Vad vi vill göra |
|----------|------------------|
| Riktig hårdvara | Använda riktiga drivers |
| Test/simulering | Använda stubbar |
| Ny plattform | Byta drivers |

Om systemlogiken själv skapar drivers:

```cpp
auto* led = new driver::gpio::Esp32s3(pin);
```

då:
* Är logiken låst till ESP32-S3.
* Går systemet inte att testa utan hårdvara.

Med factory:

```cpp
auto* led = factory.gpio(pin);
```

kan vi byta mellan:

```cpp
driver::factory::Esp32s3 factory{};
```

och:

```cpp
driver::factory::Stub factory{};
```

utan att ändra systemlogiken.

Detta kallas *dependency injection via factory*.

---

### Översikt av arkitekturen i ett vanligt inbyggt system
Anta att vi har ett inbyggt system, där systemet delas upp i fyra lager:

```
systemlogik
    ↑
driver interfaces
    ↑
factory
    ↑
riktiga drivers / stubbar
```

Systemlogiken vet alltså inte om den körs mot riktig hårdvara eller mot stubbar.
Det enda den känner till är interfaces.

---

### Exempel på implementation med factory

#### Steg 1: Interface för GPIO
Vi skapar ett enkelt GPIO-interface i en fil `driver/gpio/interface.h`, såsom visas nedan:

```cpp
#pragma once

namespace driver::gpio
{
class Interface
{
public:
    virtual ~Interface() noexcept = default;
    virtual bool read() const noexcept = 0;
    virtual void write(bool state) noexcept = 0;
    virtual void toggle() noexcept = 0;
};
} // namespace driver::gpio
```

---

#### Steg 2: Riktig driver
Vi skapar en GPIO-driver för ESP32-S3 i en fil `driver/gpio/esp32s3.h`, såsom visas nedan:

```cpp
#pragma once

#include <cstdint>

#include "driver/gpio/interface.h"

namespace driver::gpio
{
class Esp32s3 final : public Interface
{
public:
    explicit Esp32s3(const std::uint8_t pin) noexcept
        : myPin{pin}
    {
        // ESP-specifik initiering här!
    }

    ~Esp32s3() noexcept override
    {
        // ESP-specifik cleanup här!
    }

    bool read() const noexcept override
    {
        // ESP-specifik kod här!
        return false;
    }

    void write(const bool state) noexcept override
    {
        // ESP-specifik kod här!
        (void) (state);
    }

    void toggle() noexcept override
    {
        // ESP-specifik kod här!
    }

    Esp32s3()                          = delete;
    Esp32s3(const Esp32s3&)            = delete;
    Esp32s3(Esp32s3&&)                 = delete;
    Esp32s3& operator=(const Esp32s3&) = delete;
    Esp32s3& operator=(Esp32s3&&)      = delete;

private:
    const std::uint8_t myPin;
};
} // namespace driver::gpio
```

---

#### Steg 3: Stub
Vi skapar en GPIO-stubb i en fil `driver/gpio/stub.h`, såsom visas nedan:

```cpp
#pragma once

#include <cstdint>

#include "driver/gpio/interface.h"

namespace driver::gpio
{
class Stub final : public Interface
{
public:
    Stub() noexcept
        : myState{false}
    {}
    ~Stub() noexcept = default;

    bool read() const noexcept override { return myState; }
    void write(const bool state) noexcept override { myState = state; }
    void toggle() noexcept override { myState = !myState; }

    Stub(const Stub&)            = delete;
    Stub(Stub&&)                 = delete;
    Stub& operator=(const Stub&) = delete;
    Stub& operator=(Stub&&)      = delete;

private:
    bool myState;
};
} // namespace driver::gpio
```

---

#### Steg 4: Factory-interface
Vi skapar ett factory-interface i en fil `driver/factory/interface.h` för att kunna implementera separata factories:
* En factory för att skapa instanser för ESP32-S3.
* En factory för att skapa stubb-instanser.

```cpp
#pragma once

#include <cstdint>

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
    virtual gpio::Interface* gpio(std::uint8_t pin) noexcept = 0;
};
} // namespace driver::factory
```

---

#### Steg 5: Riktig factory
Vi skapar sedan en ESP32-S3 factory i en fil `driver/factory/esp32s3.h` för att kunna konstruera ESP32-instanser:

```cpp
#pragma once

#include <cstdint>

#include "driver/factory/interface.h"
#include "driver/gpio/esp32s3.h"

namespace driver::factory
{
class Esp32s3 final : public Interface
{
public:
    Esp32s3() noexcept = default;
    ~Esp32s3() noexcept override = default;

    gpio::Interface* gpio(const std::uint8_t pin) noexcept override 
    { 
        return new gpio::Esp32s3(pin);
    }

    Esp32s3(const Esp32s3&)            = delete;
    Esp32s3(Esp32s3&&)                 = delete;
    Esp32s3& operator=(const Esp32s3&) = delete;
    Esp32s3& operator=(Esp32s3&&)      = delete;
};
} // namespace driver::factory
```

---

#### Steg 6: Stub-factory
Vi skapar också en stubb-factory i en fil `driver/factory/stub.h` för att kunna konstruera stubb-instanser:

```cpp
#pragma once

#include <cstdint>

#include "driver/factory/interface.h"
#include "driver/gpio/stub.h"

namespace driver::factory
{
class Stub final : public Interface
{
public:
    Stub() noexcept = default;
    ~Stub() noexcept override = default;

    gpio::Interface* gpio(const std::uint8_t pin) noexcept override
    {
        // Ignorerar pin-numret, eftersom det inte används av stubben.
        (void) (pin);
        return new gpio::Stub();
    }

    Stub(const Stub&)            = delete;
    Stub(Stub&&)                 = delete;
    Stub& operator=(const Stub&) = delete;
    Stub& operator=(Stub&&)      = delete;
};
} // namespace driver::factory
```

---

#### Steg 7: Systemlogik som använder factory
Vi skapar sedan en logikklass, som använder sig av en given factory för att skapa instanser,
i en fil `system/logic/logic.h`:
* I konstruktorn allokerar vi minne för GPIO-instanserna `myLed` samt `myButton` via factoryn.
* I destruktorn frigör vi allokerade resurser, dvs. GPIO-instanserna - eftersom vi har allokerat minne via råa pekare är vi själva ansvariga för att genomföra detta.
* I metoden `run()` togglas `myLed` vid stigande flank på tryckknappen.
* Eftersom `myLed` och `myButton` är pekare används piloperatorn `->` för att anropa deras metoder.

I detta exempel är det systemlogiken som äger driver-objekten. Pekarna som returneras av factoryn ägs därför av `Logic`, vilket innebär att det också är systemlogiken som ansvarar för att radera dem i destruktorn:

```cpp
#pragma once

#include <cstdint>

#include "driver/factory/interface.h"

namespace system::logic
{
class Logic final
{
public:
    explicit Logic(driver::factory::Interface& factory,
                   const std::uint8_t ledPin, const std::uint8_t buttonPin) noexcept
        : myLed{factory.gpio(ledPin)}
        , myButton{factory.gpio(buttonPin)}
    {}

    ~Logic() noexcept
    {
        delete myLed;
        delete myButton;
    }

    void run() noexcept
    {
        bool buttonPrev{false};

        while (1)
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
    driver::gpio::Interface* myLed;
    driver::gpio::Interface* myButton;
};
} // namespace system::logic
```

---

#### Steg 8: main
I funktionen `main` skapar vi en fabrik och passerar denna till systemlogiken för att skapa objekt.

Nedan visas hur en factory hade kunnat användas för att köra systemlogiken på ESP32-S3 genom att
passera en instans av `driver::factory::Esp32s3` till systemlogiken, följt av att metoden `run()` anropas:

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
    system::logic::Logic logic{factory, ledPin, buttonPin};

    // Run the system continuously.
    logic.run();
    return 0;
}
```

För att köra med stubb-drivers räcker det med att byta till stubb-fabriken `driver::factory::Stub`. Detta lämpar sig utmärkt för tester, vilket kommer att tydliggöras i en senare kurs inom mjuk- och hårdvarutestning:

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
    system::logic::Logic logic{factory, ledPin, buttonPin};

    // Run the system continuously.
    logic.run();
    return 0;
}
```

---
