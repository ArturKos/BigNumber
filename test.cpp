#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <bitset>
#include <iostream>

#include "include/digit.h"
#include "include/bignumber.h"

using namespace std;

int main() {

  cout << "Testing == operator. Range 0-254" << endl;
  for(int i = 0; i<255; i++)
  {
    bignumber a1((uint8_t)i);
    bignumber b1((uint8_t)i);
    if(a1!=b1)
     cout << "Testing: "<< i << " fail" << endl;
  }
  cout << "Testing / operator, dividend = divider. Range 0-254" << endl;

  // dividend = divider
  for(int i = 0; i<255; i++)
  {
    bignumber a1((uint8_t)i);
    bignumber b1((uint8_t)i);
    bignumber one((uint8_t)1);
    bignumber division = a1/b1;
    if(division!=one)
     cout << "Testing: "<< i << " fail" << endl;
  }

  return 0;
}
