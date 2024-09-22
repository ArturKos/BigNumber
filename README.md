# BigNumber
Implementation of numbers in C ++ using related lists. 
The size of the number depends on the architecture on which the code is run. Of course, the number of bits is greater than 64 and allows you to add, subtract, multiply and divide numbers. 
Perhaps the possibility of exponentiation, priming and checking if a given number is prime will be designed. 
Binary, hexadecimal and decimal number display available. 

Project in progress. Currently working on tests and improving library.

To compile tessts, need to have GoogleTest framework and CMake installed on your system.

To compile tests, follow:

-mkdir build

-cd build

-cmake ..

-make

-./runTests

# Latest commit verified by valgrind:

valgrind ./runTests 

==650208== Memcheck, a memory error detector

==650208== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.

==650208== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info

==650208== Command: ./runTests

==650208== 

[==========] Running 43 tests from 4 test suites.

[----------] Global test environment set-up.

[----------] 3 tests from TestBitOperation

[ RUN      ] TestBitOperation.bitleft

[       OK ] TestBitOperation.bitleft (22 ms)

[ RUN      ] TestBitOperation.bitright

[       OK ] TestBitOperation.bitright (7 ms)

[ RUN      ] TestBitOperation.addone

[       OK ] TestBitOperation.addone (7 ms)

[----------] 3 tests from TestBitOperation (43 ms total)

[----------] 8 tests from TestOperator8bit

[ RUN      ] TestOperator8bit.operatorPlus

[       OK ] TestOperator8bit.operatorPlus (4303 ms)

[ RUN      ] TestOperator8bit.operatorMinus

[       OK ] TestOperator8bit.operatorMinus (4846 ms)

[ RUN      ] TestOperator8bit.operatorBiggerEqual

[       OK ] TestOperator8bit.operatorBiggerEqual (4425 ms)

[ RUN      ] TestOperator8bit.operatorLower

[       OK ] TestOperator8bit.operatorLower (4770 ms)

[ RUN      ] TestOperator8bit.operatorNotEqual

[       OK ] TestOperator8bit.operatorNotEqual (3489 ms)

[ RUN      ] TestOperator8bit.operatorMultiplication

[       OK ] TestOperator8bit.operatorMultiplication (11906 ms)

[ RUN      ] TestOperator8bit.operatorDivide

[       OK ] TestOperator8bit.operatorDivide (18125 ms)

[ RUN      ] TestOperator8bit.operatorDivideMmodulo

[       OK ] TestOperator8bit.operatorDivideMmodulo (16423 ms)

[----------] 8 tests from TestOperator8bit (68292 ms total)

[----------] 16 tests from TestOperator16bit

[ RUN      ] TestOperator16bit.operatorBiggerEqual

[       OK ] TestOperator16bit.operatorBiggerEqual (78845 ms)

[ RUN      ] TestOperator16bit.operatorLower

[       OK ] TestOperator16bit.operatorLower (91211 ms)

[ RUN      ] TestOperator16bit.operatorNotEqual

[       OK ] TestOperator16bit.operatorNotEqual (56650 ms)

[ RUN      ] TestOperator16bit.operatorBiggerEqual_static_inputl

[       OK ] TestOperator16bit.operatorBiggerEqual_static_inputl (4 ms)

[ RUN      ] TestOperator16bit.operatorLower_static_input

[       OK ] TestOperator16bit.operatorLower_static_input (3 ms)

[ RUN      ] TestOperator16bit.operatorNotEqual_static_input

[       OK ] TestOperator16bit.operatorNotEqual_static_input (4 ms)

[ RUN      ] TestOperator16bit.operatorPlus_static_input

[       OK ] TestOperator16bit.operatorPlus_static_input (4 ms)

[ RUN      ] TestOperator16bit.operatorMinus_static_input

[       OK ] TestOperator16bit.operatorMinus_static_input (5 ms)

[ RUN      ] TestOperator16bit.operatorMultiplication_static_input

[       OK ] TestOperator16bit.operatorMultiplication_static_input (6 ms)

[ RUN      ] TestOperator16bit.operatorDivide_static_input

