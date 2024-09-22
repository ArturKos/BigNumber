#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdint>

#include <bitset>
#include <iostream>

#include "include/digit.h"
#include "include/bignumber.h"

using namespace std;

int main() {

  bignumber result;
  int a, b=1;

while (b!=0){
  cout << "Provide a=";
  cin >> a;
  cout << "Provide b=";
  cin >> b;
  if (0 == b) break;
  bignumber a1((uint8_t)a);
  bignumber b1((uint8_t)b);

    cout << "Adding" << endl;
    result = a1 + b1;
    cout << a << "+" << b << endl;
    cout << "Result in binary:"; result.printBIN(); cout << endl;
    cout << "Result in hexadecimal:"; result.printHEX(); cout << endl;
    cout << "Result in decimal:"; result.printDEC(); cout << endl;

    cout << "Subtracting" << endl;
    result = a1 - b1;
    cout << a << "-" << b << endl;
    cout << "Result in binary:"; result.printBIN(); cout << endl;
    cout << "Result in hexadecimal:"; result.printHEX(); cout << endl;
    cout << "Result in decimal:"; result.printDEC(); cout << endl;

    cout << "Multiplying" << endl;
    result = a1 * b1;
    cout << a << "*" << b << endl;
    cout << "Result in binary:"; result.printBIN(); cout << endl;
    cout << "Result in hexadecimal:"; result.printHEX(); cout << endl;
    cout << "Result in decimal:"; result.printDEC(); cout << endl;

    cout << "Dividing" << endl;
    result = a1 / b1;
    cout << a << "/" << b << endl;
    cout << "Result in binary:"; result.printBIN(); cout << endl;
    cout << "Result in hexadecimal:"; result.printHEX(); cout << endl;
    cout << "Result in decimal:"; result.printDEC(); cout << endl;

    cout << "Modulo division" << endl;
    result = a1 % b1;
    cout << a << "%" << b << endl;
    cout << "Result in binary:"; result.printBIN(); cout << endl;
    cout << "Result in hexadecimal:"; result.printHEX(); cout << endl;
    cout << "Result in decimal:"; result.printDEC(); cout << endl;

}
  cout << "Good bye !" << endl;
  return 0;
}
