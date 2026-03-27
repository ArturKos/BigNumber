/**
 * @file digit.h
 * @brief Single 8-bit digit node for the bignumber doubly-linked list.
 *
 * Each digit object stores one byte (8 bits) of a big number and maintains
 * pointers to the previous (less significant) and next (more significant)
 * digit in the chain.
 */

#ifndef DIGIT_H
#define DIGIT_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @class digit
 * @brief A node in a doubly-linked list representing one byte of a big number.
 *
 * Digits are chained from least significant (first) to most significant (last).
 * Each node holds an 8-bit unsigned value and pointers to its neighbours.
 */
class digit {
 public:
  /** @brief Default constructor. Initializes data to 0, pointers to NULL. */
  digit();

  /**
   * @brief Construct a digit with the given 8-bit value.
   * @param value The byte value to store.
   */
  digit(uint8_t value);

  /**
   * @brief Set the 8-bit data value.
   * @param value The byte value to store.
   */
  void set_data(uint8_t value);

  /**
   * @brief Get the stored 8-bit data value.
   * @return The byte value held by this digit.
   */
  uint8_t get_data() const;

  /**
   * @brief Get the previous (less significant) digit in the list.
   * @return Pointer to the previous digit, or NULL if this is the first.
   */
  digit *get_prev();

  /**
   * @brief Get the next (more significant) digit in the list.
   * @return Pointer to the next digit, or NULL if this is the last.
   */
  digit *get_next();

  /**
   * @brief Set the previous digit pointer.
   * @param node Pointer to the previous digit node.
   */
  void set_prev(digit *node);

  /**
   * @brief Set the next digit pointer.
   * @param node Pointer to the next digit node.
   */
  void set_next(digit *node);

  /** @brief Equality comparison by data value. */
  bool operator==(digit &d);

  /** @brief Inequality comparison by data value. */
  bool operator!=(digit &d);

  /** @brief Greater-than comparison by data value. */
  bool operator>(digit &d);

  /** @brief Less-than comparison by data value. */
  bool operator<(digit &d);

  /**
   * @brief Assignment operator. Copies the data value only.
   * @param obj The digit to copy data from.
   * @return Reference to this digit.
   */
  digit &operator=(const digit &obj);

 protected:
 private:
  digit *prev;       ///< Pointer to the previous (less significant) digit.
  digit *next;       ///< Pointer to the next (more significant) digit.
  uint8_t digit8bit; ///< The 8-bit data value stored in this digit.
};

#endif  // DIGIT_H
