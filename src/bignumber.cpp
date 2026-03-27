#include "../include/bignumber.h"

#include <stdlib.h>
#include <string>
#include <bitset>
#include <iostream>
#include <algorithm>
#include <cstdint>

#define bitset(byte,nbit)   ((byte) |=  (1<<(nbit)))
#define bitclear(byte,nbit) ((byte) &= ~(1<<(nbit)))
#define bitflip(byte,nbit)  ((byte) ^=  (1<<(nbit)))
#define bitcheck(byte,nbit) ((byte) &   (1<<(nbit)))

bignumber::bignumber() {
  first_digit = NULL;
  last_digit = NULL;
  negative = false;
}

uint64_t bignumber::to_uint64() const {
  uint64_t result = 0;
  digit *current = this->get_last_digit();

  while (current != NULL) {
    result = (result << 8) | current->get_data();
    current = current->get_prev();
  }
  return result;
}

bignumber::bignumber(const bignumber& other) {
  if (this == &other) return;
  first_digit = NULL;
  last_digit = NULL;
  negative = other.negative;
  digit *source = other.get_first_digit();
  clear();
  while (source != NULL) {
    push_data((uint8_t)source->get_data());
    source = source->get_next();
  }
  optimize();
}

bignumber::bignumber(uint8_t value) {
  first_digit = NULL;
  last_digit = NULL;
  negative = false;
  push_data(value);
}

bignumber::bignumber(uint16_t value) {
  first_digit = NULL;
  last_digit = NULL;
  negative = false;
  push_data( (value >> 0) & 0xFF);
  push_data( (value >> 8) & 0xFF);
}

bignumber::bignumber(uint32_t value) {
  first_digit = NULL;
  last_digit = NULL;
  negative = false;
  push_data( (value >> 0) & 0xFF);
  push_data( (value >> 8) & 0xFF);
  push_data( (value >> 16) & 0xFF);
  push_data( (value >> 24) & 0xFF);
}

bignumber::~bignumber() {
  digit *current = first_digit, *to_delete;
  if (first_digit == NULL) return;
  if (first_digit == last_digit) {
    delete first_digit;
    return;
  }
  while (current != NULL) {
    to_delete = current;
    current = current->get_next();
    delete to_delete;
  }
}

std::string bignumber::to_string_bin() const {
  std::string result;
  digit *current = this->get_last_digit();

  if (current == NULL) return "0";
  if (this->negative) result = "-";

  bool leading_zeros = true;
  while (current != NULL) {
    uint8_t byte_val = current->get_data();
    for (int bit_pos = 7; bit_pos >= 0; bit_pos--) {
      if ((byte_val >> bit_pos) & 1) {
        result += '1';
        leading_zeros = false;
      } else if (!leading_zeros) {
        result += '0';
      }
    }
    current = current->get_prev();
  }
  if (leading_zeros) result += '0';
  return result;
}

std::string bignumber::to_string_dec() const {
  bignumber remaining(*this);
  bignumber zero((uint8_t)0), ten((uint8_t)10);
  std::string result;

  remaining.negative = false;
  remaining.optimize();

  if (remaining == zero) return "0";

  while (remaining != zero) {
    bignumber remainder = remaining % ten;
    uint8_t digit_val = 0;
    if (remainder.get_first_digit() != NULL)
      digit_val = remainder.get_first_digit()->get_data();
    result += ('0' + digit_val);
    remaining = remaining / ten;
  }
  if (this->negative) result += '-';
  std::reverse(result.begin(), result.end());
  return result;
}

std::string bignumber::to_string_hex() const {
  const char HEX_CHARS[] = "0123456789ABCDEF";
  std::string result = "0x";
  digit *current = this->get_last_digit();

  if (current == NULL) return "0x0";
  if (this->negative) result = "-0x";

  bool leading_zeros = true;
  while (current != NULL) {
    uint8_t byte_val = current->get_data();
    uint8_t high_nibble = byte_val >> 4;
    uint8_t low_nibble = byte_val & 0x0F;
    if (high_nibble != 0 || !leading_zeros) {
      result += HEX_CHARS[high_nibble];
      leading_zeros = false;
    }
    if (low_nibble != 0 || !leading_zeros) {
      result += HEX_CHARS[low_nibble];
      leading_zeros = false;
    }
    current = current->get_prev();
  }
  if (leading_zeros) result += '0';
  return result;
}

