#include "../include/bignumber.h"

#include <stdlib.h>
#include <string>
#include <bitset>
#include <iostream>
#include <algorithm>


bignumber::bignumber() {
  first_digit = NULL;
  last_digit = NULL;
  negative = false;
}
uint64_t bignumber::toUINT64(){
  char num_to_convert[2048] = {'\0'};
  long return_value = 0;
  uint64_t char_idx = 0;
  digit *tmp = this->GetLastDigit();

  while (tmp != NULL) {
      std::bitset<8> data((uint8_t)tmp->GetData());
    	for (int i=7; i>=0; i--)
        {
          if (data.test(i))
            num_to_convert[char_idx] = '1';
          else
            num_to_convert[char_idx] = '0';
          if(char_idx<2048) char_idx++;
          //if(i==0) break;
        }
    tmp = tmp->GetPrevDigit();
  }
  return_value = strtol(num_to_convert, NULL, 2);
  return return_value;
}
bignumber::bignumber(const bignumber& obj) {

  if (this == &obj) return;
  first_digit = NULL;
  last_digit = NULL;
  negative = obj.negative;
  digit *tmp1 = obj.GetFristDigit();
  rm_all_bits();
  while (tmp1 != NULL) {
    push_data((uint8_t)tmp1->GetData());
    tmp1 = tmp1->GetNextDigit();
  }
  optimize();
}

bignumber::bignumber(uint8_t d) {
  first_digit = NULL;
  last_digit = NULL;
  negative = false;
/*
  if (d < 0) {
    negative = true;
    d = ~d;
    d += 1;
  }
*/
  push_data(d);
}

bignumber::bignumber(uint16_t d) {
  first_digit = NULL;
  last_digit = NULL;
  negative = false;
  push_data( (d >> 0) & 0xFF);
  push_data( (d >> 8) & 0xFF);
}

bignumber::bignumber(uint32_t d) {
  first_digit = NULL;
  last_digit = NULL;
  negative = false;
  push_data( (d >> 0) & 0xFF);
  push_data( (d >> 8) & 0xFF);
  push_data( (d >> 16) & 0xFF);
  push_data( (d >> 24) & 0xFF);
}

bignumber::~bignumber() {
  digit *tmp = first_digit, *del;
  if (first_digit == NULL) return;
  if (first_digit == last_digit) {
    delete first_digit;
    return;
  }
  while (tmp != NULL) {
    del = tmp;
    tmp = tmp->GetNextDigit();
    delete del;
  }
}
void bignumber::printBIN() const {
  digit *tmp = this->GetLastDigit();
  while (tmp != NULL) {
    std::cout << std::bitset<8>((uint8_t)tmp->GetData());
    tmp = tmp->GetPrevDigit();
  }
}
void bignumber::printDEC() {
  bignumber tmp, dec(*this), reszta, zero((uint8_t)0), dziesiec((uint8_t)10), jeden((uint8_t)1), tmp_dec((uint8_t)0);
  digit *d;
  std::string decimal_number = "";
  char decimal_numbers[] = "0123456789";

  while (dec != zero) {
    reszta = dec % dziesiec; //std::cout << "tmp_dec="; tmp_dec.printHEX(); std::cout << std::endl;
    if (reszta.GetFristDigit() != NULL) {
      decimal_number.push_back(decimal_numbers[reszta.GetFristDigit()->GetData()]);
    }
    if(dec == zero) decimal_number.push_back(decimal_numbers[0]);
    //std::cout << "reszta="; reszta.printHEX(); std::cout << std::endl;
    dec = dec / dziesiec; 
    //std::cout << "dec="; dec.printHEX(); std::cout << std::endl;
  }
  reverse(decimal_number.begin(), decimal_number.end());
  std::cout << decimal_number << std::endl;
}
bool bignumber::GetNegative() { return negative; }
void bignumber::SetNegative(bool b) { negative = b; }

