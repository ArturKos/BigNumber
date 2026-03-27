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
#include <cmath>

#include "include/digit.h"
#include "include/bignumber.h"

using namespace std;

//how many numbers to check since checking all 32 bit and 16 bit is would be time consuming
#define MAX_TEST_16_BIT_VARIBLE 1024
#define MAX_TEST_32_BIT_VARIBLE 1024
#define MAX_TEST_BIT_OPERATION 63

uint64_t array_32_bits_numbers_to_test[MAX_TEST_32_BIT_VARIBLE] = {0};
uint64_t array_16_bits_numbers_to_test[MAX_TEST_16_BIT_VARIBLE] = {0};

std::vector<int> static_array_16_bits_numbers_to_test = {
    0, 1, 2, 3,               // small values and division edge cases
    127, 128,                  // 0x7F / 0x80 — byte MSB boundary
    255, 256, 257,             // 0xFF / 0x100 / 0x101 — 1-byte to 2-byte boundary
    512, 1024,                 // powers of 2
    32767                      // 0x7FFF — INT16_MAX
};
std::vector<int64_t> static_array_32_bits_numbers_to_test = {
    0, 1, 2, 3,               // small values
    255, 256,                  // 0xFF / 0x100 — 1-byte to 2-byte boundary
    65535, 65536,              // 0xFFFF / 0x10000 — 2-byte to 3-byte boundary
    16777215, 16777216,        // 0xFFFFFF / 0x1000000 — 3-byte to 4-byte boundary
    2147483647                 // 0x7FFFFFFF — INT32_MAX
};

void prepare_16bit_data() {
    // Edge cases that must always be tested
    uint64_t edge_cases_16[] = {
        0, 1, 2, 3,           // small values
        127, 128,              // 0x7F / 0x80 — byte MSB boundary
        255, 256,              // 0xFF / 0x100 — 1-byte to 2-byte boundary
        32767                  // 0x7FFF — INT16_MAX
    };
    int edge_count = sizeof(edge_cases_16) / sizeof(edge_cases_16[0]);
    int i = 0;
    for (; i < edge_count && i < MAX_TEST_16_BIT_VARIBLE; i++) {
        array_16_bits_numbers_to_test[i] = edge_cases_16[i];
    }
    // Fill the rest with random values across the full 16-bit range
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, INT16_MAX);
    for (; i < MAX_TEST_16_BIT_VARIBLE; i++) {
        array_16_bits_numbers_to_test[i] = distrib(gen);
    }
}

void prepare_32bit_data() {
    // Edge cases that must always be tested
    uint64_t edge_cases_32[] = {
        0, 1, 2, 3,           // small values
        255, 256,              // 0xFF / 0x100 — 1-byte to 2-byte boundary
        65535, 65536,          // 0xFFFF / 0x10000 — 2-byte to 3-byte boundary
        16777215, 16777216,    // 0xFFFFFF / 0x1000000 — 3-byte to 4-byte boundary
        2147483647             // 0x7FFFFFFF — INT32_MAX
    };
    int edge_count = sizeof(edge_cases_32) / sizeof(edge_cases_32[0]);
    int i = 0;
    for (; i < edge_count && i < MAX_TEST_32_BIT_VARIBLE; i++) {
        array_32_bits_numbers_to_test[i] = edge_cases_32[i];
    }
    // Fill the rest with random values up to floor(sqrt(INT64_MAX))
    // so that a*b fits in to_uint64() which internally uses strtol (max INT64_MAX)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> distrib(0, 3037000499U);
    for (; i < MAX_TEST_32_BIT_VARIBLE; i++) {
        array_32_bits_numbers_to_test[i] = distrib(gen);
    }
}

//testing help functions
TEST(TestBitOperation, bitleft) {
  uint64_t test = 1;
  bignumber big_a((uint32_t)test);
  for (int i=0; i<MAX_TEST_BIT_OPERATION; i++){
    ASSERT_EQ(test, big_a.to_uint64())  << "a = " << test << endl
                                       << "a << 1=" << test << endl << "Lib a << 1=" << big_a.to_uint64();
   big_a.shift_left();
   test = test << 1;
  } 
}

TEST(TestBitOperation, bitright) {
  uint64_t test = UINT32_MAX;
  bignumber big_a((uint32_t)test);
  for (int i=0; i<MAX_TEST_BIT_OPERATION; i++){
      ASSERT_EQ(test, big_a.to_uint64())  << "a = " << test << endl
                                       << "a >> 1=" << test << endl << "Lib a >> 1=" << big_a.to_uint64();
      big_a.shift_right();
      test = test >> 1;
  } 
}

