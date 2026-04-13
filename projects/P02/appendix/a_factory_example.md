# Bilaga A

## Exempel på factory-interface

Factoryn används av systemlogiken för att skapa alla drivers utan att känna till
vilken hårdvara som används.

### Filen `driver/factory/interface.h`

Nedan visas hur ett exempel på ett factory-interface, som konstruerar driver-instanser
via smarta pekare:

```cpp
/**
 * @brief Factory interface.
 */
#pragma once

#include <cstdint>
#include <memory>

namespace driver
{
/** ADC driver interface. */
namespace adc { class Interface; }

/** GPIO driver interface. */
namespace gpio { class Interface; }

/** Serial driver interface. */
namespace serial { class Interface; }

/** Temperature sensor driver interface. */
namespace tempsensor { class Interface; }

/** Timer driver interface. */
namespace timer { class Interface; }
} // namespace driver

namespace driver::factory
{
/**
 * @brief Factory interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Create ADC instance.
     * 
     * @return Pointer to the ADC instance, or nullptr on failure.
     */
    virtual std::unique_ptr<adc::Interface> adc() noexcept = 0;

    /**
     * @brief Create GPIO input instance.
     * 
     * @param[in] pin Pin the GPIO instance is connected to.
     * 
     * @return Pointer to the GPIO instance, or nullptr on failure.
     */
    virtual std::unique_ptr<gpio::Interface> gpioInput(std::uint8_t pin) noexcept = 0;

    /**
     * @brief Create GPIO output instance.
     * 
     * @param[in] pin Pin the GPIO instance is connected to.
     * 
     * @return Pointer to the GPIO instance, or nullptr on failure.
     */
    virtual std::unique_ptr<gpio::Interface> gpioOutput(std::uint8_t pin) noexcept = 0;

    /**
     * @brief Create serial instance.
     * 
     * @param[in] baud_bps Baud rate in bps (bits per second).
     * 
     * @return Pointer to the serial instance, or nullptr on failure.
     */
    virtual std::unique_ptr<serial::Interface> serial(std::uint32_t baud_bps) noexcept = 0;

    /**
     * @brief Create temperature sensor instance.
     * 
     * @param[in] pin Pin the temperature sensor instance is connected to.
     * @param[in] adc ADC to use to read the input voltage.
     * 
     * @return Pointer to the temperature sensor instance, or nullptr on failure.
     */
    virtual std::unique_ptr<tempsensor::Interface> tempSensor(std::uint8_t pin,
                                                              adc::Interface& adc) noexcept = 0;

    /**
     * @brief Create timer instance.
     * 
     * @param[in] timeout_ms Timeout in milliseconds.
     * 
     * @return Pointer to the timer instance, or nullptr on failure.
     */
    virtual std::unique_ptr<timer::Interface> timer(std::uint16_t timeout_ms) noexcept = 0;
};
} // namespace driver::factory
```

---

### Användning i `main.cpp`

Vid körning på ESP32-S3 kan factoryn användas för systemlogiken enligt nedan. Här används en factory för att skapa instanser via ESP32-S3-drivers:
Klassen `driver::factory::Esp32s3` ärver `driver::factory::Interface`:

```cpp
int main()
{
    // Create ESP32-S3 factory.
    driver::factory::Esp32s3 factory{};

    // Create system logic, pass factory to create instances with it.
    system::logic::Logic logic(factory);

    bool stop{false};
    logic.run(stop);
}
```

För stub‑läge, använd i stället `driver::factory::Stub` som factory:

```cpp
driver::factory::Stub factory{};
```

---
