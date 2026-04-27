/**
 * @brief Stub factory.
 */
#pragma once

#include <cstdint>

#include "driver/factory/interface.h"
#include "driver/serial/stub.h"
#include "driver/timer/stub.h"

namespace driver::factory
{
/**
 * @brief Stub factory.
 * 
 *        This class is non-copyable and non-movable.
 */
class Stub final : public Interface
{
public:
    /**
     * @brief Constructor.
     */
    Stub() noexcept = default;

    /**
     * @brief Destructor.
     */
    ~Stub() noexcept override = default;

    /**
     * @brief Create a new serial instance.
     * 
     * @return Pointer to the new serial instance, or nullptr on failure.
     */
    driver::serial::Interface* serial() noexcept override
    {
        return new driver::serial::Stub{};
    }

     /**
     * @brief Create a new timer instance.
     * 
     * @param[in] timeout_ms Timeout in milliseconds.
     * 
     * @return Pointer to the new timer instance, or nullptr on failure.
     */
    driver::timer::Interface* timer(std::uint16_t timeout_ms) noexcept override
    {
        return new driver::timer::Stub{timeout_ms};
    }

    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment.
    Stub& operator=(Stub&&)      = delete; // No move assignment.
};
} // namespace driver::factory
