/**
 * @brief Demonstration of interfaces for LEDs.
 */
#include <chrono>
#include <cstdint>
#include <thread>

#include "driver/led/atmega328p.h"
#include "driver/led/esp32s3.h"

using namespace driver;

namespace
{
// -----------------------------------------------------------------------------
void delayMs(const std::uint16_t durationMs) noexcept
{
    std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
}

// -----------------------------------------------------------------------------
void blinkLed(driver::led::Interface& led, const std::uint16_t blinkTimeMs) noexcept
{
    // Toggle the LED, then delay the calling thread.
    led.toggle();
    delayMs(blinkTimeMs);
    
    // Toggle the LED again, then delay the calling thread.
    led.toggle();
    delayMs(blinkTimeMs);
}
} // namespace

/**
 * @brief Blink LEDs connected to different microprocessors.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Connect LED1 to pin 9 of the ATmega328P processor.
    driver::led::Atmega328p led1{9U};

    // Connect LED2 to pin 20 of the ESP32-S3 processor, enable the LED on startup.
    driver::led::Esp32s3 led2{20U, true};

    // Blink the LEDs ten times, then terminate the program.
    constexpr std::uint16_t iterations{10U};

    for (std::uint16_t i{}; i < iterations; ++i) 
    { 
        blinkLed(led1, 1000U);
        blinkLed(led2, 500U); 
    }
    return 0;
}
