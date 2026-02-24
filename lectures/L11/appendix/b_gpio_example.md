# Bilaga B

## Enkelt GPIO‑exempel

### Hårdvara
* Anslut en lysdiod i serie med en resistor (120 Ω) till pin 2 på Arduino Nano.
* Anslut den andra sidan av lysdioden till jord.

---

### Exempelkod

* Byt ut innehållet i `main/blink.c` mot följande kod:

``` c
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "freertos/task.h"

#define LED_PIN 2

void app_main(void)
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));

        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
```

---

### Bygg och flasha

Kör följande kommandon (byt ut `COMx` mot använd COM-port).

``` bash
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
