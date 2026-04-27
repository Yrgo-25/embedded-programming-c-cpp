/**
 * @brief Factory interface with smart pointers.
 */
#pragma once

#include <cstdint>
#include <memory>

/** Serial driver interface. */
namespace driver::serial { class Interface; }

/** Timer driver interface. */
namespace driver::timer { class Interface; }

namespace driver::factory
{
/**
 * @brief Factory interface with smart pointers.
 */
class Interface
{
public:
    /** 
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Create a new serial instance.
     * 
     * @return Unique pointer to the new serial instance, or nullptr on failure.
     */
    virtual std::unique_ptr<serial::Interface> serial() noexcept = 0;

     /**
     * @brief Create a new timer instance.
     * 
     * @param[in] timeout_ms Timeout in milliseconds.
     * 
     * @return Unique pointer to the new timer instance, or nullptr on failure.
     */
    virtual std::unique_ptr<timer::Interface> timer(std::uint16_t timeout_ms) noexcept = 0;
};
} // namespace driver::factory
