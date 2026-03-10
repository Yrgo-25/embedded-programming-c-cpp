# Bilaga B

## Interfaces i C++
Ett interface i C++ är en abstrakt basklass, som (normalt sett) endast innehåller rent virtuella metoder, dvs. metoder som saknar implementation. Syftet med ett interface är att definiera en gemensam uppsättning metoder som olika klasser kan implementera på sitt eget sätt utefter behov. 

### Fördelar med interfaces
Användning av interfaces har ett flertal fördelar:
* **Abstraktion:** Man kan skriva kod som arbetar mot en abstraktion, inte mot en konkret klass. Detta gör det lättare att byta ut implementationer utan att ändra resten av koden.
* **Utbytbarhet:** Olika implementationer kan användas beroende på behov, t.ex. olika hårdvarudrivrutiner eller testklasser.
* **Återanvändbarhet:** Kod som använder interfaces blir mer generell och kan återanvändas i olika sammanhang.
* **Testbarhet:** Genom att använda interfaces kan du enkelt skapa mock- eller fake-klasser för enhetstester.

### Struktur på interface
Nedan visas strukturen för ett interface för timerkretsar. Detta interface har döpts till `driver::timer::Interface`. För att hålla koden kompakt används inga kommentarer i detta exempel:

```cpp
namespace driver::timer
{
class Interface
{
public:
    virtual ~Interface() noexcept = default;
    virtual void start() noexcept = 0;
    virtual void stop() noexcept = 0;
    virtual bool isRunning() const noexcept = 0;
    virtual void reset() noexcept = 0;
};
} // namespace driver::timer
```

Via ovanstående interface kan man starta, stoppa och nollställa en timer via metoder `start()`, `stop()` samt `reset()`. Man kan också kontrollera ifall timern är aktiverad eller inte via metoden `isRunning()`.

Några saker att notera:
* Destruktorn är markerad som `virtual`:
    * Detta säkerställer att rätt destruktor anropas när en instans av en subklass raderas via en pekare eller referens till interfacet. 
    * Detta är avgörande för att undvika minnesläckor och för att korrekt städa upp resurser i både underklass och basklass.
* Destruktorn är definierad som `= default`:
    * Detta medför en tom, automatisk implementation:
    * Det är god praxis att deklarera destruktorn som virtuell i ett interface. Annars kan objekt av underklasser som raderas via en pekare eller referens till interfacet få odefinierat beteende.
    * Eftersom interfacet inte har något att städa upp, räcker det här med en default-implementation.
* Alla metoder som ska överlagras i underklasserna markeras med `virtual` och avslutas med `= 0`:
    * Detta gör dem till rent virtuella metoder utan implementation i interfacet.
* Samtliga virtuella metoder är här märkta `noexcept`:
    * Detta tvingar samtliga skarpa implementationer i subklasserna att inte kunna kasta undantag.
    * Detta är fördelaktigt i ett inbyggt system, men vill man inte tvinga på detta är det okej att skippa `noexcept` här.

---

### Struktur på konkret subklass
Nedan visas ett exempel på en konkret subklass `driver::timer::Atmega328p`, som utgör en skarp implementation för timerkretsar för mikrodator ATmega328p:

```cpp
namespace driver::timer
{
class Atmega328p final : public Interface
{
public:
    // Example constructor.
    explicit Atmega328p(std::uint16_t duration_ms) noexcept;

    // Overridden methods.
    ~Atmega328p() noexcept override;
    void start() noexcept override;
    void stop() noexcept override;
    bool isRunning() const noexcept override;
    void reset() noexcept override;

    // Additional ATmega328P-specific methods (if any).

    // Deleted constructors and assignment operators.
    Atmega328p()                             = delete;
    Atmega328p(const Atmega328p&)            = delete;
    Atmega328p(Atmega328p&&)                 = delete;
    Atmega328p& operator=(const Atmega328p&) = delete;
    Atmega328p& operator=(Atmega328p&&)      = delete;

private:
    // Timer implementation details.
};
} // namespace driver::timer
```

Några saker vi såg tidigare i avsnittet om arv dyker upp även här:
* Nyckelordet `final` används för att säkerhetställa att klassen `driver::timer::Atmega328p` inte kan ärvas; detta är den slutgiltiga implementationen:
    * Detta är som tumregel bra att göra om klassen ej ska ärvas. 
    * Ibland kan det dock vara önskvärt att kunna ärva implementationen för test och då får detta
