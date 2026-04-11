/**
 * @brief Solution for exercise set 1.
 */
#include <cstdint>
#include <cstdio>

#include "driver/timer.h"

namespace
{
// -----------------------------------------------------------------------------
int runSystem(driver::Timer& timer) noexcept
{
    // Return -1 if the timer isn't initialized.
    if (!timer.isInitialized()) { return -1; }

    constexpr std::size_t timeouts{3U};
    const std::size_t iterationCount{timer.timeout_ms() * timeouts};

    // Tick the timer continuously, print a message on timeout.
    for (std::size_t i{}; i < iterationCount; ++i)
    {
        timer.tick();

        if (timer.hasTimedOut())
        { 
            std::printf("Timeout after %u ms!\n", timer.timeout_ms()); 
        }
    }
    return 0;
}
} // namespace

/**
 * @brief Create and use a timer.
 * 
 * @return Exit status (0 = success, -1 = failure).
 */
int main()
{
    // Create and start a timer with a 1000 ms timeout.
    constexpr std::uint32_t timeout_ms{1000U};
    driver::Timer timer{timeout_ms};
    timer.start();

    // Run the system, return 0 on success.
    return runSystem(timer);
}
