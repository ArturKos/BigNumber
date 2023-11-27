#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <bitset>
#include <iostream>
#include <cassert>
#include <time.h>
#include <vector>
#include <iterator>

#include "include/digit.h"
#include "include/bignumber.h"

using namespace std;

//how many numbers to check since checking all 32 bit and 16 bit is would be time consuming
#define MAX_TEST_16_BIT_VARIBLE 1024
#define MAX_TEST_32_BIT_VARIBLE 1024

uint32_t array_32_bits_numbers_to_test[MAX_TEST_32_BIT_VARIBLE] = {0};
uint32_t array_16_bits_numbers_to_test[MAX_TEST_16_BIT_VARIBLE] = {0};

std::vector<int> static_array_16_bits_numbers_to_test = { 0, 1};
std::vector<int> static_array_32_bits_numbers_to_test = { 0, 1};

void prepare_16bit_data() {
   for (int i=0; i<MAX_TEST_16_BIT_VARIBLE; i++){
      //get only numbers > 8 bit
      array_16_bits_numbers_to_test[i] = rand() % (INT16_MAX-INT8_MAX+1) + INT8_MAX;
   }
}

void prepare_32bit_data() {
   for (int i=0; i<MAX_TEST_32_BIT_VARIBLE; i++){
      //get only numbers > 16 bit
      array_32_bits_numbers_to_test[i] = rand() % (INT32_MAX-INT16_MAX+1) + INT16_MAX;
   }
}

//testing 8 bit operators
TEST(TestOperator8bit, operatorPlus) { 
  for (unsigned long a=1; a<UINT8_MAX; a++) 
   for (unsigned long b=1; b<UINT8_MAX; b++){   
    bignumber big_a((uint32_t)a);
    bignumber big_b((uint32_t)b);
    bignumber big_result;
    unsigned long c_result;

    c_result = a + b;
    big_result = big_a + big_b;

    ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << a << endl << "b = " << b << endl
                                                << "a+b=" << c_result << endl << "Lib a+b=" << big_result.toUINT64();
   }
}

TEST(TestOperator8bit, operatorMinus) { 
  for (unsigned long a=1; a<UINT8_MAX; a++) 
   for (unsigned long b=1; b<UINT8_MAX; b++){
      bignumber big_a((uint32_t)a);
      bignumber big_b((uint32_t)b);
      bignumber big_result;
      unsigned long c_result;
      if (a>=b) {
        c_result = a - b;
        big_result = big_a - big_b;
        ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << a << endl << "b = " << b << endl
                                                    << "a-b=" << c_result << endl << "Lib a-b=" << big_result.toUINT64();
      }
   }
}

TEST(TestOperator8bit, operatorMultiplication) { 
  for (unsigned long a=1; a<UINT8_MAX; a++) 
   for (unsigned long b=1; b<UINT8_MAX; b++){
      bignumber big_a((uint32_t)a);
      bignumber big_b((uint32_t)b);
      bignumber big_result;
      unsigned long c_result;
      
      c_result = a * b;
      big_result = big_a * big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << a << endl << "b = " << b << endl
                                                 << "a*b=" << c_result << endl << "Lib a*b=" << big_result.toUINT64();
   }
}

TEST(TestOperator8bit, operatorDivide) { 
  for (unsigned long a=1; a<UINT8_MAX; a++) 
   for (unsigned long b=1; b<UINT8_MAX; b++){
      bignumber big_a((uint32_t)a);
      bignumber big_b((uint32_t)b);
      bignumber big_result;
      unsigned long c_result;
      
      c_result = a / b;
      big_result = big_a / big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << a << endl << "b = " << b << endl
                                                  << "a/b=" << c_result << endl << "Lib a/b=" << big_result.toUINT64();
   }
}

//testing 16 bit operators

TEST(TestOperator16bit, operatorPlus_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_16_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_16_bits_numbers_to_test); b++){   
    bignumber big_a((uint32_t)static_array_16_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)static_array_16_bits_numbers_to_test[b]);
    bignumber big_result;
    unsigned long c_result;

    c_result = static_array_16_bits_numbers_to_test[a] + static_array_16_bits_numbers_to_test[b];
    big_result = big_a + big_b;

    ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                << "a+b=" << c_result << endl << "Lib a+b=" << big_result.toUINT64();
   }
}

