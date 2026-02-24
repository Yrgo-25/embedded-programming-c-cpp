# Bilaga A

## Repetitionsuppgift

### Uppgiftsbeskrivning
Nedan visas ett flertrådat testprogram, där en simulerad lysdiod togglas vid stigande flank på en simulerad tryckknapp. För att genomföra simuleringen har tre trådar skapats:
* Den första tråden håller igång programmet under fem sekunder via en stoppflagga.
* Den andra tråden simulerar nedtryckning samt uppsläppning av knappen var 500:e ms.
* Den tredje tråden togglar en lysdiod på stigande flank på tryckknappen.

Flertrådningsdelen kommer behandlas senare under kursen. Det ni ska göra är att:
* Skapa en strukt döpt `gpio` med alias `gpio_t`, som ska utgöra en simulerad GPIO.
* Strukten ska endast innehålla en medlemsvariabel: 
    * `bool state`, som indikerar GPIO:ns tillstånd (`true` = hög, `false` = låg).
* Skapa associerade funktioner: 
    * `gpio_read()`: Läser GPIO:ns tillstånd (returnerar `state`). GPIO:n ska vara enbart läsbar i denna funktion.
    * `gpio_write(value)`: Sätter GPIO:ns tillstånd (sätter `state = value`).
    * `gpio_toggle()`: Togglar GPIO:ns tillstånd (sätter `state = !state`).
* **OBS!** Glöm inte nullpekar-kontroller i respektive funktion.

---

### Deluppgifter
**1.** Skriv först koden för hand (för att enklare komma ihåg detaljer). Titta vid behov på material från föregående lektion. Att skriva ned koden för hand gör det lättare att komma ihåg innehållet.  
**2.** Skriv därefter in koden i programmet nedan.   
**3.** Testkör koden [här](https://www.onlinegdb.com/online_c_compiler).

---

### Testprogram

```c
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>

/** Test parameters. */
#define TEST_DURATION_MS 5000U      // Test duration (5000 ms).
#define BUTTON_EVENT_PERIOD_MS 500U // Button event period (500 ms).
#define POLL_PERIOD_MS 10U          // Poll period (10 ms).

/** @todo Implement gpio_t and associated functions here! */

/**
 * @brief Structure holding resources shared between the threads.
 */
typedef struct shared
{
    /** LED to toggle on rising edge of the button. */
    gpio_t* led;

    /** Button to toggle the LED. */
    gpio_t* button;

    /** Mutex to protect shared resources. */
    pthread_mutex_t* mutex;

    /** Stop flag to stop all threads. */
    _Atomic bool* stop_flag;
} shared_t;

/**
 * @brief Stop all running threads once the test is finished.
 * 
 * @param[in] arg Pointer to structure holding shared resources.
 * 
 * @return NULL to satisfy the requirements of thread functions.
 */
static void* stop_thread(void* arg)
{
    // Retrieve shared resources, terminate the function on failure.
    shared_t* shared = (shared_t*)(arg);
    if (NULL == shared) { return NULL; }

    // Set the stop flag to stop all threads on timeout, then terminate the function.
    usleep(TEST_DURATION_MS * 1000U);
    atomic_store(shared->stop_flag, true);
    return NULL;
}

/**
 * @brief Thread to simulate button events.
 * 
 * @param[in] arg Pointer to structure holding shared resources.
 * 
 * @return NULL to satisfy the requirements of thread functions.
 */
static void* button_event_thread(void* arg)
{
    // Retrieve shared resources, terminate the function on failure.
    shared_t* shared = (shared_t*)(arg);
    if (NULL == shared) { return NULL; }
    
    // Keep this function going as long as the stop flag isn't set.
    while (!atomic_load(shared->stop_flag))
    {
        // Enter critical section; lock the mutex to protect shared resources.
        pthread_mutex_lock(shared->mutex);
        // Toggle the button to simulate pressdown/release.
        gpio_toggle(shared->button);
        // Leave critical section; unlock the mutex.
        pthread_mutex_unlock(shared->mutex);
        // Wait before toggling the button again.
        usleep(BUTTON_EVENT_PERIOD_MS * 1000U);
    }
    return NULL;
}

/**
 * @brief Thread to toggle the LED on button pressdown (rising edge).
 * 
 * @param[in] arg Pointer to structure holding shared resources.
 * 
 * @return NULL to satisfy the requirements of thread functions.
 */
static void* led_toggle_thread(void* arg)
{
    // Retrieve shared resources, terminate the function on failure.
    shared_t* shared = (shared_t*)(arg);
    if (NULL == shared) { return NULL; }
    bool button_prev = false;
    
    // Keep this function going as long as the stop flag isn't set.
    while (!atomic_load(shared->stop_flag))
    {
        // Enter critical section; lock the mutex to protect shared resources.
        pthread_mutex_lock(shared->mutex);
        const bool button_current = gpio_read(shared->button);
        
        // Toggle the LED on rising button edge.
        if (button_current && !button_prev)
        {
            gpio_toggle(shared->led);
            const char* msg = gpio_read(shared->led) ? "on" : "off";
            printf("Button pressed: The LED is %s!\n", msg);
        }
        // Leave critical section; unlock the mutex.
        pthread_mutex_unlock(shared->mutex);
        button_prev = button_current;
        usleep(POLL_PERIOD_MS * 1000U);
    }
    return NULL;
}

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
    gpio_t led = {false};
    gpio_t button = {false};
    
    // Initialize the mutex.
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    
    // Initialize the stop flag.
    _Atomic bool stop_flag;
    atomic_init(&stop_flag, false);
    
    // Initialize shared data structure.
    shared_t shared = { &led, &button, &mutex, &stop_flag};
    
    // Initialize the threads.
    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, stop_thread, &shared);
    pthread_create(&t2, NULL, button_event_thread, &shared);
    pthread_create(&t3, NULL, led_toggle_thread, &shared);
    
    // Synchronize the threads.
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    return 0;
}
```

---