[       OK ] TestOperator16bit.operatorDivide_static_input (4 ms)

[ RUN      ] TestOperator16bit.operatorDivideModulo_static_input

[       OK ] TestOperator16bit.operatorDivideModulo_static_input (5 ms)

[ RUN      ] TestOperator16bit.operatorPlus

[       OK ] TestOperator16bit.operatorPlus (71600 ms)

[ RUN      ] TestOperator16bit.operatorMinus

[       OK ] TestOperator16bit.operatorMinus (101231 ms)

[ RUN      ] TestOperator16bit.operatorMultiplication

[       OK ] TestOperator16bit.operatorMultiplication (339884 ms)

[ RUN      ] TestOperator16bit.operatorDivide

[       OK ] TestOperator16bit.operatorDivide (1011939 ms)

[ RUN      ] TestOperator16bit.operatorDivideModulo

[       OK ] TestOperator16bit.operatorDivideModulo (491188 ms)

[----------] 16 tests from TestOperator16bit (2242594 ms total)

[----------] 16 tests from TestOperator32bit

[ RUN      ] TestOperator32bit.operatorBiggerEqual

[       OK ] TestOperator32bit.operatorBiggerEqual (85867 ms)

[ RUN      ] TestOperator32bit.operatorLower

[       OK ] TestOperator32bit.operatorLower (109307 ms)

[ RUN      ] TestOperator32bit.operatorNotEqual

[       OK ] TestOperator32bit.operatorNotEqual (53700 ms)

[ RUN      ] TestOperator32bit.operatorBiggerEqual_static_inputl

[       OK ] TestOperator32bit.operatorBiggerEqual_static_inputl (4 ms)

[ RUN      ] TestOperator32bit.operatorLower_static_input

[       OK ] TestOperator32bit.operatorLower_static_input (4 ms)

[ RUN      ] TestOperator32bit.operatorNotEqual_static_input

[       OK ] TestOperator32bit.operatorNotEqual_static_input (4 ms)

[ RUN      ] TestOperator32bit.operatorPlus_static_input

[       OK ] TestOperator32bit.operatorPlus_static_input (6 ms)

[ RUN      ] TestOperator32bit.operatorMinus_static_input

[       OK ] TestOperator32bit.operatorMinus_static_input (7 ms)

[ RUN      ] TestOperator32bit.operatorMultiplication_static_input

[       OK ] TestOperator32bit.operatorMultiplication_static_input (6 ms)

[ RUN      ] TestOperator32bit.operatorDivide_static_input

[       OK ] TestOperator32bit.operatorDivide_static_input (7 ms)

[ RUN      ] TestOperator32bit.operatorDivideModulo_static_input

[       OK ] TestOperator32bit.operatorDivideModulo_static_input (5 ms)

[ RUN      ] TestOperator32bit.operatorPlus

[       OK ] TestOperator32bit.operatorPlus (83833 ms)

[ RUN      ] TestOperator32bit.operatorMinus

[       OK ] TestOperator32bit.operatorMinus (131681 ms)

[ RUN      ] TestOperator32bit.operatorMultiplication

[       OK ] TestOperator32bit.operatorMultiplication (843471 ms)

[ RUN      ] TestOperator32bit.operatorDivide

[       OK ] TestOperator32bit.operatorDivide (1229396 ms)

[ RUN      ] TestOperator32bit.operatorDivideModulo

[       OK ] TestOperator32bit.operatorDivideModulo (1206704 ms)

[----------] 16 tests from TestOperator32bit (3744012 ms total)

[----------] Global test environment tear-down

[==========] 43 tests from 4 test suites ran. (6054995 ms total)

[  PASSED  ] 43 tests.

==650208== 

==650208== HEAP SUMMARY:

==650208==     in use at exit: 0 bytes in 0 blocks

==650208==   total heap usage: 1,594,405,264 allocs, 1,594,405,264 frees, 38,265,877,980 bytes allocated

==650208== 

==650208== All heap blocks were freed -- no leaks are possible

==650208== 

==650208== For lists of detected and suppressed errors, rerun with: -s

==650208== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