TEST(TestOperator16bit, operatorMinus_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_16_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_16_bits_numbers_to_test); b++){   
    bignumber big_a((uint32_t)static_array_16_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)static_array_16_bits_numbers_to_test[b]);
    bignumber big_result;
    unsigned long c_result;

      if (array_16_bits_numbers_to_test[a]>=array_16_bits_numbers_to_test[b]) {
         c_result = static_array_16_bits_numbers_to_test[a] - static_array_16_bits_numbers_to_test[b];
         big_result = big_a - big_b;
         ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                     << "a-b=" << c_result << endl << "Lib a-b=" << big_result.toUINT64();
      }
   }
}

TEST(TestOperator16bit, operatorMultiplication_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_16_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_16_bits_numbers_to_test); b++){   
      bignumber big_a((uint32_t)static_array_16_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)static_array_16_bits_numbers_to_test[b]);
      bignumber big_result;
      unsigned long c_result;

      c_result = static_array_16_bits_numbers_to_test[a] * static_array_16_bits_numbers_to_test[b];
      big_result = big_a * big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                  << "a*b=" << c_result << endl << "Lib a*b=" << big_result.toUINT64();
   }
}

TEST(TestOperator16bit, operatorDivide_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_16_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_16_bits_numbers_to_test); b++){   
      bignumber big_a((uint32_t)static_array_16_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)static_array_16_bits_numbers_to_test[b]);
      bignumber big_result;
      unsigned long c_result;

      c_result = array_16_bits_numbers_to_test[a] / array_16_bits_numbers_to_test[b];
      big_result = big_a / big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                 << "a/b=" << c_result << endl << "Lib a/b=" << big_result.toUINT64();
   }
}

TEST(TestOperator16bit, operatorPlus) { 
  for (unsigned long a=1; a<MAX_TEST_16_BIT_VARIBLE; a++) 
   for (unsigned long b=1; b<MAX_TEST_16_BIT_VARIBLE; b++){   
    bignumber big_a((uint32_t)array_16_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)array_16_bits_numbers_to_test[b]);
    bignumber big_result;
    unsigned long c_result;

    c_result = array_16_bits_numbers_to_test[a] + array_16_bits_numbers_to_test[b];
    big_result = big_a + big_b;

    ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                << "a+b=" << c_result << endl << "Lib a+b=" << big_result.toUINT64();
   }
}

TEST(TestOperator16bit, operatorMinus) { 
  for (unsigned long a=1; a<MAX_TEST_16_BIT_VARIBLE; a++) 
   for (unsigned long b=1; b<MAX_TEST_16_BIT_VARIBLE; b++){   
    bignumber big_a((uint32_t)array_16_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)array_16_bits_numbers_to_test[b]);
    bignumber big_result;
    unsigned long c_result;

      if (array_16_bits_numbers_to_test[a]>=array_16_bits_numbers_to_test[b]) {
         c_result = array_16_bits_numbers_to_test[a] - array_16_bits_numbers_to_test[b];
         big_result = big_a - big_b;
         ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                     << "a-b=" << c_result << endl << "Lib a-b=" << big_result.toUINT64();
      }
   }
}

TEST(TestOperator16bit, operatorMultiplication) { 
  for (unsigned long a=1; a<MAX_TEST_16_BIT_VARIBLE; a++) 
   for (unsigned long b=1; b<MAX_TEST_16_BIT_VARIBLE; b++){   
      bignumber big_a((uint32_t)array_16_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)array_16_bits_numbers_to_test[b]);
      bignumber big_result;
      unsigned long c_result;

      c_result = array_16_bits_numbers_to_test[a] * array_16_bits_numbers_to_test[b];
      big_result = big_a * big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                  << "a*b=" << c_result << endl << "Lib a*b=" << big_result.toUINT64();
   }
}

TEST(TestOperator16bit, operatorDivide) { 
  for (unsigned long a=1; a<MAX_TEST_16_BIT_VARIBLE; a++) 
   for (unsigned long b=1; b<MAX_TEST_16_BIT_VARIBLE; b++){   
      bignumber big_a((uint32_t)array_16_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)array_16_bits_numbers_to_test[b]);
      bignumber big_result;
      unsigned long c_result;

      c_result = array_16_bits_numbers_to_test[a] / array_16_bits_numbers_to_test[b];
      big_result = big_a / big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                 << "a/b=" << c_result << endl << "Lib a/b=" << big_result.toUINT64();
   }
}

//testing 32 bit operators
TEST(TestOperator32bit, operatorPlus_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_32_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_32_bits_numbers_to_test); b++){   
    bignumber big_a((uint32_t)static_array_32_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)static_array_32_bits_numbers_to_test[b]);
    bignumber big_result;
    unsigned long c_result;

    c_result = static_array_32_bits_numbers_to_test[a] + static_array_32_bits_numbers_to_test[b];
    big_result = big_a + big_b;

    ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                               << "a+b=" << c_result << endl << "Lib a+b=" << big_result.toUINT64();
   }
}

