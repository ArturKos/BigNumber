#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <bitset>
#include <iostream>

#include "include/digit.cpp"
#include "include/bignumber.cpp"

using namespace std;

int main() {
  bignumber a1((uint8_t)214);
  bignumber b1((uint8_t)10);
  bignumber sum;

  sum = a1 / b1;
  sum.printDEC();
  return 0;
}