void bignumber::print_bin() const {
  std::cout << to_string_bin();
}

void bignumber::print_dec() const {
  std::cout << to_string_dec() << std::endl;
}

void bignumber::print_hex() const {
  std::cout << to_string_hex();
}

std::ostream& operator<<(std::ostream &os, const bignumber &num) {
  os << num.to_string_dec();
  return os;
}

bool bignumber::is_negative() { return negative; }
void bignumber::set_negative(bool value) { negative = value; }

digit *bignumber::get_last_digit() const { return last_digit; }

digit *bignumber::get_first_digit() const { return first_digit; }

void bignumber::set_first_digit(digit *node) { first_digit = node; }

void bignumber::set_last_digit(digit *node) { last_digit = node; }

void bignumber::push_data(uint8_t value) {
  if (first_digit == NULL) {
    first_digit = new digit(value);
    last_digit = first_digit;
    return;
  }
  if (first_digit == last_digit) {
    last_digit = new digit(value);
    last_digit->set_prev(first_digit);
    first_digit->set_next(last_digit);
    return;
  }
  digit *new_node = new digit(value);
  last_digit->set_next(new_node);
  new_node->set_prev(last_digit);
  last_digit = new_node;
}

void bignumber::push_data_to_end(uint8_t value) {}

void bignumber::not_bits() {
  digit *current = first_digit;

  while (current != NULL) {
    current->set_data(~current->get_data());
    current = current->get_next();
  }
}

void bignumber::set_minus() {
  digit *current = first_digit;

  while (current != NULL) {
    if (current->get_next() == NULL) {
      uint8_t byte_val = current->get_data();
      byte_val |= 1 << 7;
      current->set_data(byte_val);
    }
    current = current->get_next();
  }
}

void bignumber::optimize() {
  digit *current = this->get_last_digit(), *to_delete;
  if (this->get_first_digit() == this->get_last_digit()) return;

  while (current != NULL && current != this->get_first_digit() && current->get_data() == 0) {
      to_delete = current;
      current = current->get_prev();
      current->set_next(NULL);
      this->set_last_digit(current);
      delete to_delete;
  }
}

void bignumber::clear() {
  digit *current = first_digit, *to_delete;

  while (current != NULL) {
    to_delete = current;
    current = current->get_next();
    delete to_delete;
  }
  first_digit = NULL;
  last_digit = NULL;
}

void bignumber::add_one() {
  uint16_t carry = 1;
  digit *current = this->get_first_digit();

  while (current != NULL) {
    carry = current->get_data() + carry;
    current->set_data((uint8_t)carry);
    carry >>= 8;
    if (0 == carry) return;
    current = current->get_next();
  }
}

void bignumber::shift_left() {
  digit *current = this->get_first_digit();
  uint8_t carry = 0, byte_val;

  while (current != NULL) {
    byte_val = current->get_data();
    if (carry) {
      if ((byte_val >> 7) & 1)
        carry = 1;
      else
        carry = 0;
      byte_val = byte_val << 1;
      byte_val |= 1;
    } else {
      if ((byte_val >> 7) & 1)
        carry = 1;
      else
        carry = 0;
      byte_val = byte_val << 1;
    }
    current->set_data((uint8_t)byte_val);
    current = current->get_next();
  }
  if (carry) this->push_data((uint8_t)1);
}

void bignumber::shift_right() {
  digit *current = this->get_last_digit();
  uint8_t byte_val;
  if (this->get_first_digit() == NULL) return;
  if (this->get_first_digit() == this->get_last_digit()) {
    byte_val = current->get_data() >> 1;
    current->set_data(byte_val);
    return;
  }
  current = this->get_first_digit();
  while (current != NULL) {
    byte_val = current->get_data();
    byte_val = byte_val >> 1;
    if (NULL != current->get_next()) {
      digit *next_digit = current->get_next();
      if (bitcheck(next_digit->get_data(), 0)) {
        bitset(byte_val, 7);
      }
    }
    current->set_data(byte_val);
    current = current->get_next();
  }
}

