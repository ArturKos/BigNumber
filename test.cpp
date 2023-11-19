#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <bitset>
#include <iostream>
#include <cassert>

#include "include/digit.h"
#include "include/bignumber.h"

using namespace std;

  unsigned long fail_tests = 0;
  unsigned long pass_tests = 0;
  unsigned long plus_operator_pass = 0;
  unsigned long plus_operator_fail = 0;
  unsigned long minus_operator_pass = 0;
  unsigned long minus_operator_fail = 0;
  unsigned long multiplication_operator_pass = 0;
  unsigned long multiplication_operator_fail = 0;
  unsigned long division_operator_pass = 0;
  unsigned long division_operator_fail = 0;
  unsigned long modulo_operator_pass = 0;
  unsigned long modulo_operator_fail = 0;
  unsigned long assign_uint8_pass = 0;
  unsigned long assign_uint8_fail = 0;
  unsigned long assign_uint16_pass = 0;
  unsigned long assign_uint16_fail = 0;
  unsigned long assign_uint32_pass = 0;
  unsigned long assign_uint32_fail = 0;

void test_operators_uint8t()
{
    cout << "Testing uint8_t operators <0-" << UINT8_MAX << ">" << endl; 
  for (unsigned long a=1; a<UINT8_MAX; a++) 
   for (unsigned long b=1; b<UINT8_MAX; b++){   
    bignumber big_a((uint32_t)a);
    bignumber big_b((uint32_t)b);
    bignumber big_result;
    unsigned long c_result;

    c_result = a + b;
    big_result = big_a + big_b;
    if(c_result != big_result.toUINT64()) {
      cout << "Test faild, operator+" << endl;      
      cout << "a=" << a << "b=" << b << " a+b=" << c_result << endl;
      cout << "big_a="; big_a.printHEX(); cout << " big_b="; big_b.printHEX(); cout << " big_a+big_b="; big_result.printHEX(); cout << endl;  
      fail_tests++;
      plus_operator_fail++;
    } else { pass_tests++; plus_operator_pass++; }
    
    if (a>=b) {
        c_result = a - b;
        big_result = big_a - big_b;
        if(c_result != big_result.toUINT64()) {
          cout << "Test faild, operator-" << endl;      
          cout << "a=" << a << "b=" << b << " a-b=" << c_result << endl;
          cout << "big_a="; big_a.printHEX(); cout << " big_b="; big_b.printHEX(); cout << " big_a-big_b="; big_result.printHEX(); cout << endl;  
          fail_tests++;
          minus_operator_fail++;
        } else { pass_tests++; minus_operator_pass++; }
    }
    c_result = a * b;
    big_result = big_a * big_b;
    if(c_result != big_result.toUINT64()) {
      cout << "Test faild, operator*" << endl;      
      cout << "a=" << a << "b=" << b << " a*b=" << c_result << endl;
      cout << "big_a="; big_a.printHEX(); cout << " big_b="; big_b.printHEX(); cout << " big_a*big_b="; big_result.printHEX(); cout << endl;  
      fail_tests++; 
      multiplication_operator_fail++;   
    } else { pass_tests++; multiplication_operator_pass++; }

    c_result = a / b;
    big_result = big_a / big_b;
    if(c_result != big_result.toUINT64()) {
      cout << "Test faild, operator/" << endl;      
      cout << "a=" << a << "b=" << b << " a/b=" << c_result << endl;
      cout << "big_a="; big_a.printHEX(); cout << " big_b="; big_b.printHEX(); cout << " big_a/big_b="; big_result.printHEX(); cout << endl;  
      fail_tests++;
      division_operator_fail++;
    } else { pass_tests++; division_operator_pass++; }  

    c_result = a % b;
    big_result = big_a % big_b;
    if(c_result != big_result.toUINT64()) {
      cout << "Test faild, operator%" << endl;      
      cout << "a=" << a << "b=" << b << " a%b=" << c_result << endl;
      cout << "big_a="; big_a.printHEX(); cout << " big_b="; big_b.printHEX(); cout << " big_a%big_b="; big_result.printHEX(); cout << endl;  
      fail_tests++;
      modulo_operator_fail++;
    } else { pass_tests++; modulo_operator_pass++; }  
    //cout << "a=" << a << "b=" << b << endl;
  }
}
void test_operators_uint16t()
{
    cout << "Testing uint16_t operators <0-" << UINT16_MAX << ">" << endl; 
  for (unsigned int a=1; a<UINT16_MAX; a++) 
   for (unsigned int b=1; b<UINT16_MAX; b++){   
    cout << "a=" << a << "b=" << b << endl;
    bignumber big_a((uint16_t)a);
    bignumber big_b((uint16_t)b);
    bignumber big_result;
    long long c_result;

    c_result = a + b;
    big_result = big_a + big_b;
    if(c_result != big_result.toUINT64()) {
      cout << "Test faild, operator+" << endl;      
      cout << "a=" << a << "b=" << b << " a+b=" << c_result << endl;
      cout << "big_a="; big_a.printHEX(); cout << " big_b="; big_b.printHEX(); cout << " big_a+big_b="; big_result.printHEX(); cout << endl;  
      fail_tests++;
      plus_operator_fail++;
    } else { pass_tests++; plus_operator_pass++; }
    
    if (a>=b) {
        c_result = a - b;
        big_result = big_a - big_b;
        if(c_result != big_result.toUINT64()) {
          cout << "Test faild, operator-" << endl;      
          cout << "a=" << a << "b=" << b << " a-b=" << c_result << endl;
          cout << "big_a="; big_a.printHEX(); cout << " big_b="; big_b.printHEX(); cout << " big_a-big_b="; big_result.printHEX(); cout << endl;  
          fail_tests++;
          minus_operator_fail++;
        } else { pass_tests++; minus_operator_pass++; }
    }
    c_result = a * b;
    big_result = big_a * big_b;
    if(c_result != big_result.toUINT64()) {
      cout << "Test faild, operator*" << endl;      
      cout << "a=" << a << "b=" << b << " a*b=" << c_result << endl;
      cout << "big_a="; big_a.printHEX(); cout << " big_b="; big_b.printHEX(); cout << " big_a*big_b="; big_result.printHEX(); cout << endl;  
      fail_tests++; 
      multiplication_operator_fail++;   
    } else { pass_tests++; multiplication_operator_pass++; }

  //   c_result = a / b;
  //   big_result = big_a / big_b;
  //   if(c_result != big_result.toUINT64()) {
  //     cout << "Test faild, operator/" << endl;      
  //     cout << "a=" << a << "b=" << b << " a/b=" << c_result << endl;
  //     cout << "big_a="; big_a.printHEX(); cout << " big_b="; big_b.printHEX(); cout << " big_a/big_b="; big_result.printHEX(); cout << endl;  
  //     fail_tests++;
  //     division_operator_fail++;
  //   } else { pass_tests++; division_operator_pass++; }  

  //   c_result = a % b;
  //   big_result = big_a % big_b;
  //   if(c_result != big_result.toUINT64()) {
  //     cout << "Test faild, operator%" << endl;      
  //     cout << "a=" << a << "b=" << b << " a%b=" << c_result << endl;
  //     cout << "big_a="; big_a.printHEX(); cout << " big_b="; big_b.printHEX(); cout << " big_a%big_b="; big_result.printHEX(); cout << endl;  
  //     fail_tests++;
  //     modulo_operator_fail++;
  //   } else { pass_tests++; modulo_operator_pass++; }  
  //   //cout << "a=" << a << "b=" << b << endl;
 }
}
void test_assinging_uint8_uint_16_uint32()
{
    cout << "Testing assignning uint8_t"<< endl;
  for (unsigned long a=0; a<UINT8_MAX; a++) 
    try
    {
      bignumber assign_uint8_t_test((uint8_t) a);
      pass_tests++;
      assign_uint8_pass++;
    }
    catch (...)
    {
      cout << "Fail to assign "<< a << " to bignumber" << endl;
      assign_uint8_fail++;
      fail_tests++;
    }

  cout << "Testing assignning uint16_t"<< endl;
  for (unsigned long a=0; a<UINT16_MAX; a++) 
    try
    {
      bignumber assign_uint16_t_test((uint16_t) a);
      pass_tests++;
      assign_uint16_pass++;
    }
    catch (...)
    {
      cout << "Fail to assign "<< a << " to bignumber" << endl;
      assign_uint16_fail++;
      fail_tests++;
    }

  cout << "Testing assignning uint32_t"<< endl;
  for (unsigned long a=0; a<UINT32_MAX; a++) // TOCHANGE
    try
    {
      bignumber assign_uint32_t_test((uint32_t) a);
      pass_tests++;
      assign_uint32_pass++;
    }
    catch (...)
    {
      cout << "Fail to assign "<< a << " to bignumber" << endl;
      assign_uint32_fail++;
      fail_tests++;
    }
}
void print_statistics()
{
  cout << "Assignning uint8_t pass/fail " << assign_uint8_pass << "/" << assign_uint8_fail << endl; 
  cout << "Assignning uint16_t pass/fail " << assign_uint16_pass << "/" << assign_uint16_fail << endl; 
  cout << "Assignning uint32_t pass/fail " << assign_uint32_pass << "/" << assign_uint32_fail << endl;
  cout << "operator+ pass/fail " << plus_operator_pass << "/" << plus_operator_fail << endl; 
  cout << "operator- pass/fail " << minus_operator_pass << "/" << minus_operator_fail << endl; 
  cout << "operator* pass/fail " << multiplication_operator_pass << "/" << multiplication_operator_fail << endl; 
  cout << "operator/ pass/fail " << division_operator_pass << "/" << division_operator_fail << endl; 
  cout << "operator% pass/fail " << modulo_operator_pass << "/" << modulo_operator_fail << endl; 
  cout << "Total pass test:" << pass_tests << endl;
  cout << "Total fail test:" << fail_tests << endl;
}
int main() {

  //test_assinging_uint8_uint_16_uint32();
  //test_operators_uint8t();
  test_operators_uint16t();
  print_statistics();

  return 0;
}
