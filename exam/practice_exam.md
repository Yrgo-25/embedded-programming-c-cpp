# Övningstentamen - C++-teori

## Information

### Hjälpmedel
* En A4 anteckningar.
* Dator med textredigerare (t.ex. Visual Studio Code med IntelliSense).
* Kodkomplettering, AI-verktyg och internetåtkomst är inte tillåtna.

### Poänggränser och betygsnivåer
Totalt: 24 poäng.

Betygsgränser:
* **G:** Minst 10 poäng.
* **VG:** Minst 17 poäng.

Bidrag till kursens slutpoäng:
* Betyget **G** ger 2 poäng till kurssammanställningen.
* Betyget **VG** ger 4 poäng till kurssammanställningen.

### Viktigt
* All kod ska implementeras i en headerfil, metoder definieras i klassen.
* Koden behöver ej kommenteras.

---

## G-uppgifter

### **1.** Interface (4p)
I en fil döpt `driver/gpio/interface.h`, skapa ett nytt interface `driver::gpio::Interface`, som innehåller följande virtuella metoder:
* En destruktor:
    * Ska markeras `virtual` och `default`.
* `read()`:
    * Returnerar GPIO-instansens tillstånd (högt/lågt) som `true/false`.
    * Ska markeras `const` och `noexcept`.
* `write(bool state)`:
    * Sätter GPIO-instansens tillstånd (`true` = högt).
    * Ska markeras `noexcept`.
* `toggle()`:
    * Togglar GPIO-instansens tillstånd.
    * Ska markeras `noexcept`.

**OBS!** Samtliga metoder (förutom destruktorn) ska deklareras som rent virtuella (`= 0`).

---

### **2.** Stubbklass (6p)
I en ny fil `driver/gpio/stub.h`, skapa en underklass `driver::gpio::Stub`, som ärver ovanstående interface.

Implementera denna klass till en enkel stubb, som möjliggör att man kan sätta GPIO-instansens tillstånd via en privat medlemsvariabel `myState`. Via denna variabel ska man kunna läsa, skriva och toggla GPIO-instansen.

Denna klass ska innehålla:
* En privat medlemsvariabel `myState`:
    * Sparar GPIO-instansens tillstånd (hög/låg). 
    * Ska ha datatypen `bool`.
* En default-konstruktor:
    * Initierar `myState` till `false`.
    * Ska markeras `noexcept`.
* En destruktor:
    * Ska markeras `noexcept` samt `default`.
* Överlagrade varianter av metoderna från interfacet:
    * `read()` returnerar `myState`.
    * `write(bool state)` sätter `myState = state`.
    * `toggle()` togglar `myState`.

För denna klass ska copy- och move-konstruktorerna samt motsvarande operatorer raderas:
* Kopieringskonstruktorn `Stub(const Stub&)`.
* Förflyttningskonstruktorn `Stub(Stub&&)`.
* Kopieringstilldelningsoperatorn `Stub& operator=(const Stub&)`.
* Förflyttningstilldelningsoperatorn `Stub& operator=(Stub&&)`.

**OBS!**
* De överlagrade metoderna ska markeras `override`.
* Klassen ska inte heller kunna ärvas vidare (klassen ska markeras `final`).

Använd följande testprogram för att verifiera din implementation:

```cpp
/**
 * @brief Test program 1.
 */
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <thread>

#include "driver/gpio/stub.h"

namespace
{
// -----------------------------------------------------------------------------
void delay_ms(const std::size_t ms) noexcept
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// -----------------------------------------------------------------------------
void runLogic(driver::gpio::Interface& led, const std::size_t iterationCount = 10U,
              const std::size_t blinkSpeed_ms = 100U) noexcept
{
    const std::size_t blinkCount{iterationCount * 2U};

    for (std::size_t blink{}; blink < blinkCount; ++blink)
    {
        led.toggle();
        const char* state{led.read() ? "on" : "off"};
        std::printf("LED %s!\n", state);
        delay_ms(blinkSpeed_ms);
    }
}
} // namespace

// -----------------------------------------------------------------------------
int main()
{
    driver::gpio::Stub led{};
    runLogic(led);
    return 0;
}
```

### **3.** Factory (4p)
I en fil `driver/factory/stub.h`, lägg till nedanstående tomma stubb-factory:

```cpp
namespace driver::factory 
{ 
class Stub {}; 
} // namespace driver::factory 
```

