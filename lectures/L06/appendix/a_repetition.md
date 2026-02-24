# Bilaga A

## Repetitionsuppgift

### Uppgiftsbeskrivning
Nedan visas ett flertrådat testprogram, där en simulerad lysdiod togglas vid stigande flank på en simulerad tryckknapp. För att genomföra simuleringen har tre trådar skapats:
* Den första tråden håller igång programmet under fem sekunder via en stoppflagga.
* Den andra tråden simulerar nedtryckning samt uppsläppning av knappen var 500:e ms.
* Den tredje tråden togglar en lysdiod på stigande flank på tryckknappen.

Flertrådningsdelen kommer behandlas senare under kursen. Det ni ska göra är att:
* Skapa en klass `driver::gpio::Stub`, som ska utgöra en stubb-driver, dvs. en fejkdriver.
* Klassen ska markeras `final` så att den inte kan ärvas.
* Klassen ska endast innehålla en private medlemsvariabel: 
    * `bool myState`, som indikerar GPIO:ns tillstånd (`true` = hög, `false` = låg).
* Skapa en default-konstruktor, som sätter `myState` till `false`.
* Skapa en destruktor och markera denna som default (`= default`).
* Radera följande:
    * Kopieringskonstruktorn (`Stub(const Stub&)`).
    * Förflyttningskonstruktorn (`Stub(Stub&&)`).
    * Kopieringstilldelningsoperatorn (`Stub& operator=(const Stub&)`).
    * Förflyttningstilldelningsoperatorn (`Stub& operator=(Stub&&)`).
* Skapa följande metoder:
    * `read()`: Läser GPIO:ns tillstånd (returnerar `myState`). Ska markeras `const`.
    * `write(state)`: Sätter GPIO:ns tillstånd (sätter `myState = state`).
    * `toggle()`: Togglar lysdiodens tillstånd (sätter `myState = !myState`).
* **OBS!** Samtliga metoder ska markeras `noexcept`.

---

### Deluppgifter
**1.** Skriv först koden för hand (för att enklare komma ihåg detaljer). Titta vid behov på material från föregående lektion. Att skriva ned koden för hand gör det lättare att komma ihåg innehållet.  
**2.** Skriv därefter in koden i programmet nedan.  
**3.** Testkör koden [här](https://www.onlinegdb.com/online_c_compiler). 
Byt språk till **C++17** i det övre högra hörnet.

---

### Testprogram

```cpp
#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>

namespace
{
namespace driver::gpio
{
/** @todo Implement class driver::gpio::Stub here! */
} // namespace driver::gpio

namespace thread
{
/**
 * @brief Structure holding resources shared between the threads.
 */
struct Shared
{
    /** LED to toggle on rising edge of the button. */
    driver::gpio::Stub& led;

    /** Button to toggle the LED. */
    driver::gpio::Stub& button;

    /** Mutex to protect shared resources. */
    std::mutex mutex;

    /** Stop flag to stop all threads. */
    std::atomic<bool> stopFlag;

    /**
     * @brief Constructor.
     *
     * @param[in] led LED to toggle on rising edge of the button.
     * @param[in] button Button to toggle the LED.
     */
    Shared(driver::gpio::Stub& led,
           driver::gpio::Stub& button) noexcept
        : led{led}
        , button{button}
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
            shared.button.toggle();
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
            const bool buttonCurrent{shared.button.read()};
        
            // Toggle the LED on rising button edge.
            if (buttonCurrent && !buttonPrev)
            {
                shared.led.toggle();
                const char* state{shared.led.read() ? "on" : "off"};
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
    // Initialize the hardware.
    driver::gpio::Stub led{};
    driver::gpio::Stub button{};
    
    // Initialize shared data structure.
    thread::Shared shared{led, button};
    
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