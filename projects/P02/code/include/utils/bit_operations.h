/**
 * @brief Utility functions for bit operations.
 */
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils/type_traits.h"

namespace bit
{
/**
 * @brief Set a bit of the given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 * 
 * @param[in, out] reg Reference to the register to write to.
 * @param[in] bit The bit to set.
 */
template <typename T = uint8_t>
constexpr void set(volatile T& reg, uint8_t bit) noexcept;

/**
 * @brief Set bits of the given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 * @tparam Bits Type for additional bits to set.
 *
 * @param[in, out] reg Reference to the register to write to.
 * @param[in] bit The first bit to set.
 * @param[in] bits The other bits to set.
 */
template <typename T = uint8_t, typename... Bits>
constexpr void set(volatile T& reg, uint8_t bit, Bits... bits) noexcept;

/**
 * @brief Clear a bit of the given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 * 
 * @param[in, out] reg Reference to the register to write to.
 * @param[in] bit The bit to clear.
 */
template <typename T = uint8_t>
constexpr void clear(volatile T& reg, uint8_t bit) noexcept;

/**
 * @brief Clear bits of the given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 * @tparam Bits Type for additional bits to set.
 * 
 * @param[in, out] reg Reference to the register to write to.
 * @param[in] bit The first bit to clear.
 * @param[in] bits The other bits to clear.
 */
template <typename T = uint8_t, typename... Bits>
constexpr void clear(volatile T& reg, uint8_t bit, Bits... bits) noexcept;

/**
 * @brief Toggle a bit of the given register.
 * 
 * @tparam T The register type. Must be of unsigned type.
 * 
 * @param[in, out] reg Reference to the register to write to.
 * @param[in] bit The bit to toggle.
 */
template <typename T = uint8_t>
constexpr void toggle(volatile T& reg, uint8_t bit) noexcept;

/**
 * @brief Toggle bits of the given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 * @tparam Bits Type for additional bits to set.
 * 
 * @param[in, out] reg Reference to the register to write to.
 * @param[in] bit The first bit to toggle.
 * @param[in] bits The other bits to toggle.
 */
template <typename T = uint8_t, typename... Bits>
constexpr void toggle(volatile T& reg, uint8_t bit, Bits... bits) noexcept;

/**
 * @brief Read a bit of the given register.
 * 
 * @tparam T The register type. Must be of unsigned type.
 *
 * @param[in, out] reg Reference to the register to read from.
 * @param[in] bit The given bit to read.
 *
 * @return True if the bit is set, false otherwise.
 */
template <typename T = uint8_t>
constexpr bool read(const volatile T& reg, uint8_t bit) noexcept;

/**
 * @brief Read bits of the given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 * @tparam Bits Type for additional bits to set.
 * 
 * @param[in, out] reg Reference to the register to read from.
 * @param[in] bit The first bit to read.
 * @param[in] bits The other bits to read.
 *
 * @return True if at least one of bits is set, false otherwise.
 */
template <typename T = uint8_t, typename... Bits>
constexpr bool read(const volatile T& reg, uint8_t bit, Bits... bits) noexcept;

// -----------------------------------------------------------------------------
// Implementation details.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template <typename T>
constexpr void set(volatile T& reg, const uint8_t bit) noexcept
{
    static_assert(type_traits::is_unsigned<T>::value, "Invalid data type used for bit operation!");
    reg |= (static_cast<T>(1U) << bit);
}

// -----------------------------------------------------------------------------
template <typename T, typename... Bits>
constexpr void set(volatile T& reg, const uint8_t bit, const Bits... bits) noexcept
{
    static_assert(type_traits::is_unsigned<T>::value, "Invalid data type used for bit operation!");
    set(reg, bit);
    set(reg, (bits)...);
}

// -----------------------------------------------------------------------------
template <typename T>
constexpr void clear(volatile T& reg, const uint8_t bit) noexcept  
{
    static_assert(type_traits::is_unsigned<T>::value, "Invalid data type used for bit operation!");
    reg &= ~(static_cast<T>(1U) << bit);
}

// -----------------------------------------------------------------------------
template <typename T, typename... Bits>
constexpr void clear(volatile T& reg, const uint8_t bit, const Bits... bits) noexcept  
{
    static_assert(type_traits::is_unsigned<T>::value, "Invalid data type used for bit operation!");
    clear(reg, bit);
    clear(reg, (bits)...);
}

// -----------------------------------------------------------------------------
template <typename T>
constexpr void toggle(volatile T& reg, const uint8_t bit) noexcept
{
    static_assert(type_traits::is_unsigned<T>::value, "Invalid data type used for bit operation!");
    reg ^= (static_cast<T>(1U) << bit);
}

// -----------------------------------------------------------------------------
template <typename T, typename... Bits>
constexpr void toggle(volatile T& reg, const uint8_t bit, const Bits... bits) noexcept  
{
    toggle(reg, bit);
    toggle(reg, (bits)...);
}

// -----------------------------------------------------------------------------
template <typename T>
constexpr bool read(const volatile T& reg, const uint8_t bit) noexcept  
{
    static_assert(type_traits::is_unsigned<T>::value, "Invalid data type used for bit operation!");
    return reg & (static_cast<T>(1U) << bit);
}

// -----------------------------------------------------------------------------
template <typename T, typename... Bits>
constexpr bool read(const volatile T& reg, const uint8_t bit, Bits... bits) noexcept 
{
    static_assert(type_traits::is_unsigned<T>::value, "Invalid data type used for bit operation!");
    return read(reg, bit) | read(reg, (bits)...);
}
} // namespace bit