Du ska göra följande:
* Lägg till en metod `gpio(std::uint8_t pin)`, som returnerar en smart pekare till en GPIO-stubb, omvandlad till motsvarande GPIO-interface (`std::unique_ptr<driver::gpio::Interface>`). 
* Ignorera explicit angivet pin-nummer i denna implementation, eftersom stubben inte använder den.
* Inkludera headerfiler för att använda smarta pekare samt `std::uint8_t`.
* Undvik råa pekare i denna implementation; skapa unika pekare enligt modern C++-stil.

**OBS!** Konstruktorer, destruktorn och kopierings- och förflyttningssemantik kan skippas.

Använd följande testprogram för att verifiera din implementation:

```cpp
/**
 * @brief Test program 2.
 */
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <thread>

#include "driver/factory/stub.h"

namespace
{
// -----------------------------------------------------------------------------
void delay_ms(const std::size_t ms) noexcept
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// -----------------------------------------------------------------------------
void runLogic(driver::gpio::Interface& led, const std::size_t iterationCount = 10U,
              const std::size_t blinkSpeed_ms = 100U) noexcept
{
    const std::size_t blinkCount{iterationCount * 2U};

    for (std::size_t blink{}; blink < blinkCount; ++blink)
    {
        led.toggle();
        const char* state{led.read() ? "on" : "off"};
        std::printf("LED %s!\n", state);
        delay_ms(blinkSpeed_ms);
    }
}
} // namespace

// -----------------------------------------------------------------------------
int main()
{
    driver::factory::Stub factory{};
    auto led = factory.gpio(0U);
    runLogic(*led);
    return 0;
}
```

---

## VG-uppgifter

### **4.** Klasstemplate (6p)
I en fil `driver::eeprom::Stub`, skapa ett klasstemplate för ett stubb-EEPROM `driver::eeprom::Stub<std::uint16_t Size>`:
* Denna klass ska inneha följande metoder:
    * `read(std::uint16_t address)`
    * `write(std::uint16_t address, std::uint8_t data)`

För denna klass gäller följande:
* Template-parameter `Size` utgör storleken på EEPROM-minnet i byte. 
* Som default ska EEPROM-minnet sättas till `1024` byte.
* Metoderna ska markeras `noexcept`.

Denna klass ska innehålla:
* En constraint via en `static_assert` så att EEPROM-minnets storlek inte kan sättas till `0`. Om så är fallet ska felmeddelandet `EEPROM size must be greater than 0!` skrivas ut vid kompilering.
* En privat medlemsvariabel `myData`, som utgör en statiskt allokerad bytearray av angiven storlek `Size` (`std::uint8_t myData[Size]`).
* En default-konstruktor, som initierar arrayen med nollor (`{}`).
* En destruktor, som sätts till `= default`.
* `read(std::uint16_t address)`: 
    * Returnerar innehållet (en byte) på angiven adress om adressen är giltig (`address < Size`), annars `0U`.
    * Ska vara `const`. 
* `write(std::uint16_t address, std::uint8_t data)`: 
    * Skriver en byte till angiven adress i minnet om adressen är giltig (`address < Size`).
    * Om skrivning sker returneras `true`, annars returneras `false`. 

För denna klass ska copy- och move-konstruktorerna samt motsvarande operatorer raderas i enlighet med uppgift 2.  
Klassen ska inte heller kunna ärvas vidare.

Använd följande testprogram för att verifiera din implementation:

```cpp
/**
 * @brief Test program 3.
 */
#include <cstdint>
#include <cstdio>
#include <limits>

#include "driver/eeprom/stub.h"

namespace
{
// -----------------------------------------------------------------------------
template<std::uint16_t Size>
bool testEeprom(driver::eeprom::Stub<Size>& eeprom) noexcept
{
    constexpr std::uint16_t invalidAddress{std::numeric_limits<std::uint16_t>::max()};
    static_assert(invalidAddress > Size, 
        "EEPROM size must be less than 65 535 for tests to be performed!");
    constexpr std::uint8_t mockByte{100U};

    // Except write operations to succeed for valid EEPROM addresses.
    for (std::uint16_t address{}; address < Size; ++address)
    {
        // Terminate the test on failure.
        if (!eeprom.write(address, mockByte))
        {
            std::printf("Failed to write value %u to EEPROM address %u!\n", mockByte, address);
            return false;
        }

        // Verify that the mock value was written, terminate the test on failure.
        const std::uint8_t byte{eeprom.read(address)};

        if (mockByte != byte)
        {
            std::printf("EEPROM read failure at address %u: expected %u, actual is %u!\n", 
                    address, mockByte, byte);
            return false;
        }
    }

    // Try to write a value outside of the EEPROM memory, expect failure.
    if (eeprom.write(invalidAddress, mockByte))
    {
        std::printf("Write to invalid address %u succeeded, which shouldn't occur!\n", 
            invalidAddress);
        return false;
    }

    // Try to read from an invalid address, expect 0 to be returned.
    const std::uint8_t invalidByte{eeprom.read(invalidAddress)};

    if (0U != invalidByte)
    {
        std::printf("EEPROM should return 0 when attempting to read from an invalid address, "
            "actual value is %u!\n", invalidByte);
            return false;
    }
    // Return true if all tests pass.
    std::printf("EEPROM tests succeeded!\n");
    return true;
}
} // namespace

// -----------------------------------------------------------------------------
int main()
{
    driver::eeprom::Stub eeprom{};
    return testEeprom(eeprom) ? 0 : -1;
}
```

