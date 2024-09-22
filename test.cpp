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
#include <random>

#include "include/digit.h"
#include "include/bignumber.h"

using namespace std;

//how many numbers to check since checking all 32 bit and 16 bit is would be time consuming
#define MAX_TEST_16_BIT_VARIBLE 1024
#define MAX_TEST_32_BIT_VARIBLE 1024
#define MAX_TEST_BIT_OPERATION 63

uint64_t array_32_bits_numbers_to_test[MAX_TEST_32_BIT_VARIBLE] = {0};
uint64_t array_16_bits_numbers_to_test[MAX_TEST_16_BIT_VARIBLE] = {0};

std::vector<int> static_array_16_bits_numbers_to_test = { 0, 1};
std::vector<int> static_array_32_bits_numbers_to_test = { 0, 1};

void prepare_16bit_data() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(INT8_MAX, INT16_MAX);
   for (int i=0; i<MAX_TEST_16_BIT_VARIBLE; i++){
      //get only numbers > 8 bit
      array_16_bits_numbers_to_test[i] = distrib(gen);
      //array_16_bits_numbers_to_test[i] = rand() % (INT16_MAX-INT8_MAX+1) + INT8_MAX;
   }
}

void prepare_32bit_data() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(INT16_MAX, INT32_MAX);
   for (int i=0; i<MAX_TEST_32_BIT_VARIBLE; i++){
      //get only numbers > 16 bit
      array_32_bits_numbers_to_test[i] = distrib(gen);
      //array_32_bits_numbers_to_test[i] = rand() % (INT32_MAX-INT16_MAX+1) + INT16_MAX;
   }
}

//testing help functions
TEST(TestBitOperation, bitleft) {
  uint64_t test = 1;
  bignumber big_a((uint32_t)test);
  for (int i=0; i<MAX_TEST_BIT_OPERATION; i++){
    ASSERT_EQ(test, big_a.toUINT64())  << "a = " << test << endl
                                       << "a << 1=" << test << endl << "Lib a << 1=" << big_a.toUINT64();
   big_a.bitleft();
   test = test << 1;
  } 
}

TEST(TestBitOperation, bitright) {
  uint64_t test = UINT32_MAX;
  bignumber big_a((uint32_t)test);
  for (int i=0; i<MAX_TEST_BIT_OPERATION; i++){
      ASSERT_EQ(test, big_a.toUINT64())  << "a = " << test << endl
                                       << "a >> 1=" << test << endl << "Lib a >> 1=" << big_a.toUINT64();
      big_a.bitright();
      test = test >> 1;
  } 
}

TEST(TestBitOperation, addone) {
  uint64_t test = 255;
  bignumber big_a((uint8_t)test);

   big_a.addone();
   ASSERT_EQ(big_a.toUINT64(), 0)  << "a = " << test << endl << "a" << test << endl << "Lib a.addone() = " << big_a.toUINT64();

  test = 65535;
  bignumber big_1((uint8_t)test);

   big_1.addone();
   ASSERT_EQ(big_1.toUINT64(), 0)  << "a = " << test << endl << "a" << test << endl << "Lib a.addone() = " << big_1.toUINT64();

   for (int i=0; i<8; i++){
      bignumber big_2((uint8_t)i);

      ASSERT_EQ(big_2.toUINT64(), i)  << "a = " << i << endl << "a" << i << endl << "Lib a.addone() = " << big_2.toUINT64(); 
      big_2.addone();
   }
}
//testing 8 bit operators
TEST(TestOperator8bit, operatorPlus) { 
  for (unsigned long a=1; a<UINT8_MAX; a++) 
   for (unsigned long b=1; b<UINT8_MAX; b++){   
    bignumber big_a((uint32_t)a);
    bignumber big_b((uint32_t)b);
    bignumber big_result;
    uint64_t c_result;

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
      uint64_t c_result;
      if (a>=b) {
        c_result = a - b;
        big_result = big_a - big_b;
        ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << a << endl << "b = " << b << endl
                                                    << "a-b=" << c_result << endl << "Lib a-b=" << big_result.toUINT64();
      }
   }
}

