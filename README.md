# BigNumber

Arbitrary-precision integer arithmetic library implemented in C++ using doubly-linked lists.

Each number is stored as a chain of 8-bit nodes, allowing integers far beyond the 64-bit range. The sign is tracked separately.

## Features

- **Arithmetic:** `+`, `-`, `*`, `/`, `%`
- **Comparison:** `==`, `!=`, `<`, `<=`, `>`, `>=`
- **Bit operations:** `shift_left`, `shift_right`, `not_bits`
- **Number theory:** `is_prime` (trial division, 6k+/-1), `isqrt` (binary search)
- **Output:** binary, decimal, hexadecimal (string conversion and stdout printing)
- **Stream support:** `operator<<` for direct use with `std::cout`

## Build and run tests

Requires GoogleTest and CMake.

```bash
mkdir build
cd build
cmake ..
make
./runTests
```

## Documentation

API documentation is generated with [Doxygen](https://www.doxygen.nl/). To regenerate:

```bash
doxygen Doxyfile
```

The output is written to `docs/html/`. Open `docs/html/index.html` in a browser to view.

## Verify with Valgrind

```bash
cd build
valgrind --leak-check=full ./runTests
```