nyckelord slopas.
* Klassen `driver::timer::Atmega328p` ärver klassen `driver::timer::Interface` genom att vi skriver `: public Interface` efter klassens namn. Att vi skriver `public` innebär att allt som är publikt (eller skyddat) i interfacet också är publikt (eller skyddat) i denna underklass.

Några nya saker att notera:
* De överlagrade metoderna markeras med `override` just för att markera att detta är skarpa implementationer av
överlagrade virtuella metoder. 
* Även destruktorn markeras också med `override` för att tydligt visa att den ersätter den virtuella destruktorn i interfacet. Detta säkerställer att rätt destruktor anropas vid polymorfisk destruktion och hjälper kompilatorn att upptäcka eventuella misstag i signaturen.
* Konstruktorn har inget med interfacet att höra och är varken markerad `virtual` eller `override`.

### Exempel på användning av interfaces
Nedan visas ett interface `driver::led::Interface`. Detta interface ska kunna användas för
diverse lysdioder, exempelvis från olika mikroprocessorer:

```cpp
/**
 * @brief LED driver interface.
 */
#pragma once

#include <cstdint>

namespace driver::led
{
/**
 * @brief LED driver interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Get the pin the LED is connected to.
     * 
     * @return The pin the LED is connected to.
     */
    virtual std::uint8_t pin() const noexcept = 0;

    /**
     * @brief Check whether the LED is enabled.
     * 
     * @return True if the LED is enabled, false otherwise.
     */
    virtual bool isEnabled() const noexcept = 0;

    /**
     * @brief Enable/disable the LED.
     * 
     * @param[in] enable True if the LED is to be enabled, false otherwise.
     */
    virtual void setEnabled(bool enable) noexcept = 0;

    /**
     * @brief Toggle the LED.
     */
    virtual void toggle() noexcept = 0;
};
} // namespace driver::led
```

Nedan visas underklassen `driver::led::Atmega328p`, som ärver interfacet `driver::led::Interface` och implementeras för att enkelt styra lysdioder anslutna till mikroprocessor ATmega328P:

```cpp
namespace driver::led
{
/**
 * @brief LED driver for ATmega328P.
 * 
 *        This class is non-copyable and non-movable.
 */
class Atmega328p final : public Interface
{
public:
    /**
     * @brief Constructor.
     * 
     * @param[in] pin The pin the LED is connected to.
     */
    explicit Atmega328p(std::uint8_t pin) noexcept;

    /**
     * @brief Destructor.
     */
    ~Atmega328p() noexcept override;

    /**
     * @brief Get the pin the LED is connected to.
     * 
     * @return The pin the LED is connected to.
     */
    std::uint8_t pin() const noexcept override;

    /**
     * @brief Check whether the LED is enabled.
     * 
     * @return True if the LED is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Enable/disable the LED.
     * 
     * @param[in] enable True if the LED is to be enabled, false otherwise.
     */
    void setEnabled(bool enable) noexcept override;

    /**
     * @brief Toggle the LED.
     */
    void toggle() noexcept override;

    Atmega328p()                             = delete; // No default constructor.
    Atmega328p(const Atmega328p&)            = delete; // No copy constructor.
    Atmega328p(Atmega328p&&)                 = delete; // No move constructor.
    Atmega328p& operator=(const Atmega328p&) = delete; // No copy assignment.
    Atmega328p& operator=(Atmega328p&&)      = delete; // No move assignment.

private:
    /** The pin the LED is connected to. */
    std::uint8_t myPin;

    /** Indicate whether the LED is enabled. */
    bool myIsEnabled;
};
} // namespace driver::led
```

Genom att använda pekare eller referenser till `driver::led::Interface` kan man skriva kod som fungerar med alla underklasser. Som exempel, nedan demonstreras en funktion döpt `blinkLed()`, som används för att blinka en given LED. 

Notera att: 
* Ingående argument `led` utgörs av en referens till ett `driver::led::Interface`.
* Därmed kan lysdioden utgöras av en instans en godtycklig underklass.
* Exempelvis kan detta vara en instans av den tidigare demonstrerade klassen `driver::led::Atmega328p`, alternativt kan det vara en instans av klass för en lysdiod på en ESP32-S3-processor eller dylikt.

