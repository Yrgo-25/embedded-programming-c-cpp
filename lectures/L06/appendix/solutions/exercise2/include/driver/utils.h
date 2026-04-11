/**
 * @brief Driver utilities.
 */
#pragma once

/**
 * @brief Set bit in the given register.
 * 
 * @param[in, out] reg Destination register.
 * @param[in] bit The bit to set.
 */
#define SET(reg, bit) ((reg) |= (1UL << (bit)))

/**
 * @brief Clear bit in the given register.
 * 
 * @param[in, out] reg Destination register.
 * @param[in] bit The bit to clear.
 */
#define CLEAR(reg, bit) ((reg) &= ~(1UL << (bit)))

/**
 * @brief Toggle bit in the given register.
 * 
 * @param[in, out] reg Destination register.
 * @param[in] bit The bit to toggle.
 */
#define TOGGLE(reg, bit) ((reg) ^= (1UL << (bit)))

/**
 * @brief Read bit in the given register.
 * 
 * @param[in] reg Register to read from.
 * @param[in] bit The bit to read.
 * 
 * @return True if the bit is set, false otherwise.
 */
#define READ(reg, bit) static_cast<bool>((reg) & (1UL << (bit)))