TEST(TestOperator32bit, operatorMinus_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_32_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_32_bits_numbers_to_test); b++){   
    bignumber big_a((uint32_t)static_array_32_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)static_array_32_bits_numbers_to_test[b]);
    bignumber big_result;
    unsigned long c_result;

      if (array_32_bits_numbers_to_test[a]>=array_32_bits_numbers_to_test[b]) {
         c_result = static_array_32_bits_numbers_to_test[a] - static_array_32_bits_numbers_to_test[b];
         big_result = big_a - big_b;
         ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                                    << "a-b=" << c_result << endl << "Lib a-b=" << big_result.toUINT64();
      }
   }
}

TEST(TestOperator32bit, operatorMultiplication_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_32_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_32_bits_numbers_to_test); b++){   
      bignumber big_a((uint32_t)static_array_32_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)static_array_32_bits_numbers_to_test[b]);
      bignumber big_result;
      unsigned long c_result;

      c_result = static_array_32_bits_numbers_to_test[a] * static_array_32_bits_numbers_to_test[b];
      big_result = big_a * big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                                 << "a*b=" << c_result << endl << "Lib a*b=" << big_result.toUINT64();
   }
}

TEST(TestOperator32bit, operatorDivide_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_32_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_32_bits_numbers_to_test); b++){   
      bignumber big_a((uint32_t)static_array_32_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)static_array_32_bits_numbers_to_test[b]);
      bignumber big_result;
      unsigned long c_result;

      c_result = static_array_32_bits_numbers_to_test[a] / static_array_32_bits_numbers_to_test[b];
      big_result = big_a / big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                                 << "a/b=" << c_result << endl << "Lib a/b=" << big_result.toUINT64();
   }
}

TEST(TestOperator32bit, operatorPlus) { 
  for (unsigned long a=1; a<MAX_TEST_32_BIT_VARIBLE; a++) 
   for (unsigned long b=1; b<MAX_TEST_32_BIT_VARIBLE; b++){   
    bignumber big_a((uint32_t)array_32_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)array_32_bits_numbers_to_test[b]);
    bignumber big_result;
    unsigned long c_result;

    c_result = array_32_bits_numbers_to_test[a] + array_32_bits_numbers_to_test[b];
    big_result = big_a + big_b;

    ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                               << "a+b=" << c_result << endl << "Lib a+b=" << big_result.toUINT64();
   }
}

TEST(TestOperator32bit, operatorMinus) { 
  for (unsigned long a=1; a<MAX_TEST_32_BIT_VARIBLE; a++) 
   for (unsigned long b=1; b<MAX_TEST_32_BIT_VARIBLE; b++){   
    bignumber big_a((uint32_t)array_32_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)array_32_bits_numbers_to_test[b]);
    bignumber big_result;
    unsigned long c_result;

      if (array_32_bits_numbers_to_test[a]>=array_32_bits_numbers_to_test[b]) {
         c_result = array_32_bits_numbers_to_test[a] - array_32_bits_numbers_to_test[b];
         big_result = big_a - big_b;
         ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                                    << "a-b=" << c_result << endl << "Lib a-b=" << big_result.toUINT64();
      }
   }
}

TEST(TestOperator32bit, operatorMultiplication) { 
  for (unsigned long a=1; a<MAX_TEST_32_BIT_VARIBLE; a++) 
   for (unsigned long b=1; b<MAX_TEST_32_BIT_VARIBLE; b++){   
      bignumber big_a((uint32_t)array_32_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)array_32_bits_numbers_to_test[b]);
      bignumber big_result;
      unsigned long c_result;

      c_result = array_32_bits_numbers_to_test[a] * array_32_bits_numbers_to_test[b];
      big_result = big_a * big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                                 << "a*b=" << c_result << endl << "Lib a*b=" << big_result.toUINT64();
   }
}

TEST(TestOperator32bit, operatorDivide) { 
  for (unsigned long a=1; a<MAX_TEST_32_BIT_VARIBLE; a++) 
   for (unsigned long b=1; b<MAX_TEST_32_BIT_VARIBLE; b++){   
      bignumber big_a((uint32_t)array_32_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)array_32_bits_numbers_to_test[b]);
      bignumber big_result;
      unsigned long c_result;

      c_result = array_32_bits_numbers_to_test[a] / array_32_bits_numbers_to_test[b];
      big_result = big_a / big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                                 << "a/b=" << c_result << endl << "Lib a/b=" << big_result.toUINT64();
   }
}

int main(int argc, char **argv) {
   srand(time(NULL));
   prepare_16bit_data();
   prepare_32bit_data();
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
