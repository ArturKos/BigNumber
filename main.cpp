#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "bignumber.h"
#include <bitset>

using namespace std;

int main()
{
    bignumber a1((uint8_t) 6);
    bignumber b1((uint8_t) 4);
    bignumber sum;
 //   char key = 0;
  //  a1.printBIN();
    std::cout << endl;
   // while(key!=27){
 //   b1.printBIN();
    std::cout << endl;
   // cin >> key;
   // b1.bitleft();
   // }
    //bn.not_bits();
    //sum = a1 / b1;
 /*   digit *tmp1 = a1.GetLastDigit();
    while(tmp1!=NULL)
    {
     uint8_t u1 = tmp1->GetData();
     std::cout <<"tmp1="<< std::bitset<8> ((uint8_t)u1) << std::endl;
     tmp1 = tmp1->GetPrevDigit();
    }*/

//sum = a1 / b1;
//    std::cout << "Binary division "<< std::endl;
   // sum.printBIN();
    a1.printDEC();
    return 0;
}
