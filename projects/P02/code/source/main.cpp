/**
 * @brief P02 skeleton code.
 */
#include "driver/gpio/atmega328p.h"
#include "driver/timer/atmega328p.h"

using namespace driver;

namespace
{
/**
 * @brief Callback for the button.
 * 
 *        This callback is invoked when a button event occurs.
 */
void buttonEvent() noexcept 
{
    //! @todo Forward events to your logic instance, for instance:
    // myLogic->handleButtonEvent();
}

/**
 * @brief Callback for the toggle timer.
 * 
 *        This callback is invoked when the toggle timer times out.
 */
void timerElapsed() noexcept 
{	
    //! @todo Forward events to your logic instance:
    // myLogic->handleTimerEvent();
}
} // namespace

/**
 * @brief Initialize and run the system on the target MCU.
 * 
 * @return 0 on termination of the program (should never occur).
 */
int main()
{
    constexpr uint8_t ledPin{8U};
    constexpr uint8_t buttonPin{13U};
    constexpr uint32_t timeout_ms{100U};

    constexpr auto input{gpio::Direction::InputPullup};
    constexpr auto output{gpio::Direction::Output};

    // Initialize the GPIO devices.
    gpio::Atmega328p led{ledPin, output};
    gpio::Atmega328p button{buttonPin, input, buttonEvent};

    // Initialize the timer.
    timer::Atmega328p timer{timeout_ms, timerElapsed};

    // Todo:
    // 1. Create system::logic::Logic using button, led and timer.
    // 2. Connect callbacks to Logic handlers.
    // 3. Call logic.run(stop).
    while (1) {}
    return 0;
}