TEST(TestOperator8bit, operatorBiggerEqual) { 
  for (unsigned long a=1; a<UINT8_MAX; a++) 
   for (unsigned long b=1; b<UINT8_MAX; b++){
      bignumber big_a((uint32_t)a);
      bignumber big_b((uint32_t)b);
      bool big_result;
      bool c_result;
      
      c_result = a >= b;
      big_result = big_a >= big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << a << endl << "b = " << b << endl
                                      << "a>=b=" << c_result << endl << "Lib a>=b=" << big_result;
   }
}

TEST(TestOperator8bit, operatorLower) { 
  for (unsigned long a=1; a<UINT8_MAX; a++) 
   for (unsigned long b=1; b<UINT8_MAX; b++){
      bignumber big_a((uint32_t)a);
      bignumber big_b((uint32_t)b);
      bool big_result;
      bool c_result;
      
      c_result = a < b;
      big_result = big_a < big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << a << endl << "b = " << b << endl
                                      << "a<b=" << c_result << endl << "Lib a<b=" << big_result;
   }
}

TEST(TestOperator8bit, operatorNotEqual) { 
  for (unsigned long a=1; a<UINT8_MAX; a++) 
   for (unsigned long b=1; b<UINT8_MAX; b++){
      bignumber big_a((uint32_t)a);
      bignumber big_b((uint32_t)b);
      bool big_result;
      bool c_result;
      
      c_result = a != b;
      big_result = big_a != big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << a << endl << "b = " << b << endl
                                      << "a!=b=" << c_result << endl << "Lib a!=b=" << big_result;
   }
}

TEST(TestOperator8bit, operatorMultiplication) { 
  for (unsigned long a=1; a<UINT8_MAX; a++) 
   for (unsigned long b=1; b<UINT8_MAX; b++){
      bignumber big_a((uint32_t)a);
      bignumber big_b((uint32_t)b);
      bignumber big_result;
      uint64_t c_result;
      
      c_result = a * b;
      big_result = big_a * big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << a << endl << "b = " << b << endl
                                                 << "a*b=" << c_result << endl << "Lib a*b=" << big_result.toUINT64();
   }
}

TEST(TestOperator8bit, operatorDivide) { 
  for (unsigned long a=0; a<UINT8_MAX; a++) 
   for (unsigned long b=1; b<UINT8_MAX; b++){
      bignumber big_a((uint8_t)a);
      bignumber big_b((uint8_t)b);
      bignumber big_result;
      uint64_t c_result;
      
      c_result = a / b;
      big_result = big_a / big_b;
//     cout << "a = " << a << endl << "b = " << b << endl
  //         << "a/b=" << c_result << endl << "Lib a/b=" << big_result.toUINT64() << endl;
      ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << a << endl << "b = " << b << endl
                                                  << "a/b=" << c_result << endl << "Lib a/b=" << big_result.toUINT64() << endl;
   }
}

TEST(TestOperator8bit, operatorDivideMmodulo) { 
  for (unsigned long a=0; a<UINT8_MAX; a++) 
   for (unsigned long b=1; b<UINT8_MAX; b++){
      bignumber big_a((uint8_t)a);
      bignumber big_b((uint8_t)b);
      bignumber big_result;
      uint64_t c_result;
      
      c_result = a % b;
      big_result = big_a % big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << a << endl << "b = " << b << endl
                                                  << "a%b=" << c_result << endl << "Lib a%b=" << big_result.toUINT64();
   }
}

//testing 16 bit operators

