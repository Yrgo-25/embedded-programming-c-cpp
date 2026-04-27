/**
 * @brief Timer interface.
 */
#pragma once

namespace driver::timer
{
/**
 * @brief Timer interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Check if the timer is running.
     * 
     * @return True if the timer is running, false otherwise.
     */
    virtual bool isRunning() const noexcept = 0;

    /**
     * @brief Start the timer.
     */
    virtual void start() noexcept = 0;

    /**
     * @brief Stop the timer.
     */
    virtual void stop() noexcept = 0;

    /**
     * @brief Toggle the timer.
     */
    virtual void toggle() noexcept = 0;

    /**
     * @brief Increment the timer.
     * 
     * @note This operation is only performed if the timer is running.
     */
    virtual void tick() noexcept = 0;

    /**
     * @brief Check if the timer has timed out.
     * 
     * @return True if the timer has timed out, false otherwise.
     * 
     * @note The internal counter is cleared automatically on timeout.
     */
    virtual bool hasTimedOut() noexcept = 0;
};
} // namespace driver::timer
