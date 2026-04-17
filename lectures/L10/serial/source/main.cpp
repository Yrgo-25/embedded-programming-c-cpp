/**
 * @brief Serial driver demo.
 */
#include <cstdint>
#include <cstdio>

#include "driver/serial/stub.h"

namespace
{
/**
 * @brief Run the system with a serial device.
 *
 * @param serial Serial interface used for writing and reading.
 */
void runSystem(driver::serial::Interface& serial) noexcept
{
    serial.write("Available serial data: ");

    // Read and print available data byte by byte.
    while (serial.isDataAvailable())
    {
        // Print bytes in two-digit hexadecimal form.
        const std::uint8_t byte{serial.read()};
        std::printf("%02X ", byte);
    }
    serial.write("\n");
}
} // namespace

/**
 * @brief Create a serial stub and process simulated input.
 * 
 * @return Exit code 0.
 */
int main()
{
    // Create and connect a serial stub.
    driver::serial::Stub serial{};
    serial.connect();

    // Simulate incoming serial data.
    constexpr std::uint8_t bufLen{5U};
    const std::uint8_t buf[bufLen]{0x10U, 0xFFU, 0xA0U, 0x01U, 0x7FU};
    serial.simulateInput(buf, bufLen);

    // Pass serial driver to system logic.
    runSystem(serial);
    return 0;
}