TEST(TestBitOperation, addone) {
  uint64_t test = 255;
  bignumber big_a((uint8_t)test);

   big_a.add_one();
   ASSERT_EQ(big_a.to_uint64(), 0)  << "a = " << test << endl << "a" << test << endl << "Lib a.add_one() = " << big_a.to_uint64();

  test = 65535;
  bignumber big_1((uint8_t)test);

   big_1.add_one();
   ASSERT_EQ(big_1.to_uint64(), 0)  << "a = " << test << endl << "a" << test << endl << "Lib a.add_one() = " << big_1.to_uint64();

   for (int i=0; i<8; i++){
      bignumber big_2((uint8_t)i);

      ASSERT_EQ(big_2.to_uint64(), i)  << "a = " << i << endl << "a" << i << endl << "Lib a.add_one() = " << big_2.to_uint64(); 
      big_2.add_one();
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

    ASSERT_EQ(c_result, big_result.to_uint64())  << "a = " << a << endl << "b = " << b << endl
                                                << "a+b=" << c_result << endl << "Lib a+b=" << big_result.to_uint64();
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
        ASSERT_EQ(c_result, big_result.to_uint64())  << "a = " << a << endl << "b = " << b << endl
                                                    << "a-b=" << c_result << endl << "Lib a-b=" << big_result.to_uint64();
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
      
      ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << a << endl << "b = " << b << endl
                                                 << "a*b=" << c_result << endl << "Lib a*b=" << big_result.to_uint64();
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
  //         << "a/b=" << c_result << endl << "Lib a/b=" << big_result.to_uint64() << endl;
      ASSERT_EQ(c_result, big_result.to_uint64())  << "a = " << a << endl << "b = " << b << endl
                                                  << "a/b=" << c_result << endl << "Lib a/b=" << big_result.to_uint64() << endl;
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
      
      ASSERT_EQ(c_result, big_result.to_uint64())  << "a = " << a << endl << "b = " << b << endl
                                                  << "a%b=" << c_result << endl << "Lib a%b=" << big_result.to_uint64();
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

    ASSERT_EQ(c_result, big_result.to_uint64())  << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                << "a+b=" << c_result << endl << "Lib a+b=" << big_result.to_uint64();
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
         ASSERT_EQ(c_result, big_result.to_uint64())  << "a = " << static_array_16_bits_numbers_to_test[a] << endl << "b = " << static_array_16_bits_numbers_to_test[b] << endl
                                                     << "a-b=" << c_result << endl << "Lib a-b=" << big_result.to_uint64();
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
      
      ASSERT_EQ(c_result, big_result.to_uint64())  << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                  << "a*b=" << c_result << endl << "Lib a*b=" << big_result.to_uint64();
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
         
         ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << static_array_16_bits_numbers_to_test[a] << endl << "b = " << static_array_16_bits_numbers_to_test[b] << endl
                                                   << "a/b=" << c_result << endl << "Lib a/b=" << big_result.to_uint64();
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
         
         ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << static_array_16_bits_numbers_to_test[a] << endl << "b = " << static_array_16_bits_numbers_to_test[b] << endl
                                                   << "a%b=" << c_result << endl << "Lib a%b=" << big_result.to_uint64();
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

    ASSERT_EQ(c_result, big_result.to_uint64())  << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                << "a+b=" << c_result << endl << "Lib a+b=" << big_result.to_uint64();
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
         ASSERT_EQ(c_result, big_result.to_uint64())  << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                     << "a-b=" << c_result << endl << "Lib a-b=" << big_result.to_uint64();
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
      
      ASSERT_EQ(c_result, big_result.to_uint64())  << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                  << "a*b=" << c_result << endl << "Lib a*b=" << big_result.to_uint64();
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
      
      ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                 << "a/b=" << c_result << endl << "Lib a/b=" << big_result.to_uint64();
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
      
      ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                 << "a/b=" << c_result << endl << "Lib a/b=" << big_result.to_uint64();
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
      
      ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << array_16_bits_numbers_to_test[a] << endl << "b = " << array_16_bits_numbers_to_test[b] << endl
                                                 << "a%b=" << c_result << endl << "Lib a%b=" << big_result.to_uint64();
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

    ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                               << "a+b=" << c_result << endl << "Lib a+b=" << big_result.to_uint64();
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
         ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << static_array_32_bits_numbers_to_test[a] << endl << "b = " << static_array_32_bits_numbers_to_test[b] << endl
                                                    << "a-b=" << c_result << endl << "Lib a-b=" << big_result.to_uint64();
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
      
      ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << static_array_32_bits_numbers_to_test[a] << endl << "b = " << static_array_32_bits_numbers_to_test[b] << endl
                                                 << "a*b=" << c_result << endl << "Lib a*b=" << big_result.to_uint64();
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
      
      ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << static_array_32_bits_numbers_to_test[a] << endl << "b = " << static_array_32_bits_numbers_to_test[b] << endl
                                                 << "a/b=" << c_result << endl << "Lib a/b=" << big_result.to_uint64();
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
      
      ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << static_array_32_bits_numbers_to_test[a] << endl << "b = " << static_array_32_bits_numbers_to_test[b] << endl
                                                 << "a%b=" << c_result << endl << "Lib a%b=" << big_result.to_uint64();
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

    ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                               << "a+b=" << c_result << endl << "Lib a+b=" << big_result.to_uint64();
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
         ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                                    << "a-b=" << c_result << endl << "Lib a-b=" << big_result.to_uint64();
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
      
      ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                                 << "a*b=" << c_result << endl << "Lib a*b=" << big_result.to_uint64();
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
      
      ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                                 << "a/b=" << c_result << endl << "Lib a/b=" << big_result.to_uint64();
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
      
      ASSERT_EQ(c_result, big_result.to_uint64()) << "a = " << array_32_bits_numbers_to_test[a] << endl << "b = " << array_32_bits_numbers_to_test[b] << endl
                                                 << "a%b=" << c_result << endl << "Lib a%b=" << big_result.to_uint64();
      }
   }
}

