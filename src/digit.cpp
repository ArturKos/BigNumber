#include "../include/digit.h"

digit::digit() {
  prev = NULL;
  next = NULL;
  digit8bit = 0;
}

digit::digit(uint8_t value) {
  prev = NULL;
  next = NULL;
  digit8bit = value;
}
void digit::set_data(uint8_t value) { digit8bit = value; }
uint8_t digit::get_data() const { return digit8bit; }

digit *digit::get_prev() { return prev; }

digit *digit::get_next() { return next; }

void digit::set_prev(digit *node) { prev = node; }

void digit::set_next(digit *node) { next = node; }
bool digit::operator==(digit &other) { return this->get_data() == other.get_data(); }
bool digit::operator!=(digit &other) { return this->get_data() != other.get_data(); }
bool digit::operator>(digit &other) { return this->get_data() > other.get_data(); }
bool digit::operator<(digit &other) { return this->get_data() < other.get_data(); }
digit &digit::operator=(const digit &other) { this->set_data(other.get_data()); return *this;}