TEST(TestOperator16bit, operatorBiggerEqual) { 
  for (unsigned long a=0; a<MAX_TEST_16_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_16_BIT_VARIBLE; b++){   
    bignumber big_a((uint32_t)array_16_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)array_16_bits_numbers_to_test[b]);
    bool big_result;
    bool c_result;
      
      c_result = array_16_bits_numbers_to_test[a] >= array_16_bits_numbers_to_test[b];
      big_result = big_a >= big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << array_16_bits_numbers_to_test[a]  << endl << "b = " << array_16_bits_numbers_to_test[b]  << endl
                                      << "a>=b=" << c_result << endl << "Lib a>=b=" << big_result;
   }
}

TEST(TestOperator16bit, operatorLower) { 
  for (unsigned long a=0; a<MAX_TEST_16_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_16_BIT_VARIBLE; b++){   
    bignumber big_a((uint32_t)array_16_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)array_16_bits_numbers_to_test[b]);
    bool big_result;
    bool c_result;
      
      c_result = array_16_bits_numbers_to_test[a] < array_16_bits_numbers_to_test[b];
      big_result = big_a < big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << array_16_bits_numbers_to_test[a]  << endl << "b = " << array_16_bits_numbers_to_test[b]  << endl
                                      << "a<b=" << c_result << endl << "Lib a<b=" << big_result;
   }
}

TEST(TestOperator16bit, operatorNotEqual) { 
  for (unsigned long a=0; a<MAX_TEST_16_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_16_BIT_VARIBLE; b++){   
    bignumber big_a((uint32_t)array_16_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)array_16_bits_numbers_to_test[b]);
    bool big_result;
    bool c_result;
      
      c_result = array_16_bits_numbers_to_test[a] != array_16_bits_numbers_to_test[b];
      big_result = big_a != big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << array_16_bits_numbers_to_test[a]  << endl << "b = " << array_16_bits_numbers_to_test[b]  << endl
                                      << "a!=b=" << c_result << endl << "Lib a!=b=" << big_result;
   }
}

TEST(TestOperator16bit, operatorBiggerEqual_static_inputl) { 
  for (unsigned long a=0; a<std::size(static_array_16_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_16_bits_numbers_to_test); b++){   
    bignumber big_a((uint32_t)static_array_16_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)static_array_16_bits_numbers_to_test[b]);
    bool big_result;
    bool c_result;
      
      c_result = static_array_16_bits_numbers_to_test[a] >= static_array_16_bits_numbers_to_test[b];
      big_result = big_a >= big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << static_array_16_bits_numbers_to_test[a] << endl << "b = " << static_array_16_bits_numbers_to_test[b] << endl
                                      << "a>=b=" << c_result << endl << "Lib a>=b=" << big_result;
   }
}

TEST(TestOperator16bit, operatorLower_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_16_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_16_bits_numbers_to_test); b++){   
    bignumber big_a((uint32_t)static_array_16_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)static_array_16_bits_numbers_to_test[b]);
    bool big_result;
    bool c_result;
      
      c_result = static_array_16_bits_numbers_to_test[a] < static_array_16_bits_numbers_to_test[b];
      big_result = big_a < big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << static_array_16_bits_numbers_to_test[a] << endl << "b = " << static_array_16_bits_numbers_to_test[b] << endl
                                      << "a<b=" << c_result << endl << "Lib a<b=" << big_result;
   }
}

TEST(TestOperator16bit, operatorNotEqual_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_16_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_16_bits_numbers_to_test); b++){   
    bignumber big_a((uint32_t)static_array_16_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)static_array_16_bits_numbers_to_test[b]);
    bool big_result;
    bool c_result;
      
      c_result = static_array_16_bits_numbers_to_test[a] != static_array_16_bits_numbers_to_test[b];
      big_result = big_a != big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << static_array_16_bits_numbers_to_test[a] << endl << "b = " << static_array_16_bits_numbers_to_test[b] << endl
                                      << "a!=b=" << c_result << endl << "Lib a!=b=" << big_result;
   }
}

