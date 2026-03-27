/**
 * @file bignumber.h
 * @brief Arbitrary-precision unsigned integer arithmetic library.
 *
 * The bignumber class implements integers of arbitrary size using a
 * doubly-linked list of 8-bit digit nodes. Each node stores one byte,
 * chained from least significant (first) to most significant (last).
 *
 * Supported operations:
 * - Construction from uint8_t, uint16_t, uint32_t, and copy construction.
 * - Arithmetic: addition, subtraction, multiplication, division, modulo.
 * - Comparison: ==, !=, <, <=, >, >=.
 * - Bit manipulation: shift left, shift right, bitwise NOT.
 * - Number theory: primality testing (trial division), integer square root.
 * - Output: binary, decimal, and hexadecimal string conversion and printing.
 *
 * @note Division and modulo use a shift-and-subtract algorithm (O(bits^2)).
 *       Multiplication uses shift-and-add. Primality testing uses trial
 *       division with the 6k+/-1 optimization.
 */

#ifndef BIGNUMBER_H
#define BIGNUMBER_H
#include "../include/digit.h"
#include <string>
#include <iostream>

/**
 * @class bignumber
 * @brief Arbitrary-precision integer stored as a doubly-linked list of bytes.
 *
 * Internally, the number is represented as a doubly-linked list of digit nodes,
 * each holding 8 bits. The list is ordered from least significant byte (first_digit)
 * to most significant byte (last_digit). A separate boolean flag tracks the sign.
 */
class bignumber {
 public:
  /** @brief Default constructor. Initializes to zero. */
  bignumber();

  /**
   * @brief Copy constructor. Deep-copies all digits from another bignumber.
   * @param other The bignumber to copy.
   */
  bignumber(const bignumber& other);

  /**
   * @brief Construct from an 8-bit unsigned integer.
   * @param value Value to initialize with (0-255).
   */
  bignumber(uint8_t value);

  /**
   * @brief Construct from a 16-bit unsigned integer.
   * @param value Value to initialize with (0-65535).
   */
  bignumber(uint16_t value);

  /**
   * @brief Construct from a 32-bit unsigned integer.
   * @param value Value to initialize with (0-4294967295).
   */
  bignumber(uint32_t value);

  /**
   * @brief Construct from a 64-bit unsigned integer.
   * @param value Value to initialize with.
   * @note Currently declared but not implemented.
   */
  bignumber(uint64_t value);

  /** @brief Destructor. Frees all digit nodes in the linked list. */
  virtual ~bignumber();

  /**
   * @brief Convert to a 64-bit unsigned integer.
   *
   * Reconstructs the value by shifting and OR-ing bytes from MSB to LSB.
   * @return The numeric value, truncated to 64 bits if the bignumber exceeds that range.
   */
  uint64_t to_uint64() const;

  /**
   * @brief Convert to a binary string representation.
   *
   * Returns the number in binary without leading zeros.
   * Negative numbers are prefixed with '-'.
   * @return Binary string (e.g. "10110", "-101").
   */
  std::string to_string_bin() const;

  /**
   * @brief Convert to a decimal string representation.
   *
   * Uses repeated division by 10 to extract digits.
   * Negative numbers are prefixed with '-'.
   * @return Decimal string (e.g. "42", "-17").
   */
  std::string to_string_dec() const;

  /**
   * @brief Convert to a hexadecimal string representation.
   *
   * Returns the number in uppercase hex with "0x" prefix, no leading zeros.
   * Negative numbers are prefixed with "-0x".
   * @return Hex string (e.g. "0x2A", "-0xFF").
   */
  std::string to_string_hex() const;

  /**
   * @brief Print the number in hexadecimal to stdout.
   * @see to_string_hex()
   */
  void print_hex() const;

  /**
   * @brief Print the number in decimal to stdout, followed by a newline.
   * @see to_string_dec()
   */
  void print_dec() const;

  /**
   * @brief Print the number in binary to stdout.
   * @see to_string_bin()
   */
  void print_bin() const;

  /**
   * @brief Stream insertion operator. Outputs the decimal representation.
   * @param os The output stream.
   * @param num The bignumber to output.
   * @return Reference to the output stream.
   */
  friend std::ostream& operator<<(std::ostream &os, const bignumber &num);

  /**
   * @brief Check the sign flag.
   * @return True if the number is negative, false otherwise.
   */
  bool is_negative();

  /**
   * @brief Set the sign flag.
   * @param value True for negative, false for positive.
   */
  void set_negative(bool value);

  /**
   * @brief Get the last (most significant) digit in the linked list.
   * @return Pointer to the last digit node, or NULL if empty.
   */
  digit *get_last_digit() const;

  /**
   * @brief Get the first (least significant) digit in the linked list.
   * @return Pointer to the first digit node, or NULL if empty.
   */
  digit *get_first_digit() const;

  /**
   * @brief Set the first digit pointer.
   * @param node Pointer to the new first digit node.
   */
  void set_first_digit(digit *node);

  /**
   * @brief Set the last digit pointer.
   * @param node Pointer to the new last digit node.
   */
  void set_last_digit(digit *node);

  /** @brief Bitwise NOT. Flips all bits in every digit of the number. */
  void not_bits();

  /** @brief Set the most significant bit of the most significant byte to 1. */
  void set_minus();

  /**
   * @brief Remove leading zero bytes from the most significant end.
   *
   * Frees and unlinks digit nodes with value 0 from the tail of the list,
   * stopping when only one digit remains or a non-zero byte is found.
   */
  void optimize();

  /** @brief Free all digit nodes and reset the number to an empty state. */
  void clear();

