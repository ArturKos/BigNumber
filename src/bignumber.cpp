#include "bignumber.h"
#include <stdlib.h>
#include <iostream>
#include <bitset>

bignumber::bignumber()
{
 first_digit = NULL;
 last_digit = NULL;
 negative = false;
 //std::cout <<"big number constructor" << std::endl;
}

bignumber::bignumber(uint8_t d)
{
 first_digit = NULL;
 last_digit = NULL;
 negative = false;
 //first_digit = new digit (d);
 //last_digit =first_digit;
 if(d < 0){
   negative = true;
   d = ~d;
   d+=1;
 }
 push_data(d);
}

bignumber::bignumber(uint16_t d)
{
 first_digit = NULL;
 last_digit = NULL;
 negative = false;
 push_data((uint8_t) d);
 push_data((uint8_t) d >> 8);
}

bignumber::bignumber(uint32_t d)
{
 first_digit = NULL;
 last_digit = NULL;
 negative = false;
 push_data((uint8_t) d);
 push_data((uint8_t) d >> 8);
 push_data((uint8_t) d >> 16);
 push_data((uint8_t) d >> 24);
}

bignumber::~bignumber()
{
 digit *tmp = first_digit, *del;
 if(first_digit==NULL) return;
 if(first_digit==last_digit)
 {
  delete first_digit;
  return;
 }
 while(tmp!=NULL)
 {
  del = tmp;
  tmp = tmp->GetNextDigit();
  delete del;
 }
}
void bignumber::printBIN() const
{
digit *tmp = this->GetLastDigit();
while(tmp!=NULL)
 {
  //uint8_t data = ;
  std::cout << std::bitset<8>((uint8_t)tmp->GetData());
  tmp = tmp->GetPrevDigit();
  //std::cout << "get prev" <<std::endl;
 }
}
void bignumber::printDEC()
{
std::cout << "Print DEC" << std::endl;
 bignumber tmp, dec, reszta, zero((uint8_t)0), dziesiec((uint8_t)10);
 digit *d;
 dec = *this;
 uint8_t l;

 while(dec!=zero)
 {
  reszta = dec % dziesiec;
  dec = dec / dziesiec;
  if(reszta.GetFristDigit()!=NULL){
   d = reszta.GetFristDigit();
   l = d->GetData();
   std::cout << d;
   }
 }
}
bool bignumber::GetNegative()
{
 return negative;
}
void bignumber::SetNegative(bool b)
{
 negative = b;
}

