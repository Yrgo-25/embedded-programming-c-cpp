# Bilaga A

## Repetitionsuppgift

### Uppgiftsbeskrivning
Nedan visas ett flertrådat testprogram, där en simulerad lysdiod togglas vid stigande flank på en simulerad tryckknapp. Lysdiodens tillstånd skrivs till ett simulerat EEPROM-minne.

För att genomföra simuleringen har tre trådar skapats:
* Den första tråden håller igång programmet under fem sekunder via en stoppflagga.
* Den andra tråden simulerar nedtryckning samt uppsläppning av knappen var 500:e ms.
* Den tredje tråden togglar en lysdiod på stigande flank på tryckknappen. Vid toggling skrivs lysdiodens nya tillstånd till EEPROM-minnet.

Skapa ett klasstemplate för ett stubb-minne `driver::eeprom::Stub<std::uint16_t Size>`, som ärver befintligt interface `driver::eeprom::Interface`.

För denna klass gäller följande:
* Template-parameter `Size` utgör storleken på minnet i byte. 
* Som default ska minnet sättas till `256` byte.
* Metoderna ska markeras `override` och `noexcept`.

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
* `clear()`:
    * Rensar EEPROM-minnet genom att fylla det med nollor.

För denna klass ska copy- och move-konstruktorerna samt motsvarande operatorer raderas.  
Klassen ska inte heller kunna ärvas vidare.

---

