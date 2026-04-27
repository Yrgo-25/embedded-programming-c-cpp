/**
 * @brief Serial interface.
 */
#pragma once

#include <cstdint>

namespace driver::serial
{
/**
 * @brief Serial interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Print message.
     * 
     * @param[in] msg Message to print.
     */
    virtual void print(const char* msg) noexcept = 0;

    /**
     * @brief Print buffer data.
     * 
     * @param[in] buf Buffer holding data to print.
     * @param[in] bufLen Buffer length.
     */
    virtual void print(const std::uint8_t* buf, std::uint16_t bufLen) noexcept = 0;
};
} // namespace driver::serial
