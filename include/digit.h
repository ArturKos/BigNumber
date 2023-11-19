#ifndef DIGIT_H
#define DIGIT_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class digit {
 public:
  digit();
  digit(uint8_t d);
  void SetData(uint8_t d);
  uint8_t GetData() const;
  digit *GetPrevDigit();
  digit *GetNextDigit();
  void SetPrevDigit(digit *d);
  void SetNextDigit(digit *d);
  bool operator==(digit &d);
  bool operator!=(digit &d);
  bool operator>(digit &d);
  bool operator<(digit &d);
  digit &operator=(const digit &obj);

 protected:
 private:
  digit *prev, *next;
  uint8_t digit8bit;
};

#endif  // DIGIT_H