TEST(TestOperator16bit, operatorPlus_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_16_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_16_bits_numbers_to_test); b++){   
    bignumber big_a((uint32_t)static_array_16_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)static_array_16_bits_numbers_to_test[b]);
    bignumber big_result;
    uint64_t c_result;

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
    uint64_t c_result;

      if (static_array_16_bits_numbers_to_test[a]>=static_array_16_bits_numbers_to_test[b]) {
         c_result = static_array_16_bits_numbers_to_test[a] - static_array_16_bits_numbers_to_test[b];
         big_result = big_a - big_b;
         ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << static_array_16_bits_numbers_to_test[a] << endl << "b = " << static_array_16_bits_numbers_to_test[b] << endl
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
      uint64_t c_result;

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
      uint64_t c_result;

      if (0 != static_array_16_bits_numbers_to_test[b]){
         c_result = static_array_16_bits_numbers_to_test[a] / static_array_16_bits_numbers_to_test[b];
         big_result = big_a / big_b;
         
         ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << static_array_16_bits_numbers_to_test[a] << endl << "b = " << static_array_16_bits_numbers_to_test[b] << endl
                                                   << "a/b=" << c_result << endl << "Lib a/b=" << big_result.toUINT64();
      }
   }
}

TEST(TestOperator16bit, operatorDivideModulo_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_16_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_16_bits_numbers_to_test); b++){   
      bignumber big_a((uint32_t)static_array_16_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)static_array_16_bits_numbers_to_test[b]);
      bignumber big_result;
      uint64_t c_result;

      if (0 != static_array_16_bits_numbers_to_test[b]){
         c_result = static_array_16_bits_numbers_to_test[a] % static_array_16_bits_numbers_to_test[b];
         big_result = big_a % big_b;
         
         ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << static_array_16_bits_numbers_to_test[a] << endl << "b = " << static_array_16_bits_numbers_to_test[b] << endl
                                                   << "a%b=" << c_result << endl << "Lib a%b=" << big_result.toUINT64();
      }
   }
}

TEST(TestOperator16bit, operatorPlus) { 
  for (unsigned long a=0; a<MAX_TEST_16_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_16_BIT_VARIBLE; b++){   
    bignumber big_a((uint32_t)array_16_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)array_16_bits_numbers_to_test[b]);
    bignumber big_result;
    uint64_t c_result;

    c_result = array_16_bits_numbers_to_test[a] + array_16_bits_numbers_to_test[b];
    big_result = big_a + big_b;

    ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                << "a+b=" << c_result << endl << "Lib a+b=" << big_result.toUINT64();
   }
}

TEST(TestOperator16bit, operatorMinus) { 
  for (unsigned long a=0; a<MAX_TEST_16_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_16_BIT_VARIBLE; b++){   
    bignumber big_a((uint32_t)array_16_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)array_16_bits_numbers_to_test[b]);
    bignumber big_result;
    uint64_t c_result;

      if (array_16_bits_numbers_to_test[a]>=array_16_bits_numbers_to_test[b]) {
         c_result = array_16_bits_numbers_to_test[a] - array_16_bits_numbers_to_test[b];
         big_result = big_a - big_b;
         ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                     << "a-b=" << c_result << endl << "Lib a-b=" << big_result.toUINT64();
      }
   }
}

TEST(TestOperator16bit, operatorMultiplication) { 
  for (unsigned long a=0; a<MAX_TEST_16_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_16_BIT_VARIBLE; b++){   
      bignumber big_a((uint32_t)array_16_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)array_16_bits_numbers_to_test[b]);
      bignumber big_result;
      uint64_t c_result;

      c_result = array_16_bits_numbers_to_test[a] * array_16_bits_numbers_to_test[b];
      big_result = big_a * big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64())  << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                  << "a*b=" << c_result << endl << "Lib a*b=" << big_result.toUINT64();
   }
}