bignumber bignumber::operator/(const bignumber &obj) {
  bignumber zero((uint8_t)0), one((uint8_t)1);

  // Division by zero guard
  if (zero == obj) return zero;

  bignumber dividend, divisor;
  dividend = *this;
  divisor = obj;

  // Determine result sign, then work with absolute values
  bool result_negative = (this->negative != obj.negative);
  dividend.negative = false;
  divisor.negative = false;
  dividend.optimize();
  divisor.optimize();

  // Early returns
  if (dividend == zero) return zero;
  if (divisor == one) {
    dividend.negative = result_negative;
    return dividend;
  }
  if (dividend < divisor) return zero;
  if (dividend == divisor) {
    one.negative = result_negative;
    return one;
  }

  // Shift-and-subtract division algorithm
  bignumber quotient((uint8_t)0);
  bignumber shifted_divisor = divisor;
  bignumber bit_position((uint8_t)1);

  // Shift divisor and bit_position left together until next shift would exceed dividend
  bignumber probe = shifted_divisor;
  probe.shift_left();
  probe.optimize();
  while (dividend >= probe) {
    shifted_divisor.shift_left();
    bit_position.shift_left();
    probe = shifted_divisor;
    probe.shift_left();
    probe.optimize();
  }

  // Subtract from most significant bit down to least
  do {
    if (dividend >= shifted_divisor) {
      dividend = dividend - shifted_divisor;
      quotient = quotient + bit_position;
    }
    bit_position.shift_right();
    bit_position.optimize();
    shifted_divisor.shift_right();
    shifted_divisor.optimize();
  } while (bit_position != zero);

  quotient.negative = result_negative;
  return quotient;
}

void bignumber::shift_left_and_set_lsb() {
  if (this->get_first_digit() == NULL) return;
  this->shift_left();
  digit *lsb_digit = this->get_first_digit();
  uint8_t byte_val = lsb_digit->get_data();
  byte_val = bitset(byte_val, 0);
  lsb_digit->set_data(byte_val);
}

bignumber bignumber::operator%(const bignumber &obj) {
  bignumber zero((uint8_t)0), one((uint8_t)1);

  // Division by zero guard
  if (zero == obj) return zero;

  bignumber dividend, divisor;
  dividend = *this;
  divisor = obj;

  // Work with absolute values
  dividend.negative = false;
  divisor.negative = false;
  dividend.optimize();
  divisor.optimize();

  // Early returns
  if (dividend == zero) return zero;
  if (divisor == one) return zero;
  if (dividend < divisor) return dividend;
  if (dividend == divisor) return zero;

  // Shift-and-subtract to find remainder efficiently
  bignumber shifted_divisor = divisor;
  bignumber bit_position((uint8_t)1);

  bignumber probe = shifted_divisor;
  probe.shift_left();
  probe.optimize();
  while (dividend >= probe) {
    shifted_divisor.shift_left();
    bit_position.shift_left();
    probe = shifted_divisor;
    probe.shift_left();
    probe.optimize();
  }

  do {
    if (dividend >= shifted_divisor) {
      dividend = dividend - shifted_divisor;
    }
    bit_position.shift_right();
    bit_position.optimize();
    shifted_divisor.shift_right();
    shifted_divisor.optimize();
  } while (bit_position != zero);

  return dividend;
}

bignumber bignumber::operator*(const bignumber &obj) {
  bignumber product((uint8_t)0), shifted_operand;
  shifted_operand = obj;
  digit *current = this->get_first_digit();
  uint8_t byte_val;

  while (current != NULL) {
    for (uint8_t bit_pos = 0; bit_pos < 8; bit_pos++) {
      byte_val = current->get_data();
      if ((byte_val >> bit_pos) & 1) product = product + shifted_operand;
      shifted_operand.shift_left();
    }
    current = current->get_next();
  }
  product.optimize();
  if (this->negative != obj.negative) product.negative = true;
  return product;
}

void bignumber::equalize_bits(bignumber &other) {
  digit *this_cursor = this->get_first_digit(), *other_cursor = other.get_first_digit();
  while (this_cursor != NULL && other_cursor != NULL) {
    this_cursor = this_cursor->get_next();
    other_cursor = other_cursor->get_next();
  }
  while (this_cursor != NULL) {
    this_cursor = this_cursor->get_next();
    other.push_data((uint8_t)0);
  }
  while (other_cursor != NULL) {
    other_cursor = other_cursor->get_next();
    this->push_data((uint8_t)0);
  }
}

