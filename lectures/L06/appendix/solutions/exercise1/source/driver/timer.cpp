/**
 * @brief Timer driver implementation details.
 */
#include <cstdint>
#include <cstdio>

#include "driver/timer.h"

namespace driver
{
namespace
{
// -----------------------------------------------------------------------------
constexpr bool isTimeoutValid(const std::uint32_t timeout_ms) noexcept
{
    return 0U < timeout_ms;
}
} // namespace

// -----------------------------------------------------------------------------
Timer::Timer(const std::uint32_t timeout_ms) noexcept
    : myTimeout_ms{timeout_ms}
    , myCounter_ms{}
    , myRunning{false}
    , myInitialized{isTimeoutValid(timeout_ms)}
{}

// -----------------------------------------------------------------------------
Timer::~Timer() noexcept
{
    // Stop the timer if running.
    if (myRunning)
    {
        std::printf("Stopping timer before deletion!\n");
        stop();
    }
}

// -----------------------------------------------------------------------------
std::uint32_t Timer::timeout_ms() const noexcept { return myTimeout_ms; }

// -----------------------------------------------------------------------------
bool Timer::isRunning() const noexcept { return myRunning; }

// -----------------------------------------------------------------------------
bool Timer::isInitialized() const noexcept { return myInitialized; }

// -----------------------------------------------------------------------------
void Timer::start() noexcept 
{ 
    if (myInitialized && !myRunning) { myRunning = true; }
}

// -----------------------------------------------------------------------------
void Timer::stop() noexcept 
{ 
    if (myInitialized && myRunning) { myRunning = false; }
}

// -----------------------------------------------------------------------------
void Timer::toggle() noexcept 
{ 
    if (myInitialized) { myRunning = !myRunning; }
}

// -----------------------------------------------------------------------------
void Timer::tick() noexcept
{
    // Increment the millisecond counter only if the timer is running.
    // The timer cannot start if not initialized, hence no initialization check.
    if (myRunning) { myCounter_ms++; }
}

// -----------------------------------------------------------------------------
bool Timer::hasTimedOut() noexcept
{
    // Check if timeout has occurred, clear the millisecond counter if true.
    // The timer cannot tick if not initialized, hence no initialization check.
    const bool timeout{myTimeout_ms <= myCounter_ms};
    if (timeout) { myCounter_ms = 0U; }
    return timeout && myInitialized;
}
} // namespace driver