---

### **5.** Inspektion av flertrådat testprogram (6p)
Betrakta koden nedan. Anta att:
* Samtliga interfaces och factory-metoder är korrekt implementerade.
* Nödvändiga inkluderingsdirektiv är tillagda.

```cpp
namespace
{
struct Mock
{
    Mock() noexcept;
    void runTest(std::uint8_t pressCount = 5U) noexcept;

private:
    void delay_ms(std::uint16_t ms) noexcept;
    void simulateButtonEvent(std::uint8_t pressCount) noexcept;
    void detectButtonPress(bool& buttonPressed) noexcept;
    void toggleLed(const bool& buttonPressed) noexcept;

    bool myStopFlag;
    std::unique_ptr<driver::gpio::Interface> myButton;
    std::unique_ptr<driver::gpio::Interface> myLed;
    std::unique_ptr<driver::eeprom::Interface> myEeprom;
};

// -----------------------------------------------------------------------------
Mock::Mock() noexcept
    : myStopFlag{false}
    , myButton{nullptr}
    , myLed{nullptr}
    , myEeprom{nullptr}
{
    constexpr std::uint8_t mockPin{0U};
    driver::factory::Stub factory{};
    myLed    = factory.gpio(mockPin);
    myButton = factory.gpio(mockPin);
    myEeprom = factory.eeprom();
}

// -----------------------------------------------------------------------------
void Mock::runTest(const std::uint8_t pressCount) noexcept
{
    bool buttonPressed{false};

    std::thread t1{&Mock::simulateButtonEvent, this, pressCount};
    std::thread t2{&Mock::detectButtonPress, this, std::ref(buttonPressed)};
    std::thread t3{&Mock::toggleLed, this, std::cref(buttonPressed)};

    t1.join();
    t2.join();
    t3.join();
}

// -----------------------------------------------------------------------------
void Mock::delay_ms(const std::uint16_t ms) noexcept
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// -----------------------------------------------------------------------------
void Mock::simulateButtonEvent(const std::uint8_t pressCount) noexcept
{
    constexpr std::uint16_t pollInterval_ms{100U};
    const std::size_t eventCount{2U * pressCount};

    for (std::size_t i{}; i < eventCount; ++i)
    {
        myButton->toggle();
        delay_ms(pollInterval_ms);
    }
    myStopFlag = true;
}

// -----------------------------------------------------------------------------
void Mock::detectButtonPress(bool& buttonPressed) noexcept
{
    constexpr std::uint16_t pollInterval_ms{10U};
    bool buttonPrev{false};

    while (!myStopFlag)
    {
        const bool buttonCurrent{myButton->read()};
        buttonPressed = buttonCurrent && !buttonPrev;
        buttonPrev    = buttonCurrent;
        delay_ms(pollInterval_ms);
    }
}

// -----------------------------------------------------------------------------
void Mock::toggleLed(const bool& buttonPressed) noexcept
{
    constexpr std::uint16_t pollInterval_ms{10U};
    constexpr std::uint16_t ledAddress{0U};
    
    const bool state{static_cast<bool>(myEeprom->read(ledAddress))};
    myLed->write(state);

    while (!myStopFlag)
    {
        if (buttonPressed) 
        { 
            myLed->toggle();
            const bool state{myLed->read()};
            myEeprom->write(ledAddress, static_cast<std::uint8_t>(state));
            const char* stateStr{myLed->read() ? "on" : "off"};
            std::cout << "Button pressed: the LED is " << stateStr << "!\n";
        }
        delay_ms(pollInterval_ms);
    }
}
} // namespace

// -----------------------------------------------------------------------------
int main()
{
    Mock mock{};
    mock.runTest();
    return 0;
}
```

Svara på följande frågor:
* Vad sker i koden?
* Koden är inte trådsäker:
    * Förklara varför.
    * Föreslå minst två konkreta kodändringar.

---
