#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <bitset>
#include <iostream>
#include <cassert>

#include "include/digit.h"
#include "include/bignumber.h"

using namespace std;

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

    ASSERT_EQ(c_result, big_result.toUINT64());
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
        ASSERT_EQ(c_result, big_result.toUINT64());
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
      
      ASSERT_EQ(c_result, big_result.toUINT64());
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
      
      ASSERT_EQ(c_result, big_result.toUINT64());
   }
}

//testing 16 bit operators
TEST(TestOperator16bit, operatorPlus) { 
  for (unsigned long a=1; a<UINT16_MAX; a++) 
   for (unsigned long b=1; b<UINT16_MAX; b++){   
    bignumber big_a((uint32_t)a);
    bignumber big_b((uint32_t)b);
    bignumber big_result;
    unsigned long c_result;

    c_result = a + b;
    big_result = big_a + big_b;

    ASSERT_EQ(c_result, big_result.toUINT64());
}

TEST(TestOperator16bit, operatorMinus) { 
  for (unsigned long a=1; a<UINT16_MAX; a++) 
   for (unsigned long b=1; b<UINT16_MAX; b++){
      bignumber big_a((uint32_t)a);
      bignumber big_b((uint32_t)b);
      bignumber big_result;
      unsigned long c_result;
      if (a>=b) {
        c_result = a - b;
        big_result = big_a - big_b;
        ASSERT_EQ(c_result, big_result.toUINT64());
      }
   }
}

TEST(TestOperator16bit, operatorMultiplication) { 
  for (unsigned long a=1; a<UINT16_MAX; a++) 
   for (unsigned long b=1; b<UINT16_MAX; b++){
      bignumber big_a((uint32_t)a);
      bignumber big_b((uint32_t)b);
      bignumber big_result;
      unsigned long c_result;
      
      c_result = a * b;
      big_result = big_a * big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64());
   }
}

TEST(TestOperator16bit, operatorDivide) { 
  for (unsigned long a=1; a<UINT16_MAX; a++) 
   for (unsigned long b=1; b<UINT16_MAX; b++){
      bignumber big_a((uint32_t)a);
      bignumber big_b((uint32_t)b);
      bignumber big_result;
      unsigned long c_result;
      
      c_result = a / b;
      big_result = big_a / big_b;
      
      ASSERT_EQ(c_result, big_result.toUINT64());
   }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