  /**
   * @brief Add 1 to the number in place, discarding any overflow.
   *
   * Used internally for two's complement subtraction.
   * Carries propagate through the digit chain; overflow past the MSB is lost.
   */
  void add_one();

  /**
   * @brief Shift all bits left by one position (multiply by 2).
   *
   * Carries propagate from LSB to MSB. If the MSB overflows,
   * a new digit node is appended.
   */
  void shift_left();

  /**
   * @brief Shift all bits right by one position (integer divide by 2).
   *
   * Carries propagate from MSB to LSB. The least significant bit is lost.
   * @note Does not remove leading zero bytes; call optimize() afterward if needed.
   */
  void shift_right();

  /**
   * @brief Shift left by one bit and set the least significant bit to 1.
   *
   * Equivalent to (n << 1) | 1. Used for building numbers bit by bit.
   */
  void shift_left_and_set_lsb();

  /**
   * @brief Test whether this number is a prime.
   *
   * Uses trial division with the 6k+/-1 optimization, testing divisors
   * up to floor(sqrt(n)). Returns false for negative numbers and values < 2.
   * @return True if the number is prime, false otherwise.
   */
  bool is_prime();

  /**
   * @brief Fast exponentiation.
   * @param exponent Pointer to the exponent.
   * @return this^(*exponent).
   * @note Currently declared but not implemented.
   */
  bignumber power(bignumber *exponent);

  /**
   * @brief Compute the integer (floor) square root.
   *
   * Uses binary search to find the largest r such that r*r <= *this.
   * Returns 0 for negative numbers.
   * @param unused Reserved for future use (pass NULL).
   * @return floor(sqrt(*this)).
   */
  bignumber isqrt(bignumber *unused);

  /**
   * @brief Pad both numbers with zero bytes so they have equal byte length.
   *
   * Appends zero-valued digit nodes to the shorter number until both
   * linked lists have the same number of nodes.
   * @param other The other bignumber to equalize with.
   */
  void equalize_bits(bignumber &other);

  /**
   * @brief Addition operator.
   *
   * Adds byte by byte from LSB to MSB with carry propagation.
   * @param other The number to add.
   * @return The sum as a new bignumber.
   */
  bignumber operator+(const bignumber &other);

  /**
   * @brief Subtraction operator.
   *
   * Uses two's complement method: negates the subtrahend, then adds.
   * Handles sign combinations for negative operands.
   * @param other The number to subtract.
   * @return The difference as a new bignumber.
   */
  bignumber operator-(const bignumber &other);

  /**
   * @brief Multiplication operator.
   *
   * Uses shift-and-add (binary long multiplication). Iterates through
   * each bit of *this; for each set bit, adds the appropriately shifted
   * multiplier to the result. Time complexity: O(bits^2).
   * @param other The number to multiply by.
   * @return The product as a new bignumber.
   */
  bignumber operator*(const bignumber &other);

  /**
   * @brief Division operator (integer division).
   *
   * Uses shift-and-subtract (binary long division). Works with absolute
   * values and applies the correct sign to the result.
   * Returns 0 for division by zero.
   * Time complexity: O(bits^2).
   * @param other The divisor.
   * @return The quotient as a new bignumber.
   */
  bignumber operator/(const bignumber &other);

  /**
   * @brief Modulo operator.
   *
   * Uses the same shift-and-subtract algorithm as division but returns
   * the remainder instead of the quotient.
   * Returns 0 for division by zero.
   * @param other The divisor.
   * @return The remainder as a new bignumber.
   */
  bignumber operator%(const bignumber &other);

  /**
   * @brief Assignment operator. Deep-copies all digits from another bignumber.
   * @param other The bignumber to copy.
   * @return Reference to this bignumber.
   */
  bignumber& operator=(const bignumber &other);

  /**
   * @brief Equality comparison.
   *
   * Optimizes both operands and compares byte by byte.
   * @param other The number to compare with.
   * @return True if both numbers have identical digit sequences.
   */
  bool operator==(const bignumber &other);

  /**
   * @brief Greater-than-or-equal comparison.
   *
   * Compares by byte count first, then by byte values from MSB to LSB.
   * @param other The number to compare with.
   * @return True if *this >= other.
   */
  bool operator>=(const bignumber &other);

  /**
   * @brief Less-than-or-equal comparison.
   * @param other The number to compare with.
   * @return True if *this <= other.
   */
  bool operator<=(bignumber &other);

  /**
   * @brief Less-than comparison.
   * @param other The number to compare with.
   * @return True if *this < other.
   */
  bool operator<(bignumber &other);

  /**
   * @brief Greater-than comparison.
   * @param other The number to compare with.
   * @return True if *this > other.
   */
  bool operator>(bignumber &other);

  /** @brief Prefix increment. Adds 1 to the number. */
  void operator++();

  /** @brief Prefix decrement. Subtracts 1 from the number. */
  void operator--();

  /**
   * @brief Inequality comparison.
   * @param other The number to compare with.
   * @return True if the numbers differ.
   */
  bool operator!=(const bignumber &other);

 protected:
 private:
  digit *first_digit; ///< Pointer to the least significant byte.
  digit *last_digit;  ///< Pointer to the most significant byte.
  bool negative;      ///< Sign flag: true if the number is negative.

  /**
   * @brief Append a byte to the most significant end of the digit list.
   * @param value The byte value to append.
   */
  void push_data(uint8_t value);

  /**
   * @brief Append a byte to the least significant end of the digit list.
   * @param value The byte value to append.
   * @note Currently declared but not implemented.
   */
  void push_data_to_end(uint8_t value);
};

#endif  // BIGNUMBER_H
