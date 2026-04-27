/**
 * @brief Timer driver stub.
 */
#pragma once

#include <cstdint>

#include "driver/timer/interface.h"

namespace driver::timer
{
/**
 * @brief Timer driver stub.
 * 
 *        This class is non-copyable and non-movable.
 */
class Stub final : public Interface
{
public:
    /**
     * @brief Constructor.
     * 
     * @param[in] timeout_ms Timeout in milliseconds.
     */
    explicit Stub(const std::uint16_t timeout_ms) noexcept
        : myTimeout_ms{timeout_ms}
        , myCounter_ms{0U}
        , myRunning{false}
    {}

    /**
     * @brief Destructor.
     */
    ~Stub() noexcept override = default;

    /**
     * @brief Check if the timer is running.
     * 
     * @return True if the timer is running, false otherwise.
     */
    bool isRunning() const noexcept override { return myRunning; }

    /**
     * @brief Start the timer.
     */
    void start() noexcept override { myRunning = true; }

    /**
     * @brief Stop the timer.
     */
    void stop() noexcept override { myRunning = false; }

    /**
     * @brief Toggle the timer.
     */
    void toggle() noexcept override { myRunning = !myRunning; }

    /**
     * @brief Increment the timer.
     * 
     * @note This operation is only performed if the timer is running.
     */
    void tick() noexcept override
    {
        // Increment only if the timer is running.
        if (myRunning) { myCounter_ms++; }
    }

    /**
     * @brief Check if the timer has timed out.
     * 
     * @return True if the timer has timed out, false otherwise.
     * 
     * @note The internal counter is cleared automatically on timeout.
     */
    bool hasTimedOut() noexcept override
    {
        // Return false if the timer is stopped.
        if (!myRunning) { return false; }

        // Reset the counter on timeout.
        const bool timeout{myTimeout_ms <= myCounter_ms};
        if (timeout) { myCounter_ms = 0U; }

        // Return true on timeout, else false.
        return timeout;
    }

    Stub()                       = delete; // No default constructor.
    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment.
    Stub& operator=(Stub&&)      = delete; // No move assignment.

private:
    /** Timeout in milliseconds. */
    const std::uint16_t myTimeout_ms;

    /** Internal millisecond counter. */
    std::uint16_t myCounter_ms;

    /** True if the timer is running, false otherwise. */
    bool myRunning;
};
} // namespace driver::timer
