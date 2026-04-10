/**
 * @brief Solution for exercise set 3.
 */
#include <cstdint>
#include <cstdio>

#include "driver/buzzer.h"

namespace
{
// -----------------------------------------------------------------------------
const char* state(const driver::Buzzer& buzzer) noexcept
{
    return buzzer.isEnabled() ? "Enabled" : "Disabled";
}
} // namespace

/**
 * @brief Create and use a buzzer.
 * 
 * @return Exit status (0 = success).
 */
int main()
{
    // Create buzzer connected to pin 8.
    constexpr std::uint8_t buzzerPin{8U};
    driver::Buzzer buzzer{buzzerPin};
    std::printf("Initial state: %s\n", state(buzzer));

    // Enable the buzzer.
    buzzer.enable();
    std::printf("After enable(): %s\n", state(buzzer));

    // Toggle the buzzer.
    buzzer.toggle();
    std::printf("After toggle(): %s\n", state(buzzer));
    return 0;
}