void bignumber::printHEX()
{
 char HEX[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
 digit *tmp = this->GetLastDigit();
 uint8_t data;

 std::cout << "0x";
 while(tmp!=NULL)
  {
  data = tmp->GetData();
  std::cout << HEX[data >> 4];
  data = data << 4;
  data = data >> 4;
  std::cout << HEX[data];
  tmp = tmp->GetPrevDigit();
   //std::cout << "get prev" <<std::endl;
  }
}

digit *bignumber::GetLastDigit() const
{
 return last_digit;
}

digit *bignumber::GetFristDigit() const
{
 return first_digit;
}

void bignumber::SetFristDigit(digit *d)
{
 first_digit = d;
}

void bignumber::SetLastDigit(digit *d)
{
 last_digit = d;
}

void bignumber::push_data(uint8_t d)
{
if(first_digit==NULL)
  {
  // std::cout <<"first_digit==NULL" << std::endl;
   first_digit = new digit(d);
   last_digit = first_digit;
   return;
  }
if(first_digit==last_digit)
  {
  // std::cout <<"first_digit==last_digit" << std::endl;
   last_digit = new digit(d);
   last_digit->SetPrevDigit(first_digit);
   first_digit->SetNextDigit(last_digit);
   return;
  }

  // std::cout <<"else first_digit, last_digit" << std::endl;
   digit *tmp = new digit(d);
   last_digit->SetNextDigit(tmp);
   tmp->SetPrevDigit(last_digit);
   last_digit=tmp;

}

void bignumber::push_data_to_end(uint8_t d)
{

}

void bignumber::not_bits()
{
 digit *tmp = first_digit;

 while(tmp != NULL )
  {
   tmp->SetData(~tmp->GetData());
   tmp = tmp->GetNextDigit();
  }
}

void bignumber::set_minus()
{
 digit *tmp = first_digit;

 while(tmp !=NULL )
  {
   if(tmp->GetNextDigit()==NULL)
    {
      uint8_t d = tmp->GetData();
      d |= 1 << 7;
      tmp->SetData(d);
    }
   tmp = tmp->GetNextDigit();
  }
}
void bignumber::optimize()
{
 digit *tmp = this->GetLastDigit(), *del;
 if(this->GetFristDigit() == this->GetLastDigit()) return;
 while(tmp!=NULL)
 {
  if(tmp->GetData()==0)
   {
    del = tmp;
    tmp = tmp->GetPrevDigit();
    tmp->SetNextDigit(NULL);
    this->SetLastDigit(tmp);
    delete del;
   } else break;
 }

}

void bignumber::rm_all_bits()
{
 digit *tmp = first_digit, *del;

 while(tmp!=NULL)
 {
  del = tmp;
  tmp = tmp->GetNextDigit();
  delete del;
 }
 first_digit = NULL;
 last_digit = NULL;
}

void bignumber::addone() //pomocnicza funkcja do odejmowania zu2, dodaje do liczby 1 ostatni bit jest tracony
{
uint16_t addition= 1;
digit *tmp1 = this->GetFristDigit();

while (tmp1 != NULL)
 {
  addition = tmp1->GetData() + addition;
 // std::cout <<"tmp1->GetData()="<<t1<< " tmp2->GetData()="<<t2<< std::endl;
  tmp1->SetData((uint8_t) addition);
//  std::cout <<"sum="<< sum<< std::endl;
  addition >>= 8;
  tmp1 = tmp1->GetNextDigit();
  //std::cout <<"opertor ++" << std::endl;
 }
}

void bignumber::bitleft()
{
 digit *tmp1 = this->GetFristDigit();
 uint8_t carry = 0,
         data;

 while (tmp1 != NULL)
 {
  data = tmp1->GetData();
  if(carry)
   {
    if ((data >> 7) & 1) carry = 1; else carry = 0;
    data = data << 1;
    data |= 1;
   }else
   {
    if ((data >> 7) & 1) carry = 1; else carry = 0;
    data = data << 1;
   }
 // std::cout <<"tmp1->GetData()="<<t1<< " tmp2->GetData()="<<t2<< std::endl;
  tmp1->SetData((uint8_t) data);
  tmp1 = tmp1->GetNextDigit();
  //std::cout <<"opertor ++" << std::endl;
 }
 if(carry) this->push_data((uint8_t) 1 );
}

void bignumber::bitright()
{
 digit *tmp1 = this->GetLastDigit();
 uint8_t data;
 bool carry;
 carry = false;


 while (tmp1 != NULL)
 {
  data = tmp1->GetData();
  data = data >> 1;
  if(carry) data |= 7;
  if ((tmp1->GetData()) & 1) carry = true; else carry = false;
  tmp1->SetData(data);
  tmp1 = tmp1->GetPrevDigit();
 }
 /*
  data = tmp1->GetData();
  if(carry)
   {
    if ((data >> 6) & 1) carry = 1; else carry = 0;
    data = data << 1;
    data |= 1;
   }else
   {
    if ((data >> 6) & 1) carry = 1; else carry = 0;
    data = data << 1;
   }
 // std::cout <<"tmp1->GetData()="<<t1<< " tmp2->GetData()="<<t2<< std::endl;
  tmp1->SetData((uint8_t) data);
  tmp1 = tmp1->GetNextDigit();
  //std::cout <<"opertor ++" << std::endl;
 }
 if(carry) this->push_data((uint8_t) 1 );
 */
}
// dzielenie według opisu https://eduinf.waw.pl/inf/alg/006_bin/0012.php
bignumber bignumber::operator / (const bignumber &obj)
{
 bignumber dividend, //dzielna i jednoczeście wynik dzielenia
           divider,  //dzielnik
           difference; //dzielnik po wyrównaniu bitów ale
 bignumber wynik_dzielenia((uint8_t) 0);
 dividend = *this;
 divider  = obj;
 dividend.optimize();
 divider.optimize();
 dividend.EqualBits(divider); //wyrównanie bitów, każda liczba tyle samo bitów
 digit *tmp1 = dividend.GetLastDigit() , *tmp2 = divider.GetLastDigit();
 //zrównanie najstarszego bitu dzielnej i dzielnika
 for(register int i = 7; i>=0; i--)
  {
   if( (tmp1->GetData()>> i) & 1)
    {
     while (!( (tmp2->GetData()>> i) & 1)) divider.bitleft();
     break;
    }
  }//std::cout << "Rozpoczynam dzielenie" << std::endl;
 //dividend.printBIN();  std::cout << std::endl;
// divider.printBIN();  std::cout << std::endl;
 bool pass = true;  //zmienna pomocnicza, zapewnia jeszcze jeden przepieg w przypadku kiedy dzielnik
                   //powocil do pierwotnej postaci
 while(divider != obj || pass)
  {
 //   std::cout << "While" << std::endl;
    if(divider == obj) pass = false;
    if(dividend >= divider){
     difference = dividend - divider;//std::cout << "Różnica" << std::endl;
    // difference.printBIN(); std::cout << std::endl;
     dividend = difference; //dividend to reszta z dzielenia
     wynik_dzielenia.onebitpush();
     } else wynik_dzielenia.bitleft();
    if(pass) divider.bitright(); //std::cout << "dzielnik ";
  //  divider.printBIN();  std::cout << std::endl; std::cout << "dzielna  ";
   // dividend.printBIN(); std::cout << std::endl;
  }
 return wynik_dzielenia;
}

void bignumber::onebitpush(){
 if(this->GetFristDigit() == NULL) return;
 this->bitleft();
 digit *tmp1 = this->GetFristDigit();
 uint8_t data = tmp1->GetData();
 data |= 1;
 tmp1->SetData(data);
}
bignumber bignumber::operator % (const bignumber &obj)
{
 bignumber dividend, //dzielna i jednoczeście wynik dzielenia
           divider,  //dzielnik
           difference; //dzielnik po wyrównaniu bitów ale

 dividend = *this;
 divider  = obj;
 dividend.optimize();
 divider.optimize();
 dividend.EqualBits(divider); //wyrównanie bitów, każda liczba tyle samo bitów
 digit *tmp1 = dividend.GetLastDigit() , *tmp2 = divider.GetLastDigit();
 //zrównanie najstarszego bitu dzielnej i dzielnika
 for(register int i = 7; i>=0; i--)
  {
   if( (tmp1->GetData()>> i) & 1)
    {
     while (!( (tmp2->GetData()>> i) & 1)) divider.bitleft();
     break;
    }
  }//std::cout << "Rozpoczynam dzielenie" << std::endl;
 dividend.printBIN();  //std::cout << std::endl;
 divider.printBIN(); // std::cout << std::endl;
 bool pass = true;  //zmienna pomocnicza, zapewnia jeszcze jeden przepieg w przypadku kiedy dzielnik
                   //powocil do pierwotnej postaci
 while(divider != obj || pass)
  {
    //std::cout << "While" << std::endl;
    if(divider == obj) pass = false;
    if(dividend >= divider){
     difference = dividend - divider;//std::cout << "Różnica" << std::endl;
    // difference.printBIN(); std::cout << std::endl;
     dividend = difference;
     }
    if(pass) divider.bitright(); //std::cout << "dzielnik ";
  //  divider.printBIN();  std::cout << std::endl; std::cout << "dzielna  ";
   // dividend.printBIN(); std::cout << std::endl;
  }
 return dividend;
}

bignumber bignumber::operator * (const bignumber &obj)
{
 bignumber res((uint8_t) 0), multiplier; multiplier = obj;
 digit *tmp1 = this->GetFristDigit();
 uint8_t data;

 while(tmp1!=NULL)
 {
  for(register uint8_t i = 0; i < 8; i++)
   {
     data = tmp1->GetData();
     if( (data >> i) & 1)
     res = res + multiplier;
     multiplier.bitleft();
   }
  tmp1 = tmp1->GetNextDigit();
 }
 res.optimize();
 return res;
}

void bignumber::EqualBits(bignumber &b)
{
 digit *tmp1 = this->GetFristDigit(), *tmp2 = b.GetFristDigit();
 while(tmp1!=NULL && tmp2!=NULL) { tmp1=tmp1->GetNextDigit(); tmp2=tmp2->GetNextDigit();}
 while(tmp1!=NULL) { tmp1=tmp1->GetNextDigit(); b.push_data((uint8_t) 0); }
 while(tmp2!=NULL) { tmp2=tmp2->GetNextDigit(); this->push_data((uint8_t) 0); }
}

bignumber bignumber::operator - (const bignumber &obj)
{
 bignumber res;
 bignumber b1;
 b1=*this;
 bignumber b2;
 b2=obj;

 if(b1 == b2)
  return res;

 if(b2.negative){
 res = b1 + b2;
 return res;
 }
 b1.optimize();
 b2.optimize();
 digit *tmp1 = b1.GetFristDigit(), *tmp2 = b2.GetFristDigit();

 //wyrownaie bitow - obie liczby tyle samo bitow
 while(tmp1!=NULL && tmp2!=NULL) { tmp1=tmp1->GetNextDigit(); tmp2=tmp2->GetNextDigit();}
 while(tmp1!=NULL) { tmp1=tmp1->GetNextDigit(); b2.push_data((uint8_t) 0); }
 while(tmp2!=NULL) { tmp2=tmp2->GetNextDigit(); b1.push_data((uint8_t) 0); }
 //przygotowanie liczby b2 do odejmowania w kodzie zu2
 b2.not_bits(); //negacja wszystkich bitow w kodzie zu2
 //b2.printBIN();
 //std::cout <<"not bits"<< std::endl;
 b2.addone(); // dodanie jedynki do liczby w kodzie zu2, ostatni bit jest tracony
// std::cout <<"add one"<< std::endl;
 //b2.printBIN();
 b2.set_minus(); //ustawienie najstarszego bitu liczby na 1
 //std::cout <<"set minus"<< std::endl;
 //b2.printBIN();
 //res = b1 + b2; //odejmowanie

tmp1 = b1.GetFristDigit(); tmp2 = b2.GetFristDigit();
uint16_t sum= 0, t1,t2;
while (tmp1 != NULL && tmp2 != NULL)
 {
 //std::cout <<"sum="<< sum<< std::endl;
  //sum = tmp1->GetData() + tmp2->GetData() + sum;
  t1=tmp1->GetData();
  t2=tmp2->GetData();
   sum = t1 + t2 + sum;
 // std::cout <<"tmp1->GetData()="<<t1<< " tmp2->GetData()="<<t2<< std::endl;
  res.push_data((uint8_t) sum);
//  std::cout <<"sum="<< sum<< std::endl;
  sum >>= 8;
  tmp1 = tmp1->GetNextDigit();
  tmp2 = tmp2->GetNextDigit();
  //std::cout <<"opertor ++" << std::endl;
 }
while (tmp1 != NULL)
 {
 //std::cout <<"sum="<< sum<< std::endl;
  //sum = tmp1->GetData() + tmp2->GetData() + sum;
  t1=tmp1->GetData();
  sum = t1 + sum;
 // std::cout <<"tmp1->GetData()="<<t1<< " tmp2->GetData()="<<t2<< std::endl;
  res.push_data((uint8_t) sum);
//  std::cout <<"sum="<< sum<< std::endl;
  sum >>= 8;
  tmp1 = tmp1->GetNextDigit();
  //std::cout <<"opertor ++" << std::endl;
 }
while (tmp2 != NULL)
 {
 //std::cout <<"sum="<< sum<< std::endl;
  //sum = tmp1->GetData() + tmp2->GetData() + sum;
  t2=tmp2->GetData();
   sum = t2 + sum;
 // std::cout <<"tmp1->GetData()="<<t1<< " tmp2->GetData()="<<t2<< std::endl;
  res.push_data((uint8_t) sum);
//  std::cout <<"sum="<< sum<< std::endl;
  sum >>= 8;
  tmp2 = tmp2->GetNextDigit();
  //std::cout <<"opertor ++" << std::endl;
 }
 if((res.GetLastDigit()->GetData() >> 7) & 1){ //jeżeli ostatni bit liczby równy jest 1 to liczba jest ujemna
    res.SetNegative(true);
    res.not_bits();
    res.addone();
    }
 //std::cout <<"sum"<< std::endl;
 //res.printBIN();
// std::cout << std::endl;
 //res.optimize();
 return res;
}
bignumber bignumber::operator + (const bignumber &obj)
{
bignumber res;
//this->optimize();
//obj.optimize();
digit *tmp1 = this->GetFristDigit(), *tmp2 = obj.GetFristDigit();
uint16_t sum= 0, t1,t2;
while (tmp1 != NULL && tmp2 != NULL)
 {
 //std::cout <<"sum="<< sum<< std::endl;
  //sum = tmp1->GetData() + tmp2->GetData() + sum;
  t1=tmp1->GetData();
  t2=tmp2->GetData();
   sum = t1 + t2 + sum;
 // std::cout <<"tmp1->GetData()="<<t1<< " tmp2->GetData()="<<t2<< std::endl;
  res.push_data((uint8_t) sum);
//  std::cout <<"sum="<< sum<< std::endl;
  sum >>= 8;
  tmp1 = tmp1->GetNextDigit();
  tmp2 = tmp2->GetNextDigit();
  //std::cout <<"opertor ++" << std::endl;
 }
while (tmp1 != NULL)
 {
 //std::cout <<"sum="<< sum<< std::endl;
  //sum = tmp1->GetData() + tmp2->GetData() + sum;
  t1=tmp1->GetData();
  sum = t1 + sum;
 // std::cout <<"tmp1->GetData()="<<t1<< " tmp2->GetData()="<<t2<< std::endl;
  res.push_data((uint8_t) sum);
//  std::cout <<"sum="<< sum<< std::endl;
  sum >>= 8;
  tmp1 = tmp1->GetNextDigit();
  //std::cout <<"opertor ++" << std::endl;
 }
while (tmp2 != NULL)
 {
 //std::cout <<"sum="<< sum<< std::endl;
  //sum = tmp1->GetData() + tmp2->GetData() + sum;
  t2=tmp2->GetData();
   sum = t2 + sum;
 // std::cout <<"tmp1->GetData()="<<t1<< " tmp2->GetData()="<<t2<< std::endl;
  res.push_data((uint8_t) sum);
//  std::cout <<"sum="<< sum<< std::endl;
  sum >>= 8;
  tmp2 = tmp2->GetNextDigit();
  //std::cout <<"opertor ++" << std::endl;
 }
while (sum > 0)
 {
 //std::cout <<"sum="<< sum<< std::endl;
  //sum = tmp1->GetData() + tmp2->GetData() + sum;
 // std::cout <<"tmp1->GetData()="<<t1<< " tmp2->GetData()="<<t2<< std::endl;
  res.push_data((uint8_t) sum);
//  std::cout <<"sum="<< sum<< std::endl;
  sum >>= 8;
  //std::cout <<"opertor ++" << std::endl;
 }
 //res.printBIN();
res.optimize();
return res;
}
bignumber& bignumber::operator = (const bignumber& obj)
{
if (this == &obj) return *this;
digit *tmp1 = obj.GetFristDigit();
this->rm_all_bits();
while (tmp1 != NULL )
 {
  this->push_data((uint8_t) tmp1->GetData());
  tmp1 = tmp1->GetNextDigit();
 }
this->optimize();
return *this;
}

bool bignumber::operator==(const bignumber &obj)
{
 bignumber b1, b2;
 b1 = *this;
 b2 = obj;
 b1.optimize();
 b2.optimize();
 digit *tmp1 = b1.GetFristDigit(), *tmp2 = b2.GetFristDigit();
 uint8_t data1, data2;
//std::cout <<"opertor ==" << std::endl;
//b1.printBIN(); std::cout << std::endl;
//b2.printBIN(); std::cout << std::endl;
 while(tmp1 != NULL && tmp2!=NULL)
 {/* Line 541 */
   data1 = tmp1->GetData();
   data2 = tmp2->GetData();
   if(data1 != data2)
     return false;
   tmp1 = tmp1->GetNextDigit();
   tmp2 = tmp2->GetNextDigit();
 }
 if((tmp1 == NULL && tmp2 != NULL) ||(tmp2 == NULL && tmp1 != NULL))
   return false;
 return true;
}
bool bignumber::operator!=(const bignumber &obj)
{//std::cout <<"opertor !=" << std::endl;
  return !(*this == obj);
}
bool bignumber::operator>=(const bignumber &obj)
{//std::cout <<"opertor >=" << std::endl;
 bignumber b1, b2;
 b1 = *this;
 b2 = obj;
 b1.optimize();
 b2.optimize();
 digit *tmp1 = b1.GetFristDigit(), *tmp2 = b2.GetFristDigit();
 uint8_t data1, data2;

 if(b1 == b2)
   return true;
 //obj->optimize();
 //this->optimize();

  while(tmp1 != NULL && tmp2!=NULL)//sprawdzam czy liczby mają tyle samo bitów
  {
   tmp1 = tmp1->GetNextDigit();
   tmp2 = tmp2->GetNextDigit();
  }
  if(tmp2 == NULL && tmp1 != NULL)
   return true; else //pierwsza liczba ma więcej niezerowych bitów wiec jest większa
  if(tmp1 == NULL && tmp2 != NULL) //druga liczba ma więcej niezerowych bitów więc jest większa
   return false;

   tmp1 = this->GetLastDigit(); //zaczynamy sprawdzanie od ostatnich najstarszych bitów
   tmp2 = obj.GetLastDigit(); //w tym miejscu obie lczby mają tyle samo niezerowych bitów
 while(tmp1 != NULL && tmp2 != NULL)
 {
   data1 = tmp1->GetData(); //std::cout <<"opertor >= while(tmp1 != NULL && tmp2 != NULL)" << std::endl;
   data2 = tmp2->GetData();
   if(data1 < data2)
     return false;
   tmp1 = tmp1->GetPrevDigit();
   tmp2 = tmp2->GetPrevDigit();
 }
 return true;
}
bool bignumber::operator<=(bignumber &obj){}
bool bignumber::operator<(bignumber &obj){}
bool bignumber::operator>(bignumber &obj){}

void bignumber::operator++()
{
 bignumber add((uint8_t) 1);
 *this = *this + add;
}

void bignumber::operator--()
{
  bignumber add((uint8_t) 1);
 *this = *this - add;
}