TEST(TestOperator16bit, operatorDivide) { 
  for (unsigned long a=0; a<MAX_TEST_16_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_16_BIT_VARIBLE; b++){   
      bignumber big_a((uint32_t)array_16_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)array_16_bits_numbers_to_test[b]);
      bignumber big_result;
      uint64_t c_result;

   if (0 != array_16_bits_numbers_to_test[b]){
      //cout << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl;
      c_result = array_16_bits_numbers_to_test[a] / array_16_bits_numbers_to_test[b];
      big_result = big_a / big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                 << "a/b=" << c_result << endl << "Lib a/b=" << big_result.toUINT64();
   }
   }
  for (unsigned long a=0; a<MAX_TEST_16_BIT_VARIBLE; a++) 
   for (unsigned long b=MAX_TEST_16_BIT_VARIBLE-1; b>0; b--){   
      bignumber big_a((uint32_t)array_16_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)array_16_bits_numbers_to_test[b]);
      bignumber big_result;
      uint64_t c_result;

   if (0 != array_16_bits_numbers_to_test[b]){
      //cout << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl;
      c_result = array_16_bits_numbers_to_test[a] / array_16_bits_numbers_to_test[b];
      big_result = big_a / big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                 << "a/b=" << c_result << endl << "Lib a/b=" << big_result.toUINT64();
   }
   }
}

TEST(TestOperator16bit, operatorDivideModulo) { 
  for (unsigned long a=0; a<MAX_TEST_16_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_16_BIT_VARIBLE; b++){   
      bignumber big_a((uint32_t)array_16_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)array_16_bits_numbers_to_test[b]);
      bignumber big_result;
      uint64_t c_result;

   if (0 != array_16_bits_numbers_to_test[b]){
      c_result = array_16_bits_numbers_to_test[a] % array_16_bits_numbers_to_test[b];
      big_result = big_a % big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                 << "a%b=" << c_result << endl << "Lib a%b=" << big_result.toUINT64();
   }
   }
}
//testing 32 bit operators

TEST(TestOperator32bit, operatorBiggerEqual) { 
  for (unsigned long a=0; a<MAX_TEST_32_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_32_BIT_VARIBLE; b++){   
    bignumber big_a((uint32_t)array_32_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)array_32_bits_numbers_to_test[b]);
    bool big_result;
    bool c_result;
      
      c_result = array_32_bits_numbers_to_test[a] >= array_32_bits_numbers_to_test[b];
      big_result = big_a >= big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[a] << endl
                                      << "a>=b=" << c_result << endl << "Lib a>=b=" << big_result;
   }
}

TEST(TestOperator32bit, operatorLower) { 
  for (unsigned long a=0; a<MAX_TEST_32_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_32_BIT_VARIBLE; b++){   
    bignumber big_a((uint32_t)array_32_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)array_32_bits_numbers_to_test[b]);
    bool big_result;
    bool c_result;
      
      c_result = array_32_bits_numbers_to_test[a] < array_32_bits_numbers_to_test[b];
      big_result = big_a < big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                      << "a<b=" << c_result << endl << "Lib a<b=" << big_result;
   }
}

TEST(TestOperator32bit, operatorNotEqual) { 
  for (unsigned long a=0; a<MAX_TEST_32_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_32_BIT_VARIBLE; b++){   
    bignumber big_a((uint32_t)array_32_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)array_32_bits_numbers_to_test[b]);
    bool big_result;
    bool c_result;
      
      c_result = array_32_bits_numbers_to_test[a] != array_32_bits_numbers_to_test[b];
      big_result = big_a != big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                      << "a!=b=" << c_result << endl << "Lib a!=b=" << big_result;
   }
}

TEST(TestOperator32bit, operatorBiggerEqual_static_inputl) { 
  for (unsigned long a=0; a<std::size(static_array_32_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_32_bits_numbers_to_test); b++){   
      bignumber big_a((uint32_t)static_array_32_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)static_array_32_bits_numbers_to_test[b]);
    bool big_result;
    bool c_result;
      
      c_result = static_array_32_bits_numbers_to_test[a] >= static_array_32_bits_numbers_to_test[b];
      big_result = big_a >= big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << static_array_32_bits_numbers_to_test[a] << endl << "b = " << static_array_32_bits_numbers_to_test[b] << endl
                                      << "a>=b=" << c_result << endl << "Lib a>=b=" << big_result;
   }
}

