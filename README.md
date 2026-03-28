# BigNumber

An arbitrary-precision integer arithmetic library implemented in **C++** using a **doubly-linked list** of 8-bit digit nodes. Each number is stored as a chain of bytes from least significant to most significant, allowing integers far beyond the native 64-bit range. Fully tested with **GoogleTest** and verified leak-free with **Valgrind**.

![C++](https://img.shields.io/badge/C%2B%2B-11-blue)
![GoogleTest](https://img.shields.io/badge/GoogleTest-tested-green)
![Valgrind](https://img.shields.io/badge/Valgrind-leak--free-brightgreen)
![CMake](https://img.shields.io/badge/CMake-3.14+-orange)
![Doxygen](https://img.shields.io/badge/Docs-Doxygen-blue)

## Features

- **Arithmetic operators**: `+`, `-`, `*`, `/`, `%` with full operator overloading
- **Comparison operators**: `==`, `!=`, `<`, `<=`, `>`, `>=`
- **Increment / Decrement**: prefix `++` and `--`
- **Bit manipulation**: `shift_left` (multiply by 2), `shift_right` (divide by 2), `not_bits` (bitwise NOT), `shift_left_and_set_lsb`
- **Primality testing**: trial division with the 6k +/- 1 optimization, testing divisors up to floor(sqrt(n))
- **Integer square root**: binary search algorithm returning floor(sqrt(n))
- **Multiple constructors**: from `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`, and copy constructor
- **Output formats**: binary, decimal, and hexadecimal string conversion with `to_string_bin()`, `to_string_dec()`, `to_string_hex()`
- **Stream support**: `operator<<` for direct use with `std::cout`
- **Sign tracking**: separate boolean flag for negative numbers with correct sign propagation through arithmetic
- **Two's complement subtraction**: negation via bitwise NOT + add one, then standard addition
- **Shift-and-add multiplication**: O(bits^2) binary long multiplication
- **Shift-and-subtract division**: O(bits^2) binary long division for both quotient and remainder
- **Memory-safe**: leading zero bytes are automatically stripped via `optimize()`; destructor frees all nodes

## Dependencies

| Library | Version | Purpose |
|---------|---------|---------|
| [GoogleTest](https://github.com/google/googletest) | any | Unit testing framework |
| CMake | >= 3.14 | Build system |
| [Valgrind](https://valgrind.org/) | any | Memory leak verification (optional) |
| [Doxygen](https://www.doxygen.nl/) | any | API documentation generation (optional) |

### Installing dependencies (Ubuntu / Debian)

```bash
sudo apt-get install cmake libgtest-dev valgrind doxygen
```

## Building and Running Tests

```bash
git clone https://github.com/ArturKos/BigNumber.git
cd BigNumber
mkdir build && cd build
cmake ..
make -j$(nproc)
./runTests
```

The test suite includes:

- Bit shift operations (left/right) across the full 64-bit range
- 16-bit and 32-bit addition, subtraction, multiplication, division, and modulo
- Edge cases: byte boundaries (0xFF/0x100, 0xFFFF/0x10000), zero, one, INT16_MAX, INT32_MAX
- Randomized test vectors (1024 samples per bit width)
- Comparison operators on randomized data
- Primality testing against known primes and composites
- Integer square root verification

## Memory Verification

```bash
cd build
valgrind --leak-check=full ./runTests
```

The library is verified to produce zero memory leaks -- all linked list nodes are properly freed.

## API Documentation

Full API documentation is generated with Doxygen. Every class, method, and parameter is documented in the header files.

```bash
doxygen Doxyfile
```

The output is written to `docs/html/`. Open `docs/html/index.html` in a browser to view.

## Quick Example

```cpp
#include "include/bignumber.h"

bignumber a((uint32_t)123456789);
bignumber b((uint32_t)987654321);

bignumber sum = a + b;
bignumber product = a * b;
bignumber quotient = b / a;
bignumber remainder = b % a;

std::cout << "Sum: " << sum << std::endl;           // decimal output
std::cout << "Hex: " << product.to_string_hex() << std::endl;
std::cout << "Bin: " << quotient.to_string_bin() << std::endl;

if (a.is_prime()) {
    std::cout << "a is prime" << std::endl;
}

bignumber root = product.isqrt(NULL);
std::cout << "sqrt: " << root << std::endl;
```

## Internal Representation

Each `bignumber` is a doubly-linked list of `digit` nodes:

```
first_digit (LSB) <-> digit <-> digit <-> ... <-> last_digit (MSB)
```

- Each `digit` stores one `uint8_t` (8 bits) and pointers to `prev`/`next` neighbors
- The `negative` flag tracks the sign independently
- Arithmetic operates byte-by-byte with carry propagation
- Leading zero bytes are automatically removed by `optimize()`

## Project Structure

```
BigNumber/
├── CMakeLists.txt              # Build configuration (GoogleTest + pthread)
├── Doxyfile                    # Doxygen configuration
├── README.md                   # This file
├── main.cpp                    # Interactive demo (read two numbers, show all operations)
├── test.cpp                    # GoogleTest suite (1024+ randomized + edge-case tests)
├── include/
│   ├── bignumber.h             # bignumber class declaration with full Doxygen docs
│   └── digit.h                 # digit node declaration (8-bit doubly-linked list node)
├── src/
│   ├── bignumber.cpp           # All arithmetic, comparison, conversion, and number theory
│   └── digit.cpp               # Digit node construction, accessors, and operators
└── makefile                    # Alternative Makefile build
```

## Algorithms

| Operation | Algorithm | Complexity |
|-----------|-----------|------------|
| Addition | Byte-by-byte with carry | O(n) |
| Subtraction | Two's complement + addition | O(n) |
| Multiplication | Shift-and-add (binary long mult) | O(n^2) |
| Division | Shift-and-subtract (binary long div) | O(n^2) |
| Modulo | Shift-and-subtract (returns remainder) | O(n^2) |
| Primality | Trial division, 6k +/- 1 | O(sqrt(n) * n) |
| Square root | Binary search | O(log(n) * n^2) |

Where n = number of bits in the operands.

## License

This project is provided as-is for educational purposes.

---

**Author:** Artur Kos