// Helper to check primality with standard C++ for test verification
static bool is_prime_ref(uint32_t n) {
  if (n < 2) return false;
  if (n < 4) return true;
  if (n % 2 == 0 || n % 3 == 0) return false;
  for (uint32_t i = 5; i * i <= n; i += 6) {
    if (n % i == 0 || n % (i + 2) == 0) return false;
  }
  return true;
}

TEST(TestPrime, knownPrimes) {
  uint8_t primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43,
                      47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
  for (uint8_t p : primes) {
    bignumber b(p);
    EXPECT_TRUE(b.is_prime()) << (int)p << " should be prime";
  }
}

TEST(TestPrime, knownComposites) {
  uint8_t composites[] = {0, 1, 4, 6, 8, 9, 10, 12, 14, 15, 16, 18, 20,
                          21, 24, 25, 26, 27, 28, 30, 100, 121, 169};
  for (uint8_t c : composites) {
    bignumber b(c);
    EXPECT_FALSE(b.is_prime()) << (int)c << " should not be prime";
  }
}

TEST(TestPrime, largerPrimes) {
  uint16_t large_primes[] = {101, 127, 251, 509, 1021, 7919, 15683, 49999};
  for (uint16_t p : large_primes) {
    bignumber b(p);
    EXPECT_TRUE(b.is_prime()) << p << " should be prime";
  }
}

TEST(TestPrime, sweepSmallRange) {
  for (uint32_t n = 0; n < 256; n++) {
    bignumber b((uint8_t)n);
    EXPECT_EQ(b.is_prime(), is_prime_ref(n)) << "mismatch at n=" << n;
  }
}

TEST(TestOperator8bit, operatorSqrt) {
  for (unsigned long a = 0; a < UINT8_MAX; a++) {
    bignumber big_a((uint8_t)a);
    bignumber big_result = big_a.isqrt(NULL);
    uint64_t c_result = (uint64_t)sqrt((double)a);
    ASSERT_EQ(c_result, big_result.to_uint64()) << "sqrt(" << a << ") should be " << c_result;
  }
}

TEST(TestOperator16bit, operatorSqrt_static_input) {
  for (unsigned long a = 0; a < std::size(static_array_16_bits_numbers_to_test); a++) {
    bignumber big_a((uint32_t)static_array_16_bits_numbers_to_test[a]);
    bignumber big_result = big_a.isqrt(NULL);
    uint64_t c_result = (uint64_t)sqrt((double)static_array_16_bits_numbers_to_test[a]);
    ASSERT_EQ(c_result, big_result.to_uint64()) << "sqrt(" << static_array_16_bits_numbers_to_test[a] << ") should be " << c_result;
  }
}

