# Langlands Explorer Toolkit

A C++20 computational toolkit for exploring aspects of the Langlands correspondence, with a focus on modular forms, automorphic representations, and L-functions.

## Overview

This project implements sophisticated mathematical concepts from number theory and representation theory to enable computational exploration of the Langlands program. The toolkit provides:

- **Advanced number theory**: Number fields, class groups, unit groups
- **Modular forms theory**: Spaces of modular forms, Hecke operators, q-expansions
- **Automorphic representations**: Adelic representations, local components
- **L-functions**: Dirichlet L-functions, modular form L-functions, functional equations
- **Galois representations**: p-adic representations, local-global compatibility

## Mathematical Background

The Langlands proposes profound connections between:
- Representation theory of reductive groups (automorphic side)
- Number theory and Galois representations (arithmetic side)

This toolkit focuses on computationally accessible aspects, particularly:
- Classical modular forms and their relation to elliptic curves (modularity theorem)
- Hecke eigenvalues and Frobenius traces
- Functional equations of L-functions
- p-adic Hodge theory in simple cases

## Features

- **Arbitrary precision arithmetic**: Using Boost.Multiprecision for rigorous computations
- **Modern C++20**: Concepts, coroutines, modules (when supported)
- **Linear algebra**: Eigen library for efficient matrix operations
- **Extensible design**: Template-based architecture for easy extension
- **Rigorous testing**: Comprehensive unit test suite
- **Visualization**: Optional plotting capabilities for modular forms and L-function zeros

## Building the Project

### Prerequisites

- C++20 compatible compiler (GCC 11+, Clang 12+, MSVC 2019+)
- CMake 3.18+
- Boost 1.75+
- Eigen 3.3+
- Optional: PARI/GP for cross-verification, Qwt for visualization

### Compilation

```bash
# Clone the repository
git clone <repository-url>
cd langlands-toolkit

# Create build directory
mkdir build && cd build

# Configure
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build .

# Run tests (if enabled)
ctest
```

### CMake Options

- `BUILD_EXAMPLES`: Build example programs (default: ON)
- `BUILD_TESTS`: Build unit tests (default: ON)
- `ENABLE_VISUALIZATION`: Enable visualization modules (default: ON)
- `USE_SYSTEM_PARI`: Use system PARI/GP library (default: OFF)

## Usage Examples

### Working with Modular Forms

```cpp
#include <langlands/modular/modular_form.hpp>
#include <iostream>

int main() {
    // Create space of weight 2 modular forms for Γ₀(11)
    langlands::modular::Gamma0 gamma0(11);
    langlands::modular::ModularFormsSpace<langlands::modular::Gamma0> 
        space(gamma0, 2, true, true);  // weight 2, holomorphic, cuspidal
    
    std::cout << "Dimension of S_2(Γ₀(11)): " << space.dimension() << std::endl;
    
    // Access basis elements
    auto f = space.basis_element(0);
    
    // Evaluate at a point in the upper half-plane
    std::complex<langlands::core::precision::float_type<>> tau(0.0, 1.0);
    std::complex<langlands::core::precision::float_type<>> value = f(tau);
    
    std::cout << "f(i) = " << value << std::endl;
    
    // Compute Hecke operators
    auto T2 = space.hecke_operator(2);
    std::cout << "Hecke operator T_2 matrix:\n" << T2 << std::endl;
    
    return 0;
}
```

### Number Field Computations

```cpp
#include <langlands/ntheory/number_field.hpp>
#include <iostream>

int main() {
    // Define quadratic field ℚ(√5) via polynomial x² - 5
    std::vector<langlands::core::precision::int_type<>> coeffs = {
        langlands::core::precision::literals::operator"" _idq(-5),  // constant term
        langlands::core::precision::literals::operator"" _idq(0),   // linear term
        langlands::core::precision::literals::operator"" _idq(1)    // quadratic term
    };
    
    langlands::ntheory::NumberField K(coeffs);
    
    std::cout << "Number field: " << K.to_string() << std::endl;
    std::cout << "Degree: " << K.degree() << std::endl;
    std::cout << "Discriminant: " << K.discriminant() << std::endl;
    
    // Check if 5 is ramified
    std::cout << "Is 5 ramified? " << (K.is_ramified(5) ? "Yes" : "No") << std::endl;
    
    // Decompose prime 11
    auto decomposition = K.decompose_prime(11);
    std::cout << "Decomposition of 11: ";
    for (const auto& [prime, exp] : decomposition) {
        std::cout << "𝔭^" << exp << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

## Project Structure

```
langlands-toolkit/
├── include/                    # Header files
│   ├── langlands/              # Main namespace
│   │   ├── core/               # Core utilities (exceptions, precision)
│   │   ├── ntheory/            # Number theory modules
│   │   ├── modular/            # Modular forms theory
│   │   ├── automorphic/        # Automorphic representations
│   │   ├── lfunctions/         # L-functions
│   │   ├── galois/             # Galois representations
│   │   └── visualization/      # Optional visualization components
│   └── ... 
├── src/                        # Source files
│   ├── core/
│   ├── ntheory/
│   ├── modular/
│   ├── automorphic/
│   ├── lfunctions/
│   ├── galois/
│   └── visualization/
├── tests/                      # Unit tests (Catch2)
├── examples/                   # Example programs
├── visualization/              # Visualization modules (if enabled)
├── CMakeLists.txt              # Build configuration
├── README.md                   # This file
└── LICENSE                     # MIT License
```

## Mathematical References

The implementation draws from standard references including:

1. **Number Theory**:
   - H. Cohen, *A Course in Computational Algebraic Number Theory*
   - D. A. Cox, *Primes of the Form x² + ny²*

2. **Modular Forms**:
   - H. Cohen & Strömberg, *Modular Forms: A Classical Approach*
   - T. Miyake, *Modular Forms*
   - D. Zagier, *Modular Forms and Differential Operators*

3. **Automorphic Forms**:
   - D. Bump, *Automorphic Forms and Representations*
   - S. Gelbart, *Automorphic Forms on Adele Groups*

4. **L-functions**:
   - H. Iwaniec & E. Kowalski, *Analytic Number Theory*
   - J. Tate, *Fourier Analysis in Number Fields and Hecke's Zeta Functions*

5. **Galois Representations**:
   - R. Taylor & A. Wiles, *Ring-Theoretic Properties of Certain Hecke Algebras*
   - K. Buzzard, *Analytic continuation of Artin L-functions*

## Contributing

Contributions are welcome! Please feel free to submit pull requests, open issues for bugs or feature requests, or suggest improvements.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- The PARI/GP team for their invaluable number theory library
- The Boost and Eigen teams for excellent C++ libraries
- The mathematical community whose work makes this exploration possible