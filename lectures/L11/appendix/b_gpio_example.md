# Bilaga B

## Enkelt GPIO‑exempel

### Hårdvara
* Anslut en lysdiod i serie med en resistor (120 Ω) till pin 2 på Arduino Nano.
* Anslut den andra sidan av lysdioden till jord.

---

### Exempelkod

* Byt ut innehållet i `main/blink.c` mot följande kod:

``` c
/**
 * @brief GPIO example.
 */
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/** LED pin on the ESP32-S3. */
#define LED_PIN 2U

/** High GPIO signal. */
#define GPIO_HIGH 1U

/** Low GPIO signal. */
#define GPIO_LOW 0U

/** Blink speed in milliseconds. */
#define BLINK_SPEED_MS 500U

/**
 * @brief Blink an LED every 500 ms.
 */
void app_main(void)
{
    // Configure LED as output.
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Blink LED every 500 ms.
    while (1)
    {
        gpio_set_level(LED_PIN, GPIO_HIGH);
        vTaskDelay(pdMS_TO_TICKS(BLINK_SPEED_MS));

        gpio_set_level(LED_PIN, GPIO_LOW);
        vTaskDelay(pdMS_TO_TICKS(BLINK_SPEED_MS));
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