TEST(TestOperator16bit, operatorSqrt) {
  for (unsigned long a = 0; a < MAX_TEST_16_BIT_VARIBLE; a++) {
    bignumber big_a((uint32_t)array_16_bits_numbers_to_test[a]);
    bignumber big_result = big_a.isqrt(NULL);
    uint64_t c_result = (uint64_t)sqrt((double)array_16_bits_numbers_to_test[a]);
    ASSERT_EQ(c_result, big_result.to_uint64()) << "sqrt(" << array_16_bits_numbers_to_test[a] << ") should be " << c_result;
  }
}

TEST(TestOperator32bit, operatorSqrt_static_input) {
  for (unsigned long a = 0; a < std::size(static_array_32_bits_numbers_to_test); a++) {
    bignumber big_a((uint32_t)static_array_32_bits_numbers_to_test[a]);
    bignumber big_result = big_a.isqrt(NULL);
    uint64_t c_result = (uint64_t)sqrt((double)static_array_32_bits_numbers_to_test[a]);
    ASSERT_EQ(c_result, big_result.to_uint64()) << "sqrt(" << static_array_32_bits_numbers_to_test[a] << ") should be " << c_result;
  }
}

TEST(TestOperator32bit, operatorSqrt) {
  for (unsigned long a = 0; a < MAX_TEST_32_BIT_VARIBLE; a++) {
    bignumber big_a((uint32_t)array_32_bits_numbers_to_test[a]);
    bignumber big_result = big_a.isqrt(NULL);
    uint64_t r = big_result.to_uint64();
    uint64_t v = array_32_bits_numbers_to_test[a];
    // For large values, sqrt(double) can lose precision, so verify r*r <= v < (r+1)*(r+1)
    ASSERT_TRUE(r * r <= v) << "sqrt(" << v << ")=" << r << " but " << r << "^2=" << r*r << " > " << v;
    ASSERT_TRUE((r + 1) * (r + 1) > v) << "sqrt(" << v << ")=" << r << " but " << (r+1) << "^2=" << (r+1)*(r+1) << " <= " << v;
  }
}

// --- Helper: reference binary string without leading zeros ---
static std::string ref_bin(uint64_t v) {
  if (v == 0) return "0";
  std::string s;
  while (v > 0) { s += ('0' + (v & 1)); v >>= 1; }
  std::reverse(s.begin(), s.end());
  return s;
}

// --- Helper: reference hex string with 0x prefix ---
static std::string ref_hex(uint64_t v) {
  if (v == 0) return "0x0";
  char buf[32];
  std::snprintf(buf, sizeof(buf), "0x%lX", (unsigned long)v);
  return buf;
}

// ============ 8-bit toString tests ============

TEST(TestOperator8bit, toStringBIN) {
  for (unsigned long a = 0; a < UINT8_MAX; a++) {
    bignumber big_a((uint8_t)a);
    ASSERT_EQ(ref_bin(a), big_a.to_string_bin()) << "toStringBIN(" << a << ")";
  }
}

TEST(TestOperator8bit, toStringDEC) {
  for (unsigned long a = 0; a < UINT8_MAX; a++) {
    bignumber big_a((uint8_t)a);
    ASSERT_EQ(std::to_string(a), big_a.to_string_dec()) << "toStringDEC(" << a << ")";
  }
}

TEST(TestOperator8bit, toStringHEX) {
  for (unsigned long a = 0; a < UINT8_MAX; a++) {
    bignumber big_a((uint8_t)a);
    ASSERT_EQ(ref_hex(a), big_a.to_string_hex()) << "toStringHEX(" << a << ")";
  }
}

// ============ 16-bit toString tests (static + random) ============

TEST(TestOperator16bit, toStringBIN_static_input) {
  for (unsigned long a = 0; a < std::size(static_array_16_bits_numbers_to_test); a++) {
    uint32_t v = static_array_16_bits_numbers_to_test[a];
    bignumber big_a(v);
    ASSERT_EQ(ref_bin(v), big_a.to_string_bin()) << "toStringBIN(" << v << ")";
  }
}

TEST(TestOperator16bit, toStringDEC_static_input) {
  for (unsigned long a = 0; a < std::size(static_array_16_bits_numbers_to_test); a++) {
    uint32_t v = static_array_16_bits_numbers_to_test[a];
    bignumber big_a(v);
    ASSERT_EQ(std::to_string(v), big_a.to_string_dec()) << "toStringDEC(" << v << ")";
  }
}

