/**
 * @brief GPIO driver demo.
 */
#include "driver/gpio/atmega328p.h"
#include "driver/gpio/types.h"

using namespace driver;

/**
 * @brief Initialize GPIO and start the application.
 */
int main()
{
    constexpr uint8_t ledPin{8U};
    constexpr uint8_t buttonPin{13U};

    // Initialize GPIOs.
    gpio::Atmega328p led{ledPin, gpio::Direction::Output};
    gpio::Atmega328p button{buttonPin, gpio::Direction::InputPullup};
    bool buttonPrev{false};
    
	// Toggle the LED on rising button edge.
    while (1) 
    { 
        const bool buttonNow{button.read()};
        const bool risingEdge{buttonNow && !buttonPrev};
        if (risingEdge) { led.toggle(); }
        buttonPrev = buttonNow;
    }
}