```cpp
/**
 * @brief Blink the given LED.
 * 
 * @param[in] led The LED to blink.
 * @param[in] blinkTimeMs The blink time in milliseconds.
 */
void blinkLed(driver::led::Interface& led, const std::uint16_t blinkTimeMs) noexcept
{
    // Toggle the LED, then delay the calling thread.
    led.toggle();
    std::this_thread::sleep_for(std::chrono::milliseconds(blinkTimeMs));
    
    // Toggle the LED again, then delay the calling thread.
    led.toggle();
    std::this_thread::sleep_for(std::chrono::milliseconds(blinkTimeMs));
}
```

När funktionen ovan anropas kan vi använda en av underklasserna direkt, exempelvis vår klass
`driver::led::Atmega328p`. Antag att vi har implementerat en lysdiod ansluten till pin 9 på en Arduino Uno  via en instans döpt `led1`:

```cpp
driver::led::Atmega328p led1{9U};
```

Vi kan blinka denna lysdiod med en blinktid på exempelvis `1000 ms` genom att kalla på funktionen `blinkLed()`. Eftersom `driver::led::Atmega328p` utgör en underklass till `driver::led::Interface` kan vi passera `led1` direkt:

```cpp
blinkLed(led1, 1000U);
```

Anta att vi också har skapat en underklass `driver::led::Esp32s3` för att implementera lysdioder för en ESP32-S3-processor, såsom visas nedan. 

Notera att:
* Implementationen är lite annorlunda denna gång; framförallt har användaren möjlighet att sätta lysdiodens startvärde direkt via anrop av konstruktorn.
* Denna klass har också en privat metod döpt `init()`. 
* Detta demonstreras för att visa att underklasserna kan skräddarsys efter behov:

```cpp
namespace driver::led
{
/**
 * @brief LED driver for ESP32-S3.
 * 
 *        This class is non-copyable and non-movable.
 */
class Esp32s3 final : public Interface
{
public:
    /**
     * @brief Constructor.
     * 
     * @param[in] pin The pin the LED is connected to.
     * @param[in] initialState Initial state of the LED (default = off).
     */
    explicit Esp32s3(std::uint8_t pin, bool initialState = false) noexcept;

    /**
     * @brief Destructor.
     */
    ~Esp32s3() noexcept override;

    /**
     * @brief Get the pin the LED is connected to.
     * 
     * @return The pin the LED is connected to.
     */
    std::uint8_t pin() const noexcept override;

    /**
     * @brief Check whether the LED is enabled.
     * 
     * @return True if the LED is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Enable/disable the LED.
     * 
     * @param[in] enable True if the LED is to be enabled, false otherwise.
     */
    void setEnabled(bool enable) noexcept override;

    /**
     * @brief Toggle the LED.
     */
    void toggle() noexcept override;

    Esp32s3()                          = delete; // No default constructor.
    Esp32s3(const Esp32s3&)            = delete; // No copy constructor.
    Esp32s3(Esp32s3&&)                 = delete; // No move constructor.
    Esp32s3& operator=(const Esp32s3&) = delete; // No copy assignment.
    Esp32s3& operator=(Esp32s3&&)      = delete; // No move assignment.

private:
    /**
     * @brief Initialize the LED.
     */
    void init(bool initialState) noexcept;

    /** The pin the LED is connected to. */
    std::uint8_t myPin;

    /** Indicate whether the LED is enabled. */
    bool myIsEnabled;
};
} // namespace driver::led
```

Anta att vi har implementerat en lysdiod ansluten till pin 20 på en ESP32-S3-processor via en instans döpt `led2`. Vi tänder lysdioden direkt vid start:

```cpp
driver::led::Esp32s3 led2{20U, true};
```

Vi kan även blinka denna lysdiod via anrop av funktionen `blinkLed()`, eftersom klassen `driver::led::esp32s3` utgör en underklass till `driver::led::Interface`. Som exempel, för att blinka denna lysdiod var 500:e millisekund kan följande anrop göras:

```cpp
blinkLed(led2, 500U);
```

Se det fullständiga exemplet [här](../examples/cpp_interface/README.md).

Se även följande [C-implementation](../examples/c_interface/README.md) för att förstå hur interfaces fungerar "under huven".

---
