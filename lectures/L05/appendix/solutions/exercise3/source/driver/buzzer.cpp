/**
 * @brief Buzzer driver implementation details.
 */
#include <cstdint>
#include <cstdio>

#include "driver/buzzer.h"

namespace driver
{
// -----------------------------------------------------------------------------
Buzzer::Buzzer(const std::uint8_t pin, const bool initialState) noexcept
    : myPin{pin}
    , myEnabled{initialState}
{}

// -----------------------------------------------------------------------------
Buzzer::~Buzzer() noexcept
{
    std::printf("Releasing resources allocated for buzzer at pin %u!\n", myPin);
}

// -----------------------------------------------------------------------------
std::uint8_t Buzzer::pin() const noexcept { return myPin; }

// -----------------------------------------------------------------------------
bool Buzzer::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
void Buzzer::enable() noexcept { myEnabled = true; }

// -----------------------------------------------------------------------------
void Buzzer::disable() noexcept { myEnabled = false; }

// -----------------------------------------------------------------------------
void Buzzer::toggle() noexcept { myEnabled = !myEnabled; }

} // namespace driver
