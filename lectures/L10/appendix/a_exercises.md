# Bilaga A

## Övningsuppgifter

### Uppgiftsbeskrivning
Nedan visas ett flertrådat testprogram, där en simulerad timer löper ut var 500:e millisekund och togglar en lysdiod. Lysdioden har i detta fall implementerats via en simpel strukt för enkelhets skull. 

För att genomföra simuleringen har tre trådar skapats:
* Den första tråden håller igång programmet under fem sekunder via en stoppflagga.
* Den andra tråden ser till att timern löper ut var 500:e ms.
* Den tredje tråden togglar en lysdiod varje gång timern löper ut.

Flertrådningsdelen kommer behandlas senare under kursen. Det ni ska göra är följande:

#### 1. Interface
Skapa ett nytt interface `driver::timer::Interface`, som innehåller följande virtuella metoder:
* Destruktor, ska markeras `virtual` och sättas till `= default`.
* `isRunning()`, som returnerar timerns tillstånd (på/av) som `true/false`.
* `start()`, som startar timern.
* `stop()`, som stoppar timern.
* `toggle()`, som togglar timerns tillstånd.
* `timedOut()`, som indikerar om timern har löpt ut som `true/false`:
    * Vid timeout ska timerns interna timeout-flagga återställas (`= false`).

Viktigt:
* Samtliga metoder ska markeras `noexcept`.
* Samtliga metoder (förutom destruktorn) ska deklareras som rent virtuella (`= 0`).
* `isRunning()` ska vara `const`.

---

#### 2. Stubklass
Skapa en underklass `driver::timer::Stub`, som ärver ovanstående interface.

Implementera denna klass till en enkel stubb, som möjliggör att man kan:
* Kontrollera och läsa timern är på eller av via en privat medlemsvariabel `myState`. 
* Kontrollera och läsa ifall timern har löpt ut via en privat medlemsvariabel `myTimedOut`.

Denna klass ska innehålla:
* En privat medlemsvariabel `myState`, som sparar timerns tillstånd (på/av). 
* En privat medlemsvariabel `myTimedOut`, som sparar timerns timeout-tillstånd.
* En default-konstruktor, som initierar `myState` samt `myTimedOut` till `false`.
* En destruktor, som sätts till `= default`.
* Överlagrade varianter av metoderna från interfacet:
    * `isRunning()` returnerar `myState`.
    * `start()` sätter `myState = true`.
    * `stop()` sätter `myState = false`.
    * `toggle()` togglar `myState`.
    * `timedOut()` gör två saker:
        * Returnerar `true` endast om både `myTimedOut == true` och `myState == true`.
        * Om (och endast om) metoden returnerar `true` ska timeout-flaggan återställas
      (`myTimedOut = false`).
    * `setTimedOut(bool timedOut)`, som sätter timeout-status (`myTimedOut = timedOut`).

För denna klass ska copy- och move-konstruktorerna samt motsvarande operatorer raderas:
* Kopieringskonstruktorn.
* Förflyttningskonstruktorn.
* Kopieringstilldelningsoperatorn.
* Förflyttningstilldelningsoperatorn.

Viktigt:
* De överlagrade metoderna ska markeras `override`.
* Klassen ska inte heller kunna ärvas vidare (klassen ska markeras `final`).

---

### Deluppgifter
**1.** Skriv först koden för hand (för att enklare komma ihåg detaljer). Titta vid behov på material från föregående lektion. Att skriva ned koden för hand gör det lättare att komma ihåg innehållet.  
**2.** Skriv därefter in koden i programmet nedan.  
**3.** Testkör koden [här](https://www.onlinegdb.com/online_c_compiler). 
Byt språk till **C++17** i det övre högra hörnet.

```cpp
#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>

namespace
{
namespace driver::timer
{
/** @todo Implement class driver::timer::Interface here! */

/** @todo Implement class driver::timer::Stub here! */
} // namespace driver::timer

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

namespace thread
{
/**
 * @brief Structure holding resources shared between the threads.
 */
struct Shared
{
    /** LED to toggle on timeout. */
    driver::gpio::Interface& led;

    /** Timer to toggle the LED. */
    driver::timer::Interface& timer;

    /** Mutex to protect shared resources. */
    std::mutex mutex;

    /** Stop flag to stop all threads. */
    std::atomic<bool> stopFlag;

    /**
     * @brief Constructor.
     *
     * @param[in] led LED to toggle on timeout.
     * @param[in] timer Timer to toggle the LED.
     */
    Shared(driver::gpio::Interface& led,
           driver::timer::Interface& timer) noexcept
        : led{led}
        , timer{timer}
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
 * @brief Thread to simulate timeouts.
 * 
 * @param[in] shared Structure holding shared resources.
 */
void timeoutEvent(Shared& shared)
{   
    constexpr std::uint16_t timeout_ms{500U};

    // Cast to timer stub to access 'setTimedOut()'.
    // This is safe in this implementation, since we're using stubs.
    auto& timerStub = static_cast<driver::timer::Stub&>(shared.timer);

    // Start the timer.
    shared.timer.start();

    // Keep this function going as long as the stop flag isn't set.
    while (!shared.stopFlag.load())
    {
        // Wait for timeout.
        delay_ms(timeout_ms);

        // Enter critical section; lock the mutex to protect shared resources.
        {
            std::lock_guard<std::mutex> lock{shared.mutex};

            // Simulate timeout.
            timerStub.setTimedOut(true);
        }
    }
}

/**
 * @brief Thread to toggle the LED on timeout.
 * 
 * @param[in] shared Structure holding shared resources.
 */
void ledToggle(Shared& shared)
{
    constexpr std::uint16_t pollPeriod_ms{10U};
    
    // Keep this function going as long as the stop flag isn't set.
    while (!shared.stopFlag.load())
    {
        // Enter critical section; lock the mutex to protect shared resources.
        {
            std::lock_guard<std::mutex> lock{shared.mutex};
        
            // Toggle the LED on timeout.
            if (shared.timer.timedOut())
            {
                shared.led.toggle();
                const char* state{shared.led.read() ? "on" : "off"};
                std::cout << "Timeout: The LED is " << state << "!\n";
            }
        }
        delay_ms(pollPeriod_ms);
    }
}
} // namespace thread
} // namespace

/**
 * @brief Simulate toggling an LED on timeout.
 * 
 *        Use stub drivers and simulate hardware behaviour via threads.
 * 
 * @return 0 on termination of the function.
 */
int main()
{
    // Initialize the hardware.
    driver::gpio::Stub led{};
    driver::timer::Stub timer{};
    
    // Initialize shared data structure.
    thread::Shared shared{led, timer};
    
    // Initialize the threads.
    std::thread t1{thread::stopThreads, std::ref(shared)};
    std::thread t2{thread::timeoutEvent, std::ref(shared)};
    std::thread t3{thread::ledToggle, std::ref(shared)};
    
    // Synchronize the threads.
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
```

---
