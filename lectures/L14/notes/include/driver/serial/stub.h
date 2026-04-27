/**
 * @brief Serial driver stub.
 */
#pragma once

#include <cstdint>
#include <cstdio>

#include "driver/serial/interface.h"

namespace driver::serial
{
/**
 * @brief Serial driver stub.
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
     * @brief Print message.
     * 
     * @param[in] msg Message to print.
     */
    void print(const char* msg) noexcept override
    {
        // Print message if valid.
        if (nullptr != msg) { std::printf("%s", msg); }
    }

    /**
     * @brief Print buffer data.
     * 
     * @param[in] buf Buffer holding data to print.
     * @param[in] bufLen Buffer length.
     */
    void print(const std::uint8_t* buf, const std::uint16_t bufLen) noexcept override
    {
        // Check buffer, terminate if invalid.
        if ((nullptr == buf) || (0U == bufLen)) { return; }

        // Write each byte one by one.
        for (std::uint16_t i{}; i < bufLen; ++i) 
        { 
            std::printf("%u", buf[i]); 
        }
    }

    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment.
    Stub& operator=(Stub&&)      = delete; // No move assignment.
};
} // namespace driver::serial
