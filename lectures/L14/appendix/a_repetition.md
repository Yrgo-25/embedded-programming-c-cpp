# Bilaga A

## Repetitionsuppgift

### Uppgiftsbeskrivning
Nedan visas ett flertrådat testprogram, där en simulerad lysdiod togglas vid stigande flank på en simulerad tryckknapp. En simulerad serial-driver används för att skriva ut lysdiodens tillstånd. 

För att genomföra simuleringen har tre trådar skapats:
* Den första tråden håller igång programmet under fem sekunder via en stoppflagga.
* Den andra tråden simulerar nedtryckning samt uppsläppning av knappen var 500:e ms.
* Den tredje tråden togglar en lysdiod på stigande flank på tryckknappen.

Flertrådningsdelen kommer behandlas senare under kursen. Det ni ska göra är att:
* Skapa en stubb-factory `driver::factory::Stub`, som ärver `driver::factory::Interface` och implementerar:
    * `gpio(std::uint8_t pin)`, som skapar och returnerar en ny GPIO-stubb i form av en rå pekare (`driver::gpio::Interface*`).
        * Pin-numret används inte i stubben: skriv `(void) (pin);`.
    * `serial()`, som skapar och returnerar en ny serial-stubb i form av en rå pekare (`driver::serial::Interface*`).

Viktigt:
* Använd `new` för att skapa instanserna.
* Eftersom factory skapar med `new` måste resurserna frigöras med `delete`.
* I detta program frigörs resurserna i en destruktor i `thread::Shared`, så det är inget ni behöver göra.

**OBS!** Konstruktorer och kopierings- och förflyttningssemantik kan skippas.

---

### Deluppgifter
**1.** Skriv först koden för hand (för att enklare komma ihåg detaljer). Titta vid behov på material från föregående lektion. Att skriva ned koden för hand gör det lättare att komma ihåg innehållet.  
**2.** Skriv därefter in koden i programmet nedan.  
**3.** Testkör koden [här](https://www.onlinegdb.com/online_c_compiler). 
Byt språk till **C++17** i det övre högra hörnet.

**OBS!** Konstruktorer, destruktorn och kopierings- och förflyttningssemantik kan skippas.


```cpp
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

namespace
{
namespace driver::serial
{
/**
 * @brief Serial driver interface.
 */
class Interface
{
public:
    virtual ~Interface() noexcept = default;
    virtual std::int16_t write(const char* data) noexcept = 0;
    virtual std::int16_t read(char* buffer, std::size_t bufferSize) noexcept = 0;
};

/**
 * @brief Minimalistic serial stub driver.
 */
struct Stub final : public Interface
{
    ~Stub() noexcept override = default;

    std::int16_t write(const char* data) noexcept override
    {
        if (nullptr == data) { return -1; }
        const std::size_t bytesToTransmit{std::strlen(data)};
        std::cout << data;
        return static_cast<std::int16_t>(bytesToTransmit);
    }

    std::int16_t read(char* buffer, const std::size_t bufferSize) noexcept override
    {
        if (nullptr == buffer) { return -1; }
        const std::size_t bytesToCopy{std::min(bufferSize, myBuffer.size())};
        
        for (std::size_t i{}; i < bytesToCopy; ++i)
        {
            buffer[i] = myBuffer[i];
        }
        return static_cast<std::int16_t>(bytesToCopy);
    }

    void setInputData(const char* data) noexcept 
    { 
        if (nullptr != data) { myBuffer = data; }
    }

private:
    std::string myBuffer{};
};
} // namespace driver::serial

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
    virtual gpio::Interface* gpio(std::uint8_t pin) noexcept = 0;
    virtual serial::Interface* serial() noexcept = 0;
};

/** @todo Implement class driver::factory::Stub here! */

} // namespace driver::factory

namespace thread
{
/**
 * @brief Structure holding resources shared between the threads.
 */
struct Shared
{
    /** LED to toggle on rising edge of the button. */
    driver::gpio::Interface* led;

    /** Button to toggle the LED. */
    driver::gpio::Interface* button;

    /** Serial instance to print via UART. */
    driver::serial::Interface* serial;

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
        , serial{factory.serial()}
        , mutex{}
        , stopFlag{false}
    {}

    /**
     * @brief Destructor.
     */
    ~Shared() noexcept
    {
        // Release allocated resources.
        delete led;
        delete button;
        delete serial;

        led    = nullptr;
        button = nullptr;
        serial = nullptr;
    }

    // Ensure unique ownership of allocated resources; prohibit copying.
    Shared(const Shared&)            = delete;
    Shared(Shared&&)                 = delete;
    Shared& operator=(const Shared&) = delete;
    Shared& operator=(Shared&&)      = delete;
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
                const char* msg{shared.led->read() ? "The LED is on!\n" : "The LED is off!\n"};
                shared.serial->write(msg);
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