TEST(TestOperator16bit, toStringHEX_static_input) {
  for (unsigned long a = 0; a < std::size(static_array_16_bits_numbers_to_test); a++) {
    uint32_t v = static_array_16_bits_numbers_to_test[a];
    bignumber big_a(v);
    ASSERT_EQ(ref_hex(v), big_a.to_string_hex()) << "toStringHEX(" << v << ")";
  }
}

TEST(TestOperator16bit, toStringBIN) {
  for (unsigned long a = 0; a < MAX_TEST_16_BIT_VARIBLE; a++) {
    uint64_t v = array_16_bits_numbers_to_test[a];
    bignumber big_a((uint32_t)v);
    ASSERT_EQ(ref_bin(v), big_a.to_string_bin()) << "toStringBIN(" << v << ")";
  }
}

TEST(TestOperator16bit, toStringDEC) {
  for (unsigned long a = 0; a < MAX_TEST_16_BIT_VARIBLE; a++) {
    uint64_t v = array_16_bits_numbers_to_test[a];
    bignumber big_a((uint32_t)v);
    ASSERT_EQ(std::to_string(v), big_a.to_string_dec()) << "toStringDEC(" << v << ")";
  }
}

TEST(TestOperator16bit, toStringHEX) {
  for (unsigned long a = 0; a < MAX_TEST_16_BIT_VARIBLE; a++) {
    uint64_t v = array_16_bits_numbers_to_test[a];
    bignumber big_a((uint32_t)v);
    ASSERT_EQ(ref_hex(v), big_a.to_string_hex()) << "toStringHEX(" << v << ")";
  }
}

// ============ 32-bit toString tests (static + random) ============

TEST(TestOperator32bit, toStringBIN_static_input) {
  for (unsigned long a = 0; a < std::size(static_array_32_bits_numbers_to_test); a++) {
    uint64_t v = static_array_32_bits_numbers_to_test[a];
    bignumber big_a((uint32_t)v);
    ASSERT_EQ(ref_bin(v), big_a.to_string_bin()) << "toStringBIN(" << v << ")";
  }
}

TEST(TestOperator32bit, toStringDEC_static_input) {
  for (unsigned long a = 0; a < std::size(static_array_32_bits_numbers_to_test); a++) {
    uint64_t v = static_array_32_bits_numbers_to_test[a];
    bignumber big_a((uint32_t)v);
    ASSERT_EQ(std::to_string(v), big_a.to_string_dec()) << "toStringDEC(" << v << ")";
  }
}

TEST(TestOperator32bit, toStringHEX_static_input) {
  for (unsigned long a = 0; a < std::size(static_array_32_bits_numbers_to_test); a++) {
    uint64_t v = static_array_32_bits_numbers_to_test[a];
    bignumber big_a((uint32_t)v);
    ASSERT_EQ(ref_hex(v), big_a.to_string_hex()) << "toStringHEX(" << v << ")";
  }
}

TEST(TestOperator32bit, toStringBIN) {
  for (unsigned long a = 0; a < MAX_TEST_32_BIT_VARIBLE; a++) {
    uint64_t v = array_32_bits_numbers_to_test[a];
    bignumber big_a((uint32_t)v);
    ASSERT_EQ(ref_bin(v), big_a.to_string_bin()) << "toStringBIN(" << v << ")";
  }
}

TEST(TestOperator32bit, toStringDEC) {
  for (unsigned long a = 0; a < MAX_TEST_32_BIT_VARIBLE; a++) {
    uint64_t v = array_32_bits_numbers_to_test[a];
    bignumber big_a((uint32_t)v);
    ASSERT_EQ(std::to_string(v), big_a.to_string_dec()) << "toStringDEC(" << v << ")";
  }
}

TEST(TestOperator32bit, toStringHEX) {
  for (unsigned long a = 0; a < MAX_TEST_32_BIT_VARIBLE; a++) {
    uint64_t v = array_32_bits_numbers_to_test[a];
    bignumber big_a((uint32_t)v);
    ASSERT_EQ(ref_hex(v), big_a.to_string_hex()) << "toStringHEX(" << v << ")";
  }
}

int main(int argc, char **argv) {
   srand(time(NULL));
   prepare_16bit_data();
   prepare_32bit_data();
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