TEST(TestOperator32bit, operatorLower_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_32_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_32_bits_numbers_to_test); b++){   
      bignumber big_a((uint32_t)static_array_32_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)static_array_32_bits_numbers_to_test[b]);
    bool big_result;
    bool c_result;
      
      c_result = static_array_32_bits_numbers_to_test[a] < static_array_32_bits_numbers_to_test[b];
      big_result = big_a < big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << static_array_32_bits_numbers_to_test[a] << endl << "b = " << static_array_32_bits_numbers_to_test[a] << endl
                                      << "a<b=" << c_result << endl << "Lib a<b=" << big_result;
   }
}

TEST(TestOperator32bit, operatorNotEqual_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_32_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_32_bits_numbers_to_test); b++){   
      bignumber big_a((uint32_t)static_array_32_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)static_array_32_bits_numbers_to_test[b]);
    bool big_result;
    bool c_result;
      
      c_result = static_array_32_bits_numbers_to_test[a] != static_array_32_bits_numbers_to_test[b];
      big_result = big_a != big_b;
      
      ASSERT_EQ(c_result, big_result) << "a = " << a << endl << "b = " << b << endl
                                      << "a!=b=" << c_result << endl << "Lib a!=b=" << big_result;
   }
}

TEST(TestOperator32bit, operatorPlus_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_32_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_32_bits_numbers_to_test); b++){   
      bignumber big_a((uint32_t)static_array_32_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)static_array_32_bits_numbers_to_test[b]);
    bignumber big_result;
    uint64_t c_result;

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
    uint64_t c_result;

      if (static_array_32_bits_numbers_to_test[a]>=static_array_32_bits_numbers_to_test[b]) {
         c_result = static_array_32_bits_numbers_to_test[a] - static_array_32_bits_numbers_to_test[b];
         big_result = big_a - big_b;
         ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << static_array_32_bits_numbers_to_test[a] << endl << "b = " << static_array_32_bits_numbers_to_test[b] << endl
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
      uint64_t c_result;

      c_result = static_array_32_bits_numbers_to_test[a] * static_array_32_bits_numbers_to_test[b];
      big_result = big_a * big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << static_array_32_bits_numbers_to_test[a] << endl << "b = " << static_array_32_bits_numbers_to_test[b] << endl
                                                 << "a*b=" << c_result << endl << "Lib a*b=" << big_result.toUINT64();
   }
}

TEST(TestOperator32bit, operatorDivide_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_32_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_32_bits_numbers_to_test); b++){   
      bignumber big_a((uint32_t)static_array_32_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)static_array_32_bits_numbers_to_test[b]);
      bignumber big_result;
      uint64_t c_result;
      if (0 != static_array_32_bits_numbers_to_test[b]){
      c_result = static_array_32_bits_numbers_to_test[a] / static_array_32_bits_numbers_to_test[b];
      big_result = big_a / big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << static_array_32_bits_numbers_to_test[a] << endl << "b = " << static_array_32_bits_numbers_to_test[b] << endl
                                                 << "a/b=" << c_result << endl << "Lib a/b=" << big_result.toUINT64();
      }
   }
}
TEST(TestOperator32bit, operatorDivideModulo_static_input) { 
  for (unsigned long a=0; a<std::size(static_array_32_bits_numbers_to_test); a++) 
   for (unsigned long b=0; b<std::size(static_array_32_bits_numbers_to_test); b++){   
      bignumber big_a((uint32_t)static_array_32_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)static_array_32_bits_numbers_to_test[b]);
      bignumber big_result;
      uint64_t c_result;
      if (0 != static_array_32_bits_numbers_to_test[b]){
      c_result = static_array_32_bits_numbers_to_test[a] % static_array_32_bits_numbers_to_test[b];
      big_result = big_a % big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << static_array_32_bits_numbers_to_test[a] << endl << "b = " << static_array_32_bits_numbers_to_test[b] << endl
                                                 << "a%b=" << c_result << endl << "Lib a%b=" << big_result.toUINT64();
      }
   }
}

