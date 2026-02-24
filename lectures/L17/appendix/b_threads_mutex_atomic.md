# Bilaga B

## Trådar, mutex och atomiska variabler
I denna bilaga demonstreras grunderna i concurrency i C++:
* `std::thread` - Trådar för parallell exekvering av kod i C++.
* `std::mutex` kombinerat med `std::lock_guard` - Skydd för delad data (kritiska sektioner).
* `std::atomic` - Hur vi delar enstaka variabler mellan trådar utan mutex.
* Skillnaden mellan atomiska variabler och mutex i praktiken.

---

### Teori

#### 1. Vad är en tråd?
* En tråd är ett exekveringsflöde.
* Flera trådar delar samma adressrymd.
* I C++ skapas en tråd med:

``` cpp
std::thread t{funktion, argument};
```

**Viktigt**: Metoden `join()` måste anropas för att synkronisera trådar:

```cpp
std::thread t1{funktion1, argument1};
std::thread t2{funktion2, argument2};

t1.join();
t2.join();
```

Alternativt kan `detach()` användas så att den givna tråden körs isolerat från övriga trådar, men detta är ej rekommenderat i kursens kontext.

#### 2. Vad är en data race?
En data race uppstår när:
* Två trådar har åtkomst till samma minnesområde.
* Minst en åtkomst är skrivning.
* Ingen synkronisering finns.

Resultat:
* Leder till *undefined behavior* – även om programmet verkar fungera vid testkörning.
* Programmet kan fungera tills det plötsligt inte gör det.

#### 3. Mutex - kritisk sektion
Mutex är ett lås, som medför att man enkelt kan skydda kritiska regioner, dvs. regioner innehållande resurser som delas mellan trådar.

Mutex implementeras via typen `std::mutex` från headerfilen `<mutex>` i C++:

```cpp
std::mutex mutex{};
```

Mutex kan användas för att skydda resurser som delas mellan trådar:
* En given mutex delas mellan trådarna.
* Vi skyddar delade resurser genom att låsa mutexen via metoden `lock()`. Denna metod kommer blocka aktuell tråd tills resurserna är tillgängliga.
* Vi gör delade resurser tillgängliga för andra trådar genom att låsa upp mutexen via metoden `unlock()`.
* En mutex kan skydda flera relaterade variabler samtidigt.

Nedan visas ett exempel på hur en mutex delas mellan två funktioner, som körs i var sin tråd.
Vi kan tänka att respektive tråd tillfälligt får exklusiv åtkomst till de delade resurserna:
* Trådarna turas om att använda `std::cout` för terminalutskrifter.
* En mutex används för att skydda `std::cout` i respektive tråd:
    * En given tråd reserverar `std::cout` så fort denna är tillgänglig genom att låsa mutexen.
    Så länge `std::cout` är reserverad ligger denna tråd och väntar.
    * När `std::cout` är tillgänlig genomförs utskrift.
    * Efter att utskriften är genomförd låses mutexen upp för att göra `std::cout` tillgänglig för den andra tråden.

**OBS!** Dessa exempel saknar stoppmekanism och används endast för illustration.

```cpp
std::mutex mutex{};

// Function running in thread 1.
void thread1Func()
{
    while (1)
    {
        // Lock mutex before entering critical region.
        mutex.lock();
        {
            // Critical region - Feel free to access shared resources here.
            // Note: Terminal print is a resource shared between the threads.
            std::cout << "Thread1 currently has access to the terminal!\n";
        }
        // Unlock the mutex to make shared data available for the other thread.
        mutex.unlock();
    }
}

// Function running in thread 2.
void thread2Func()
{
    while (1)
    {
        // Lock mutex before entering critical region.
        mutex.lock();
        {
            // Critical region - Feel free to access shared resources here.
            // Note: Terminal print is a resource shared between the threads.
            std::cout << "Thread2 currently has access to the terminal!\n";
        }
        // Unlock the mutex to make shared data available for other threads.
        mutex.unlock();
    }
}
```

Vi kan också använda oss av typen `std::lock_guard<T>` från headerfilen `<mutex>` för att automatiskt låsa och låsa upp vår mutex:

```cpp
std::lock_guard<std::mutex> lock{mutex};
```

Detta genomförs genom att:
* Mutexen låses automatiskt när vår lock guard skapas.
* Mutexen låses upp automatiskt när vår lock guard raderas, dvs. när den går ur scope.

Ovanstående exempel kan därmed implementeras enligt nedan, där lock guards används för att hantera mutexen. Måsvingar används för att begränsa livslängden på respektive lock guard:

```cpp
std::mutex mutex{};

// Function running in thread 1.
void thread1Func()
{
    while (1)
    {
        // Enter critical region, lock the mutex to protect shared resources.
        {
            std::lock_guard<std::mutex> lock{mutex};
            std::cout << "Thread1 currently has access to the terminal!\n";
            // The mutex is automatically unlocked by the lock guard here.
        }
    }
}

// Function running in thread 2.
void thread2Func()
{
    while (1)
    {
        // Enter critical region, lock the mutex to protect shared resources.
        {
            std::lock_guard<std::mutex> lock{mutex};
            std::cout << "Thread2 currently has access to the terminal!\n";
            // The mutex is automatically unlocked by the lock guard here.
        }
    }
}
```

---

#### 4. `std::atomic`
Enstaka variabler som delas mellan trådar, exempelvis stoppflaggor, kan sättas till atomiska i stället för att använda en mutex. Detta underlättar kraftigt, då vi inte behöver skapa en mutex och låsa/låsa upp denna:
* Atomiska variabler implementeras via typen `std::atomic` från headerfilen `<atomic>`.
* Som exempel, en atomisk bool-variabel döpt `stopFlag` kan implementeras såsom visas nedan:

