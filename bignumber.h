#ifndef BIGNUMBER_H
#define BIGNUMBER_H
#include "digit.h"

class bignumber
{
    public:
        bignumber();
        bignumber(uint8_t d);
        bignumber(uint16_t d);
        bignumber(uint32_t d);
        bignumber(uint64_t d);
        virtual ~bignumber();
        void printHEX();
        void printDEC();
        void printBIN();
        bool GetNegative();
        void SetNegative(bool b);
        digit *GetLastDigit() const;
        digit *GetFristDigit() const;
        void SetFristDigit(digit *d);
        void SetLastDigit(digit *d);
        void not_bits();
        void set_minus();
        void optimize();
        void rm_all_bits();
        void addone();
        void bitleft();
        void bitright();
        void EqualBits(bignumber &b); //tyle samo bitów w obu liczbach
        bignumber operator + (const bignumber &obj);
        bignumber operator - (const bignumber &obj);
        bignumber operator * (const bignumber &obj);
        bignumber operator / (const bignumber &obj);
      //  Foo& Foo::operator= (Foo const& ex)
        bignumber& operator = (const bignumber &obj);

        bool operator==(const bignumber &obj);
        bool operator>=(const bignumber &obj);
        bool operator<=(bignumber &obj);
        bool operator<(bignumber &obj);
        bool operator>(bignumber &obj);
        void operator++();
        void operator--();
        bool operator!=(const bignumber &obj);

    protected:

    private:
    digit *first_digit, *last_digit;
    bool negative;
    void push_data(uint8_t d);
    void push_data_to_end(uint8_t d);
};

#endif // BIGNUMBER_H