### Deluppgifter
**1.** Skriv först koden för hand (för att enklare komma ihåg detaljer). Titta vid behov på material från föregående lektion. Att skriva ned koden för hand gör det lättare att komma ihåg innehållet.  
**2.** Skriv därefter in koden i programmet nedan.  
**3.** Testkör koden [här](https://www.onlinegdb.com/online_c_compiler). 
Byt språk till **C++17** i det övre högra hörnet.

---

### Testkod

```cpp
#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

namespace
{
namespace driver::eeprom
{
/**
 * @brief EEPROM interface.
 */
class Interface
{
public:
    virtual ~Interface() noexcept = default;
    virtual std::uint8_t read(std::uint16_t address) const noexcept = 0;
    virtual bool write(std::uint16_t address, std::uint8_t data) noexcept = 0;
    virtual void clear() noexcept = 0;
};

/** @todo Add class driver::eeprom::Stub<std::uint16_t Size = 256U> here! */

} // namespace driver::eeprom

namespace driver::gpio
{
/**
 * @brief GPIO driver interface.
 */
class Interface
{
public:
    virtual ~Interface() noexcept = default;
    virtual bool read() const noexcept = 0;
    virtual void write(const bool state) noexcept = 0;
    virtual void toggle() noexcept = 0;
};

/**
 * @brief Minimalistic GPIO stub driver.
 */
struct Stub final : public Interface
{
    ~Stub() noexcept override = default;
    bool read() const noexcept override { return myState; }
    void write(const bool state) noexcept override { myState = state; }
    void toggle() noexcept override { myState = !myState; }

private:
    bool myState{false};
};
} // namespace driver::gpio

namespace driver::factory
{
/**
 * @brief Factory interface.
 */
class Interface
{
public:
    virtual ~Interface() noexcept = default;
    virtual std::unique_ptr<eeprom::Interface> eeprom() noexcept = 0;
    virtual std::unique_ptr<gpio::Interface> gpio(std::uint8_t pin) noexcept = 0;
};

/**
 * @brief Stub factory.
 */
class Stub final : public Interface
{
public:
    Stub() noexcept = default;
    ~Stub() noexcept override = default;

    std::unique_ptr<eeprom::Interface> eeprom() noexcept override
    {
        return std::make_unique<eeprom::Stub<>>();
    }

    std::unique_ptr<gpio::Interface> gpio(std::uint8_t pin) noexcept override
    {
        (void) (pin);
        return std::make_unique<gpio::Stub>();
    }

    Stub(const Stub&)            = delete;
    Stub(Stub&&)                 = delete;
    Stub& operator=(const Stub&) = delete;
    Stub& operator=(Stub&&)      = delete;
};
} // namespace driver::factory

namespace thread
{
/**
 * @brief Structure holding resources shared between the threads.
 */
struct Shared
{
    /** Address to store the LED state in EEPROM. */
    static constexpr std::uint16_t LedAddr{0U};

    /** LED to toggle on rising edge of the button. */
    std::unique_ptr<driver::gpio::Interface> led;

    /** Button to toggle the LED. */
    std::unique_ptr<driver::gpio::Interface> button;

    /** EEPROM memory. */
    std::unique_ptr<driver::eeprom::Interface> eeprom;

    /** Mutex to protect shared resources. */
    std::mutex mutex;

    /** Stop flag to stop all threads. */
    std::atomic<bool> stopFlag;

    /**
     * @brief Constructor.
     *
     * @param[in] factory Factory to create hardware.
     */
    Shared(driver::factory::Stub& factory) noexcept
        : led{factory.gpio(0U)}
        , button{factory.gpio(1U)}
        , eeprom{factory.eeprom()}
        , mutex{}
        , stopFlag{false}
    {}
};

/**
 * @brief Delay the calling thread.
 *
 * @param[in] ms Duration to delay the thread in ms.
 */
void delay_ms(const std::uint16_t ms) noexcept
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

/**
 * @brief Stop all running threads once the test is finished.
 * 
 * @param[in] shared Structure holding shared resources.
 */
void stopThreads(Shared& shared) noexcept
{
    constexpr std::uint16_t testDuration_ms{5000U};

    // Set the stop flag to stop all threads on timeout, then terminate the function.
    delay_ms(testDuration_ms);
    shared.stopFlag.store(true);
}

/**
 * @brief Thread to simulate button events.
 * 
 * @param[in] shared Structure holding shared resources.
 */
void buttonEvent(Shared& shared)
{   
    constexpr std::uint16_t buttonEventPeriod_ms{500U};

    // Keep this function going as long as the stop flag isn't set.
    while (!shared.stopFlag.load())
    {
        // Enter critical section; lock the mutex to protect shared resources.
        {
            std::lock_guard<std::mutex> lock{shared.mutex};

            // Toggle the button to simulate pressdown/release.
            shared.button->toggle();
        }
        // Wait before toggling the button again.
        delay_ms(buttonEventPeriod_ms);
    }
}

/**
 * @brief Thread to toggle the LED on button pressdown (rising edge).
 * 
 * @param[in] shared Structure holding shared resources.
 */
void ledToggle(Shared& shared)
{
    constexpr std::uint16_t pollPeriod_ms{10U};

    bool buttonPrev{false};
    
    // Keep this function going as long as the stop flag isn't set.
    while (!shared.stopFlag.load())
    {
        // Enter critical section; lock the mutex to protect shared resources.
        {
            std::lock_guard<std::mutex> lock{shared.mutex};
            const bool buttonCurrent{shared.button->read()};
        
            // Toggle the LED on rising button edge.
            if (buttonCurrent && !buttonPrev)
            {
                shared.led->toggle();

                // Store the LED state in EEPROM.
                const std::uint8_t ledState{static_cast<std::uint8_t>(shared.led->read())};
                shared.eeprom->write(shared.LedAddr, ledState);

                // Print the new led state in the terminal by reading from EEPROM.
                const char* state{shared.eeprom->read(shared.LedAddr) ? "on" : "off"};
                std::cout << "Button pressed: The LED is " << state << "!\n";
            }
            buttonPrev = buttonCurrent;
        }
        delay_ms(pollPeriod_ms);
    }
}
} // namespace thread
} // namespace

/**
 * @brief Simulate toggling an LED on rising button edge.
 * 
 *        Use a GPIO stub driver and simulate hardware behaviour via threads.
 * 
 * @return 0 on termination of the function.
 */
int main()
{
    // Create stub factory.
    driver::factory::Stub factory{};
    
    // Initialize shared data structure.
    thread::Shared shared{factory};
    
    // Initialize the threads.
    std::thread t1{thread::stopThreads, std::ref(shared)};
    std::thread t2{thread::buttonEvent, std::ref(shared)};
    std::thread t3{thread::ledToggle, std::ref(shared)};
    
    // Synchronize the threads.
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
```

---