bignumber bignumber::operator-(const bignumber &obj) {
  bignumber result;
  bignumber left;
  left = *this;
  bignumber right;
  right = obj;

  bignumber zero((uint8_t)0);
  right.optimize();
  if (right == zero) return left;

  if (left == right && !left.negative && !right.negative) {
    // (+a) - (+a) = 0
    return bignumber((uint8_t)0);
  } else
  if (left == right && left.negative && !right.negative) {
    // (-a) - (+a) = -(a + a)
    result = left + right;
    result.negative = true;
    return result;
  } else
  if (left == right && left.negative && right.negative) {
    // (-a) - (-a) = 0
    return bignumber((uint8_t)0);
  } else
  if (left.negative && !right.negative) {
    // (-a) - (+b) = -(a + b)
    result = left + right;
    result.negative = true;
    return result;
  }
  left.optimize();
  right.optimize();

  digit *left_cursor = left.get_first_digit(), *right_cursor = right.get_first_digit();

  left.equalize_bits(right);

  // Prepare right operand for two's complement subtraction
  right.not_bits();
  right.add_one();
  left_cursor = left.get_first_digit();
  right_cursor = right.get_first_digit();
  uint16_t carry = 0, left_byte, right_byte;
  while (left_cursor != NULL && right_cursor != NULL) {
    left_byte = left_cursor->get_data();
    right_byte = right_cursor->get_data();
    carry = left_byte + right_byte + carry;
    result.push_data((uint8_t)carry);
    carry >>= 8;
    left_cursor = left_cursor->get_next();
    right_cursor = right_cursor->get_next();
  }
  while (left_cursor != NULL) {
    left_byte = left_cursor->get_data();
    carry = left_byte + carry;
    result.push_data((uint8_t)carry);
    carry >>= 8;
    left_cursor = left_cursor->get_next();
  }
  while (right_cursor != NULL) {
    right_byte = right_cursor->get_data();
    carry = right_byte + carry;
    result.push_data((uint8_t)carry);
    carry >>= 8;
    right_cursor = right_cursor->get_next();
  }

  return result;
}

bignumber bignumber::operator+(const bignumber &obj) {
  bignumber result;
  digit *left_cursor = this->get_first_digit(), *right_cursor = obj.get_first_digit();
  uint16_t carry = 0, left_byte, right_byte; //TODO handle +left + (-right)
  while (left_cursor != NULL && right_cursor != NULL) {
    left_byte = left_cursor->get_data();
    right_byte = right_cursor->get_data();
    carry = left_byte + right_byte + carry;
    result.push_data((uint8_t)carry);
    carry >>= 8;
    left_cursor = left_cursor->get_next();
    right_cursor = right_cursor->get_next();
  }
  while (left_cursor != NULL) {
    left_byte = left_cursor->get_data();
    carry = left_byte + carry;
    result.push_data((uint8_t)carry);
    carry >>= 8;
    left_cursor = left_cursor->get_next();
  }
  while (right_cursor != NULL) {
    right_byte = right_cursor->get_data();
    carry = right_byte + carry;
    result.push_data((uint8_t)carry);
    carry >>= 8;
    right_cursor = right_cursor->get_next();
  }
  while (carry > 0) {
    result.push_data((uint8_t)carry);
    carry >>= 8;
  }
  result.optimize();
  return result;
}

bignumber& bignumber::operator=(const bignumber &other) {
  if (this == &other) return *this;
  this->negative = other.negative;
  digit *source = other.get_first_digit();
  this->clear();
  while (source != NULL) {
    this->push_data((uint8_t)source->get_data());
    source = source->get_next();
  }
  this->optimize();
  return *this;
}

bool bignumber::operator==(const bignumber &obj) {
  bignumber left, right;
  left = *this;
  right = obj;
  left.optimize();
  right.optimize();
  digit *left_cursor = left.get_first_digit(), *right_cursor = right.get_first_digit();
  uint8_t left_byte, right_byte;
  while (left_cursor != NULL && right_cursor != NULL) {
    left_byte = left_cursor->get_data();
    right_byte = right_cursor->get_data();
    if (left_byte != right_byte) return false;
    left_cursor = left_cursor->get_next();
    right_cursor = right_cursor->get_next();
  }
  if ((left_cursor == NULL && right_cursor != NULL) || (right_cursor == NULL && left_cursor != NULL))
    return false;
  return true;
}

