/**
 * @brief System logic implementation.
 */
#pragma once

#include <chrono>
#include <memory>
#include <thread>

#include "driver/factory/interface.h"

namespace system::logic
{
/**
 * @brief System logic implementation.
 * 
 *        This class is non-copyable and non-movable.
 */
class Logic final
{
public:
    /**
     * @brief Constructor.
     * 
     * @param[in] factory Factory with which to create driver instances.
     */
    explicit Logic(driver::factory::Interface& factory) noexcept
        : mySerial{factory.serial()}
        , myTimer{factory.timer(Timeout_ms)}
    {}

    /**
     * @brief Destructor.
     */
    ~Logic() noexcept = default;

    /**
     * @brief Run system.
     */
    void run() noexcept
    {
        /** Start the timer, then enter the main loop. */
        myTimer->start();

        while (true)
        {
            // Simulate that 1 ms passes.
            constexpr std::uint8_t sleep_ms{1U};
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
            
            // Increment the timer every 1 ms.
            myTimer->tick();

            // Print a message on timeout.
            if (myTimer->hasTimedOut())
            {
                mySerial->print("Timeout!\n");
            }
        }
    }

    Logic()                        = delete; // No default constructor.
    Logic(const Logic&)            = delete; // No copy constructor.
    Logic(Logic&&)                 = delete; // No move constructor.
    Logic& operator=(const Logic&) = delete; // No copy assignment.
    Logic& operator=(Logic&&)      = delete; // No copy assignment.

private:
    /** Timeout in ms. */
    static constexpr std::uint16_t Timeout_ms{1000U};

    /** Serial driver for printing. */
    std::unique_ptr<driver::serial::Interface> mySerial;

    /** Internal timer. */
    std::unique_ptr<driver::timer::Interface> myTimer;
};
} // namespace system::logic
