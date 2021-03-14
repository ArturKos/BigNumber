#include "digit.h"

digit::digit()
{
    prev = NULL;
    next = NULL;
    digit8bit = 0;
}
/*
digit::~digit()
{
    //dtor
}*/
digit::digit(uint8_t d)
{
    prev = NULL;
    next = NULL;
    digit8bit = d;
}
void digit::SetData(uint8_t d)
{
 digit8bit = d;
}
uint8_t digit::GetData() const
{
 return digit8bit;
}

digit *digit::GetPrevDigit()
{
 return prev;
}

digit *digit::GetNextDigit()
{
 return next;
}

void digit::SetPrevDigit(digit *d)
{
 prev = d;
}

void digit::SetNextDigit(digit *d)
{
 next = d;
}
bool digit::operator==(digit &d) { return this->GetData() == d.GetData(); }
bool digit::operator!=(digit &d) { return this->GetData() != d.GetData(); }
bool digit::operator>(digit &d)  { return this->GetData()  > d.GetData(); }
bool digit::operator<(digit &d)  { return this->GetData()  < d.GetData(); }
digit& digit::operator = (const digit &obj) { this->SetData(obj.GetData()); }