```cpp
std::atomic<bool> stopFlag{false};
```

För att sätta värdet på stoppflaggan kan vi anropa metoder `store()` samt `load()`:

```cpp
// Set the stop flag.
stopFlag.store(true);

// Check the stop flag.
const bool stop{stopFlag.load()};
```

Atomiska stoppflaggor är vanligt för att stoppa trådar samtidigt, såsom visas nedan, där två trådar
körs så länge given stoppflagga inte är satt:

```cpp
// Function running in thread 1.
void thread1Func(const std::atomic<bool>& stopFlag) noexcept
{
    // Run thread 1 until the stop flag is set.
    while (!stopFlag.load())
    {
        // Add code here!
    }
}

// Function running in thread 2.
void thread2Func(const std::atomic<bool>& stopFlag) noexcept
{
    // Run thread 2 until the stop flag is set.
    while (!stopFlag.load())
    {
        // Add code here!
    }
}
```

Viktigt:
* Atomic skyddar endast en variabel.
* Om flera variabler måste uppdateras tillsammans krävs en mutex, även om varje enskild variabel är atomisk.
* En fördel med mutex är att den också erbjuder minnessynkronisering mellan trådar, så att skrivningar i kritiska sektionen blir synliga för andra trådar när låset släpps.

---

### Exempel: Mutex med lock guard samt atomisk stoppflagga
Nedanstående exempel demonstrerar ett typiskt embedded-scenario:
* En sändtråd (TX) producerar ett meddelande periodiskt.
* En mottagartråd (RX) läser och konsumerar ny data.
* En tredje tråd stoppar programexekveringen efter en viss tid.

``` cpp
#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>

namespace
{
/**
 * @brief Shared memory used by TX and RX threads.
 *
 *        Access must be protected by mutex to avoid data races.
 */
struct SharedMem
{
    /** Shared data. */
    std::uint16_t data{};

    /** Indicate that new data is available (protected by a mutex, hence not atomic). */
    bool newData{false};
};

/** Mutex to protect critical regions. */
std::mutex mutex{};

// -----------------------------------------------------------------------------
void wait_ms(const std::size_t ms) noexcept
{
    // Sleep the calling thread for the given duration.
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// -----------------------------------------------------------------------------
void txThread(SharedMem& shared, const std::atomic<bool>& stop,
              const std::size_t txInterval_ms) noexcept
{
    std::uint16_t counter{};

    // Run the TX loop as long as the stop flag isn't set.
    while (!stop.load())
    {
        // Enter critical section, lock the mutex to protect shared memory.
        {
            std::lock_guard<std::mutex> lock{mutex};

            // Produce new data and mark it as available.
            shared.data    = counter++;
            shared.newData = true;
            std::cout << "TX: Produced " << shared.data << "!\n";
        }
        wait_ms(txInterval_ms);
    }
}

// -----------------------------------------------------------------------------
void rxThread(SharedMem& shared, const std::atomic<bool>& stop,
              const std::size_t rxInterval_ms) noexcept
{
    // Run the RX loop as long as the stop flag isn't set.
    while (!stop.load())
    {
        // Enter critical section, lock the mutex to protect shared memory.
        {
            std::lock_guard<std::mutex> lock{mutex};

            // Consume new data if available.
            if (shared.newData)
            {
                std::cout << "RX: Consumed " << shared.data << "!\n";
                shared.newData = false;
            }
        }
        wait_ms(rxInterval_ms);
    }
}

// -----------------------------------------------------------------------------
void stopThread(std::atomic<bool>& stop, const std::size_t timeout_ms) noexcept
{
    // Wait for timeout, then signal all threads to stop.
    wait_ms(timeout_ms);
    stop.store(true);
}
} // namespace

// -----------------------------------------------------------------------------
int main()
{
    constexpr std::size_t txInterval_ms{1000U};
    constexpr std::size_t rxInterval_ms{100U};
    constexpr std::size_t threadTimeout_ms{10000U};

    // Shared memory structure.
    SharedMem sharedMem{};

    // Atomic stop flag shared between threads.
    // Atomic is sufficient here, since it protects a single variable.
    std::atomic<bool> stopFlag{false};

    // Create and run threads during ten seconds to simulate communication.
    std::thread t1{txThread, std::ref(sharedMem), std::cref(stopFlag), txInterval_ms};
    std::thread t2{rxThread, std::ref(sharedMem), std::cref(stopFlag), rxInterval_ms};
    std::thread t3{stopThread, std::ref(stopFlag), threadTimeout_ms};

    // Synchronize the threads.
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
```
---

### Varför behövs mutex?
`SharedMem` innehåller resurser som delas mellan TX- och RX-trådarna och som hör ihop:
* `data`: Data som delas mellan trådarna.
* `newData`: Indikerar att ny data finns tillgänglig.

Om TX och RX kör samtidigt utan mutex kan RX läsa data medan den håller på att uppdateras.  
Mutex säkerställer att uppdateringen av båda fälten sker helt och hållet innan någon annan tråd får läsa dem.

---

### Tumregel
* `std::atomic` är bra för enstaka variabler, exempelvis för stoppflaggor.
* `std::mutex` behövs när multipla resurser, såsom variabler, delas mellan trådarna.

---

## Sammanfattning
I denna bilaga har vi sett:
* Hur man skapar trådar med `std::thread`.
* Hur man skyddar delad data med `std::mutex`.
* Hur man använder `std::atomic` korrekt.
* Skillnaden mellan atomiska variabler och mutex.

Detta är grunden för säker parallell programmering i modern C++.

---
