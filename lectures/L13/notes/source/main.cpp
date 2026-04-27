/**
 * @brief Demo using a stub factory.
 */
#include "driver/factory/stub.h"
#include "system/logic/logic.h"

/**
 * @brief Run the system using stubbed drivers.
 *
 * @return 0 on exit (should never happen).
 */
int main()
{
    // Create a stub factory.
    driver::factory::Stub factory{};

    // Create system logic.
    // The stub factory is injected, hence stub implementations are used.
    system::logic::Logic logic{factory};

    // Run the system (infinite loop).
    logic.run();

    // Return success code (unreachable).
    return 0;
}