void bignumber::printHEX() {
  char HEX[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  digit *tmp = this->GetLastDigit();
  uint8_t data;

  std::cout << "0x";
  while (tmp != NULL) {
    data = tmp->GetData();
    std::cout << HEX[data >> 4];
    data = data << 4;
    data = data >> 4;
    std::cout << HEX[data];
    tmp = tmp->GetPrevDigit();
  }
}

digit *bignumber::GetLastDigit() const { return last_digit; }

digit *bignumber::GetFristDigit() const { return first_digit; }

void bignumber::SetFristDigit(digit *d) { first_digit = d; }

void bignumber::SetLastDigit(digit *d) { last_digit = d; }

void bignumber::push_data(uint8_t d) {
  if (first_digit == NULL) {
    first_digit = new digit(d);
    last_digit = first_digit;
    return;
  }
  if (first_digit == last_digit) {
    last_digit = new digit(d);
    last_digit->SetPrevDigit(first_digit);
    first_digit->SetNextDigit(last_digit);
    return;
  }
  digit *tmp = new digit(d);
  last_digit->SetNextDigit(tmp);
  tmp->SetPrevDigit(last_digit);
  last_digit = tmp;
}

void bignumber::push_data_to_end(uint8_t d) {}

void bignumber::not_bits() {
  digit *tmp = first_digit;
  //std::cout << "before_not_bits="; this->printBIN(); std::cout << std::endl;

  while (tmp != NULL) {
    tmp->SetData(~tmp->GetData());
    tmp = tmp->GetNextDigit();
  } 
  //std::cout << "not_bits=       "; this->printBIN(); std::cout << std::endl;  
}

void bignumber::set_minus() {
  digit *tmp = first_digit;
    std::cout << "before_setminus() =       "; this->printBIN(); std::cout << std::endl; 

  while (tmp != NULL) {
    if (tmp->GetNextDigit() == NULL) {
      uint8_t d = tmp->GetData();
      d |= 1 << 7;
      tmp->SetData(d);
    }
    tmp = tmp->GetNextDigit();
  }
      std::cout << "after_setminus()  =       "; this->printBIN(); std::cout << std::endl; 
}
void bignumber::optimize() {
  digit *tmp = this->GetLastDigit(), *del;
  if (this->GetFristDigit() == this->GetLastDigit()) return;
  //std::cout << "before optimise  =       "; this->printBIN(); std::cout << std::endl;

  while (tmp != NULL && tmp != this->GetFristDigit() && tmp->GetData() == 0) {
      del = tmp;
      tmp = tmp->GetPrevDigit();
      tmp->SetNextDigit(NULL);
      this->SetLastDigit(tmp);
      delete del;
  }
    //std::cout << "after optimise   =       "; this->printBIN(); std::cout << std::endl;
}

void bignumber::rm_all_bits() {
  digit *tmp = first_digit, *del;

  while (tmp != NULL) {
    del = tmp;
    tmp = tmp->GetNextDigit();
    delete del;
  }
  first_digit = NULL;
  last_digit = NULL;
}

void bignumber::addone()  // pomocnicza funkcja do odejmowania zu2, dodaje do
                          // liczby 1 ostatni bit jest tracony
{     //std::cout << "before_addone()=       "; this->printBIN(); std::cout << std::endl;
  *this = *this + bignumber((uint8_t)1);
    //std::cout << "after_addone() =       "; this->printBIN(); std::cout << std::endl;
 /* uint16_t addition = 1;
  digit *tmp1 = this->GetFristDigit();

  while (tmp1 != NULL) {
    addition = tmp1->GetData() + addition;
    tmp1->SetData((uint8_t)addition);
    addition >>= 8;
    tmp1 = tmp1->GetNextDigit();
  }*/
}

void bignumber::bitleft() {
  digit *tmp1 = this->GetFristDigit();
  uint8_t carry = 0, data;

  while (tmp1 != NULL) {
    data = tmp1->GetData();
    if (carry) {
      if ((data >> 7) & 1)
        carry = 1;
      else
        carry = 0;
      data = data << 1;
      data |= 1;
    } else {
      if ((data >> 7) & 1)
        carry = 1;
      else
        carry = 0;
      data = data << 1;
    }
    tmp1->SetData((uint8_t)data);
    tmp1 = tmp1->GetNextDigit();
  }
  if (carry) this->push_data((uint8_t)1);
}

void bignumber::bitright() {
  digit *tmp1 = this->GetLastDigit();
  uint8_t data;
  bool carry;
  carry = false;

  while (tmp1 != NULL) {
    data = tmp1->GetData();
    data = data >> 1;
    if (carry) data |= 7;
    if ((tmp1->GetData()) & 1)
      carry = true;
    else
      carry = false;
    tmp1->SetData(data);
    tmp1 = tmp1->GetPrevDigit();
  }
}
// dzielenie według opisu https://eduinf.waw.pl/inf/alg/006_bin/0012.php
bignumber bignumber::operator/(const bignumber &obj) {
   bignumber dividend,  // dzielna i jednoczeście wynik dzielenia
             divider;   // dzielnik
  bignumber wynik_dzielenia((uint8_t)0), jeden((uint8_t)1), zero((uint8_t)0);
  dividend = *this;
  divider = obj;
  dividend.optimize();
  divider.optimize();
  dividend.EqualBits(divider);  // wyrównanie bitów, każda liczba tyle samo
                                // bitów
  if(dividend < divider) return wynik_dzielenia; else
  if(dividend == zero) return wynik_dzielenia; else// dzielna jest równa 0 zwracamy 0
  if(dividend == divider) { // dzielna i dzielnik są równe, zwracamy 1 lub -1
      if (this->negative != obj.negative) jeden.negative = true;
      return jeden;
  } 
  if(divider == jeden) return dividend;


  digit *tmp1 = dividend.GetLastDigit(), *tmp2 = divider.GetLastDigit();
  // zrównanie najstarszego bitu dzielnej i dzielnika
  for (int i = 7; i >= 0; i--) {
    if ((tmp1->GetData() >> i) & 1) {
      while (!((tmp2->GetData() >> i) & 1)) divider.bitleft();
      break;
    }
  }  
  bool pass = true;  // zmienna pomocnicza, zapewnia jeszcze jeden przebieg w
                     // przypadku kiedy dzielnik powocil do pierwotnej postaci
  while (divider != obj || pass) {
    if (divider == obj) pass = false;
    if (dividend >= divider) {
      dividend = dividend - divider; 
      wynik_dzielenia.onebitpush();
    } else
      wynik_dzielenia.bitleft();
    if (pass) divider.bitright();
  }
  if (this->negative != obj.negative) wynik_dzielenia.negative = true;
  return wynik_dzielenia;
}

void bignumber::onebitpush() {
  if (this->GetFristDigit() == NULL) return;
  this->bitleft();
  digit *tmp1 = this->GetFristDigit();
  uint8_t data = tmp1->GetData();
  data |= 1;
  tmp1->SetData(data);
}
bignumber bignumber::operator%(const bignumber &obj) {
   bignumber dividend,  // dzielna i jednoczeście wynik dzielenia
             divider;   // dzielnik
  bignumber jeden((uint8_t)1), zero((uint8_t)0);
  dividend = *this;
  divider = obj;
  dividend.optimize();
  divider.optimize();
  dividend.EqualBits(divider);  // wyrównanie bitów, każda liczba tyle samo
                                // bitów
  if(dividend < divider) return dividend; else
  if(dividend == zero) return zero; else// dzielna jest równa 0 zwracamy 0
  if(dividend == divider) return zero; else// dzielna i dzielnik są równe, zwracamy 0
  if(divider == jeden) return zero;
  digit *tmp1 = dividend.GetLastDigit(), *tmp2 = divider.GetLastDigit();
  // zrównanie najstarszego bitu dzielnej i dzielnika
  for (int i = 7; i >= 0; i--) {
    if ((tmp1->GetData() >> i) & 1) {
      while (!((tmp2->GetData() >> i) & 1)) divider.bitleft();
      break;
    }
  }  
  bool pass = true;  // zmienna pomocnicza, zapewnia jeszcze jeden przepieg w
                     // przypadku kiedy dzielnik powocil do pierwotnej postaci
  while (divider != obj || pass) {
    if (divider == obj) pass = false;
    if (dividend >= divider) {
      dividend = dividend - divider; 
    }
    if (pass) divider.bitright();
  }
  return dividend; //reszta z dzielenia
}

bignumber bignumber::operator*(const bignumber &obj) {
  bignumber res((uint8_t)0), multiplier;
  multiplier = obj;
  digit *tmp1 = this->GetFristDigit();
  uint8_t data;

  while (tmp1 != NULL) {
    for (uint8_t i = 0; i < 8; i++) {
      data = tmp1->GetData();
      if ((data >> i) & 1) res = res + multiplier;
      multiplier.bitleft();
    }
    tmp1 = tmp1->GetNextDigit();
  }
  res.optimize();
  if (this->negative != obj.negative) res.negative = true;
  return res;
}

void bignumber::EqualBits(bignumber &b) {
  digit *tmp1 = this->GetFristDigit(), *tmp2 = b.GetFristDigit();
  while (tmp1 != NULL && tmp2 != NULL) {
    tmp1 = tmp1->GetNextDigit();
    tmp2 = tmp2->GetNextDigit();
  }
  while (tmp1 != NULL) {
    tmp1 = tmp1->GetNextDigit();
    b.push_data((uint8_t)0);
  }
  while (tmp2 != NULL) {
    tmp2 = tmp2->GetNextDigit();
    this->push_data((uint8_t)0);
  }
}

bignumber bignumber::operator-(const bignumber &obj) {
  bignumber res;
  bignumber b1;
  b1 = *this;
  bignumber b2;
  b2 = obj;

  if (b1 == b2 && !b1.negative && !b2.negative) {  //+b1 - (+b2), abs(b1) = abs(b2)
    return bignumber((uint8_t)0);
  }else 
  if (b1 == b2 &&  b1.negative && !b2.negative) {  //-b1 -(+b2) = -b1 - b2, abs(b1) = abs(b2)
    res = b1 + b2;
    res.negative = true;
    return res;
  }else
  if (b1 == b2 &&  b1.negative && b2.negative) {  //-b1 -(-b2) = -b1 + b2, abs(b1) = abs(b2)
    return bignumber((uint8_t)0);
  }else
  if (b1.negative && !b2.negative) {  //-b1 - (+b2) = -b1 - b2
    res = b1 + b2;
    res.negative = true;
    return res;
  }
  b1.optimize();
  b2.optimize();
  //b1.printBIN(); std::cout << std::endl;
  //b2.printBIN(); std::cout << std::endl;
  digit *tmp1 = b1.GetFristDigit(), *tmp2 = b2.GetFristDigit();

  b1.EqualBits(b2);
  //b1.printBIN(); std::cout << std::endl;
 // b2.printBIN(); std::cout << std::endl;
  // przygotowanie liczby b2 do odejmowania w kodzie zu2
  b2.not_bits();  // negacja wszystkich bitow w kodzie zu2
  b2.addone();  // dodanie jedynki do liczby w kodzie zu2, ostatni bit jest
                // tracony
  //b2.set_minus();  // ustawienie najstarszego bitu liczby na 1
  tmp1 = b1.GetFristDigit();
  tmp2 = b2.GetFristDigit();
  uint16_t sum = 0, t1, t2;
  while (tmp1 != NULL && tmp2 != NULL) {
    t1 = tmp1->GetData();
    t2 = tmp2->GetData();
    sum = t1 + t2 + sum;
    res.push_data((uint8_t)sum);
    sum >>= 8;
    tmp1 = tmp1->GetNextDigit();
    tmp2 = tmp2->GetNextDigit();
  }
  while (tmp1 != NULL) {
    t1 = tmp1->GetData();
    sum = t1 + sum;
    res.push_data((uint8_t)sum);
    sum >>= 8;
    tmp1 = tmp1->GetNextDigit();
  }
  while (tmp2 != NULL) {
    t2 = tmp2->GetData();
    sum = t2 + sum;
    res.push_data((uint8_t)sum);
    sum >>= 8;
    tmp2 = tmp2->GetNextDigit();
  } /*
  if ((res.GetLastDigit()->GetData() >> 7) & 1) {  // jeżeli ostatni bit liczby równy jest 1 to liczba jest ujemna
    res.SetNegative(true);
    res.not_bits();
    res.addone();
  }*/
  return res;
}
bignumber bignumber::operator+(const bignumber &obj) {
  bignumber res;
  digit *tmp1 = this->GetFristDigit(), *tmp2 = obj.GetFristDigit();
  uint16_t sum = 0, t1, t2; //TODO warible +t1 + (-t2)
  while (tmp1 != NULL && tmp2 != NULL) {
    t1 = tmp1->GetData();
    t2 = tmp2->GetData();
    sum = t1 + t2 + sum;
    res.push_data((uint8_t)sum);
    sum >>= 8;
    tmp1 = tmp1->GetNextDigit();
    tmp2 = tmp2->GetNextDigit();
  }
  while (tmp1 != NULL) {
    t1 = tmp1->GetData();
    sum = t1 + sum;
    res.push_data((uint8_t)sum);
    sum >>= 8;
    tmp1 = tmp1->GetNextDigit();
  }
  while (tmp2 != NULL) {
    t2 = tmp2->GetData();
    sum = t2 + sum;
    res.push_data((uint8_t)sum);
    sum >>= 8;
    tmp2 = tmp2->GetNextDigit();
  }
  while (sum > 0) {
    res.push_data((uint8_t)sum);
    sum >>= 8;
  }
  res.optimize();
  return res;
}

bignumber& bignumber::operator=(const bignumber &obj) {
  if (this == &obj) return *this;
  this->negative = obj.negative;
  digit *tmp1 = obj.GetFristDigit();
  this->rm_all_bits();
  while (tmp1 != NULL) {
    this->push_data((uint8_t)tmp1->GetData());
    tmp1 = tmp1->GetNextDigit();
  }
  this->optimize();
  return *this;
}

bool bignumber::operator==(const bignumber &obj) {
  bignumber b1, b2;
  b1 = *this;
  b2 = obj;
  b1.optimize();
  b2.optimize();
  digit *tmp1 = b1.GetFristDigit(), *tmp2 = b2.GetFristDigit();
  uint8_t data1, data2;
  while (tmp1 != NULL && tmp2 != NULL) { 
    data1 = tmp1->GetData();
    data2 = tmp2->GetData();
    if (data1 != data2) return false;
    tmp1 = tmp1->GetNextDigit();
    tmp2 = tmp2->GetNextDigit();
  }
  if ((tmp1 == NULL && tmp2 != NULL) || (tmp2 == NULL && tmp1 != NULL))
    return false;
  return true;
}
bool bignumber::operator!=(const bignumber &obj) {  
  return !(*this == obj);
}
bool bignumber::operator>=(
    const bignumber &obj) { 
  bignumber b1, b2;
  b1 = *this;
  b2 = obj;
  b1.optimize();
  b2.optimize();
  digit *tmp1 = b1.GetFristDigit(), *tmp2 = b2.GetFristDigit();
  uint8_t data1, data2;

  if (b1 == b2) return true;
  
  while (tmp1 != NULL &&
         tmp2 != NULL)  // sprawdzam czy liczby mają tyle samo bitów
  {
    tmp1 = tmp1->GetNextDigit();
    tmp2 = tmp2->GetNextDigit();
  }
  if (tmp2 == NULL && tmp1 != NULL)
    return true;
  else  // pierwsza liczba ma więcej niezerowych bitów wiec jest większa
      if (tmp1 == NULL && tmp2 != NULL)  // druga liczba ma więcej niezerowych
                                         // bitów więc jest większa
    return false;

  tmp1 = this->GetLastDigit();  // zaczynamy sprawdzanie od ostatnich
                                // najstarszych bitów
  tmp2 = obj.GetLastDigit();  // w tym miejscu obie lczby mają tyle samo
                              // niezerowych bitów
  while (tmp1 != NULL && tmp2 != NULL) {
    data1 = tmp1->GetData();  // std::cout <<"opertor >= while(tmp1 != NULL &&
                              // tmp2 != NULL)" << std::endl;
    data2 = tmp2->GetData();
    if (data1 < data2) return false;
    tmp1 = tmp1->GetPrevDigit();
    tmp2 = tmp2->GetPrevDigit();
  }
  return true;
}
bool bignumber::operator<(bignumber &obj) { 
  bignumber b1, b2;
  b1 = *this;
  b2 = obj;
  
  return (!(b1>=b2));
}

bool bignumber::operator<=(bignumber &obj) {}

bool bignumber::operator>(bignumber &obj) {}

void bignumber::operator++() {
  bignumber add((uint8_t)1);
  *this = *this + add;
}

void bignumber::operator--() {
  bignumber add((uint8_t)1);
  *this = *this - add;
}