TEST(TestOperator32bit, operatorPlus) { 
  for (unsigned long a=0; a<MAX_TEST_32_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_32_BIT_VARIBLE; b++){   
    bignumber big_a((uint32_t)array_32_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)array_32_bits_numbers_to_test[b]);
    bignumber big_result;
    uint64_t c_result;

    c_result = array_32_bits_numbers_to_test[a] + array_32_bits_numbers_to_test[b];
    big_result = big_a + big_b;

    ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                               << "a+b=" << c_result << endl << "Lib a+b=" << big_result.toUINT64();
   }
}

TEST(TestOperator32bit, operatorMinus) { 
  for (unsigned long a=0; a<MAX_TEST_32_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_32_BIT_VARIBLE; b++){   
    bignumber big_a((uint32_t)array_32_bits_numbers_to_test[a]);
    bignumber big_b((uint32_t)array_32_bits_numbers_to_test[b]);
    bignumber big_result;
    uint64_t c_result;

      if (array_32_bits_numbers_to_test[a]>=array_32_bits_numbers_to_test[b]) {
         c_result = array_32_bits_numbers_to_test[a] - array_32_bits_numbers_to_test[b];
         big_result = big_a - big_b;
         ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                                    << "a-b=" << c_result << endl << "Lib a-b=" << big_result.toUINT64();
      }
   }
}

TEST(TestOperator32bit, operatorMultiplication) { 
  for (unsigned long a=0; a<MAX_TEST_32_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_32_BIT_VARIBLE; b++){   
      bignumber big_a((uint32_t)array_32_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)array_32_bits_numbers_to_test[b]);
      bignumber big_result;
      uint64_t c_result;

      c_result = array_32_bits_numbers_to_test[a] * array_32_bits_numbers_to_test[b];
      big_result = big_a * big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                                 << "a*b=" << c_result << endl << "Lib a*b=" << big_result.toUINT64();
   }
}

TEST(TestOperator32bit, operatorDivide) { 
  for (unsigned long a=0; a<MAX_TEST_32_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_32_BIT_VARIBLE; b++){   
      bignumber big_a((uint32_t)array_32_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)array_32_bits_numbers_to_test[b]);
      bignumber big_result;
      uint64_t c_result;

      if (0 != array_32_bits_numbers_to_test[b]){
      //cout << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl;
      c_result = array_32_bits_numbers_to_test[a] / array_32_bits_numbers_to_test[b];
      big_result = big_a / big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                                 << "a/b=" << c_result << endl << "Lib a/b=" << big_result.toUINT64();
      }
   }
}

TEST(TestOperator32bit, operatorDivideModulo) { 
  for (unsigned long a=0; a<MAX_TEST_32_BIT_VARIBLE; a++) 
   for (unsigned long b=0; b<MAX_TEST_32_BIT_VARIBLE; b++){   
      bignumber big_a((uint32_t)array_32_bits_numbers_to_test[a]);
      bignumber big_b((uint32_t)array_32_bits_numbers_to_test[b]);
      bignumber big_result;
      uint64_t c_result;

      if (0 != array_32_bits_numbers_to_test[b]){
      c_result = array_32_bits_numbers_to_test[a] % array_32_bits_numbers_to_test[b];
      big_result = big_a % big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                                 << "a%b=" << c_result << endl << "Lib a%b=" << big_result.toUINT64();
      }
   }
}

int main(int argc, char **argv) {
   srand(time(NULL));
   prepare_16bit_data();
   prepare_32bit_data();
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
