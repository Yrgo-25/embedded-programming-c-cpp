# Bilaga B

## Enkelt GPIO‑exempel

### Hårdvara
* Anslut en lysdiod i serie med en resistor (120 Ω) till pin 2 på Arduino Nano.
* Anslut den andra sidan av lysdioden till jord.

---

### Exempelkod
* Döp om `main/blink.c` till `main/blink.cpp`.
* Uppdatera även filnamnet i `main/CMakeLists.txt`:

```cmake
idf_component_register(SRCS "blink.cpp"
                       INCLUDE_DIRS ".")
```

Ersätt innehållet i `main/blink.cpp` med följande kod:

``` cpp
/**
 * @brief GPIO example.
 */
#include <cstdint>

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief Blink an LED every 500 ms.
 */
extern "C" void app_main(void)
{
    constexpr gpio_num_t ledPin{static_cast<gpio_num_t>(2U)};
    constexpr std::uint32_t gpioHigh{1U};
    constexpr std::uint32_t gpioLow{0U};
    constexpr std::uint32_t blinkSpeed_ms{500U};

    // Configure LED as output.
    gpio_reset_pin(ledPin);
    gpio_set_direction(ledPin, GPIO_MODE_OUTPUT);

    // Blink LED every 500 ms.
    while (1)
    {
        gpio_set_level(ledPin, gpioHigh);
        ESP_LOGI("MAIN", "LED on!");
        vTaskDelay(pdMS_TO_TICKS(blinkSpeed_ms));

        gpio_set_level(ledPin, gpioLow);
        ESP_LOGI("MAIN", "LED off!");
        vTaskDelay(pdMS_TO_TICKS(blinkSpeed_ms));
    }
}
```

---

### Bygg och flasha
Kör följande kommandon (byt ut `COMx` mot använd COM-port).

``` bash
idf.py set-target esp32s3
idf.py build
idf.py -p COMx flash monitor
```

Nu ska lysdioden blinka.

---

### Felsökning

#### Ingen COM‑port syns
* Kontrollera USB‑kabeln.
* Prova en annan USB‑port.
* Kontrollera drivrutiner i Device Manager (Enhetshanteraren).

#### Build‑fel
* Kontrollera att du använder **ESP‑IDF Command Prompt**.
* Kör följande kommandon:

``` bash
idf.py fullclean
idf.py build
```

---