bool bignumber::operator!=(const bignumber &obj) {
  return !(*this == obj);
}

bool bignumber::operator>=(const bignumber &obj) {
  bignumber left, right;
  left = *this;
  right = obj;
  left.optimize();
  right.optimize();
  digit *left_cursor = left.get_first_digit(), *right_cursor = right.get_first_digit();
  uint8_t left_byte, right_byte;

  if (left == right) return true;

  // Check if both numbers have the same number of bytes
  while (left_cursor != NULL && right_cursor != NULL) {
    left_cursor = left_cursor->get_next();
    right_cursor = right_cursor->get_next();
  }
  if (right_cursor == NULL && left_cursor != NULL)
    return true;   // left has more bytes, so it is larger
  else if (left_cursor == NULL && right_cursor != NULL)
    return false;  // right has more bytes, so it is larger

  // Both have the same byte count — compare from most significant byte down
  left_cursor = this->get_last_digit();
  right_cursor = obj.get_last_digit();
  while (left_cursor != NULL && right_cursor != NULL) {
    left_byte = left_cursor->get_data();
    right_byte = right_cursor->get_data();
    if (left_byte < right_byte) return false;
    if (left_byte > right_byte) return true;
    left_cursor = left_cursor->get_prev();
    right_cursor = right_cursor->get_prev();
  }
  return true;
}

bool bignumber::operator<(bignumber &obj) {
  bignumber left, right;
  left = *this;
  right = obj;
  return (!(left >= right));
}

bool bignumber::operator<=(bignumber &obj) {
  return !(obj < *this);
}

bool bignumber::operator>(bignumber &obj) {
  return obj < *this;
}

void bignumber::operator++() {
  bignumber one((uint8_t)1);
  *this = *this + one;
}

void bignumber::operator--() {
  bignumber one((uint8_t)1);
  *this = *this - one;
}

bool bignumber::is_prime() {
  bignumber zero((uint8_t)0), one((uint8_t)1), two((uint8_t)2), three((uint8_t)3);
  bignumber five((uint8_t)5), six((uint8_t)6);

  if (this->negative) return false;

  bignumber value = *this;
  value.optimize();

  if (value < two) return false;
  if (value == two || value == three) return true;
  if ((value % two) == zero) return false;
  if ((value % three) == zero) return false;

  // Check divisors of the form 6k-1 and 6k+1 up to sqrt(value)
  bignumber divisor = five;
  while ((divisor * divisor) < value || (divisor * divisor) == value) {
    if ((value % divisor) == zero) return false;
    bignumber divisor_plus_two = divisor + two;
    if ((value % divisor_plus_two) == zero) return false;
    divisor = divisor + six;
  }

  return true;
}

bignumber bignumber::isqrt(bignumber *unused_param) {
  (void)unused_param;
  bignumber zero((uint8_t)0), one((uint8_t)1);

  if (this->negative) return zero;

  bignumber value = *this;
  value.optimize();

  if (value == zero) return zero;
  if (value == one) return one;

  // Binary search: find largest root where root*root <= value
  // Upper bound: shift 1 left by ceil(bits(value)/2)
  bignumber upper_bound((uint8_t)1);
  bignumber halved_value = value;
  halved_value.shift_right();
  halved_value.optimize();
  while (halved_value != zero) {
    upper_bound.shift_left();
    halved_value.shift_right();
    halved_value.optimize();
  }

  bignumber lower_bound((uint8_t)1);
  bignumber result = lower_bound;

  while (lower_bound < upper_bound || lower_bound == upper_bound) {
    bignumber midpoint = lower_bound + upper_bound;
    midpoint.shift_right();
    midpoint.optimize();

    if (midpoint == zero) break;

    bignumber square = midpoint * midpoint;

    if (square == value) return midpoint;
    if (value < square) {
      upper_bound = midpoint - one;
    } else {
      result = midpoint;
      lower_bound = midpoint + one;
    }
  }

  return result;
}
