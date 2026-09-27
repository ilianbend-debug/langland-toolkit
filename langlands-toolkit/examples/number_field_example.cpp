/**
 * @example number_field_example.cpp
 * @brief Example demonstrating number field computations
 *
 * This example shows how to:
 * 1. Define number fields via polynomials
 * 2. Compute basic invariants (degree, discriminant, signature)
 * 3. Work with algebraic numbers
 * 4. Check ramification and decompose primes
 */

#include <langlands/ntheory/number_field.hpp>
#include <langlands/core/precision.hpp>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace langlands;
using namespace langlands::core::precision;

int main() {
    try {
        // Set precision for output
        PrecisionManager::set_precision(50);
        std::cout << std::setprecision(50);

        std::cout << "=== Langlands Explorer Toolkit ===" << std::endl;
        std::cout << "Example: Number Field Computations" << std::endl << std::endl;

        // Example 1: Quadratic field ℚ(√5)
        std::cout << "1. Quadratic field ℚ(√5) defined by x² - 5 = 0" << std::endl;
        {
            // Polynomial: x² - 5 = x² + 0·x - 5
            std::vector<int_type<>> coeffs = {
                literals::operator"" _idq(-5),  // constant term
                literals::operator"" _idq(0),   // linear term
                literals::operator"" _idq(1)    // quadratic term
            };

            ntheory::NumberField K(coeffs);

            std::cout << "   Field: " << K.to_string() << std::endl;
            std::cout << "   Degree: " << K.degree() << std::endl;
            std::cout << "   Discriminant: " << K.discriminant() << std::endl;

            // Signature (r1, r2) where r1 = # real embeddings, r2 = # complex conjugate pairs
            auto sig = K.signature();
            std::cout << "   Signature: (r1, r2) = (" << sig.first << ", " << sig.second << ")" << std::endl;

            // Check if 5 is ramified (it should be, since 5 divides the discriminant)
            std::cout << "   Is 5 ramified? " << (K.is_ramified(5) ? "Yes" : "No") << std::endl;

            // Decompose prime 11
            std::cout << "   Decomposition of 11: ";
            auto decomp_11 = K.decompose_prime(11);
            for (size_t i = 0; i < decomp_11.size(); ++i) {
                if (i > 0) std::cout << " · ";
                std::cout << "𝔭_" << (i+1) << "^" << decomp_11[i].second;
            }
            std::cout << std::endl << std::endl;
        }

        // Example 2: Cyclotomic field ℚ(ζ₅) - 5th cyclotomic field
        std::cout << "2. Cyclotomic field ℚ(ζ₅) (5th roots of unity)" << std::endl;
        {
            // 5th cyclotomic polynomial: Φ₅(x) = x⁴ + x³ + x² + x + 1
            std::vector<int_type<>> coeffs = {
                literals::operator"" _idq(1),   // constant term
                literals::operator"" _idq(1),   // x term
                literals::operator"" _idq(1),   // x² term
                literals::operator"" _idq(1),   // x³ term
                literals::operator"" _idq(1)    // x⁴ term
            };

            ntheory::NumberField K(coeffs);

            std::cout << "   Field: " << K.to_string() << std::endl;
            std::cout << "   Degree: " << K.degree() << std::endl;
            std::cout << "   Discriminant: " << K.discriminant() << std::endl;

            auto sig = K.signature();
            std::cout << "   Signature: (r1, r2) = (" << sig.first << ", " << sig.second << ")" << std::endl;

            // In cyclotomic fields, only primes dividing n are ramified
            std::cout << "   Is 5 ramified? " << (K.is_ramified(5) ? "Yes" : "No") << std::endl;
            std::cout << "   Is 2 ramified? " << (K.is_ramified(2) ? "Yes" : "No") << std::endl << std::endl;
        }

        // Example 3: Working with algebraic numbers
        std::cout << "3. Algebraic number arithmetic" << std::endl;
        {
            // Define √2 as a root of x² - 2 = 0
            std::vector<int_type<>> coeffs_sqrt2 = {
                literals::operator"" _idq(-2),
                literals::operator"" _idq(0),
                literals::operator"" _idq(1)
            };
            ntheory::NumberField Q_sqrt2(coeffs_sqrt2);

            // Create algebraic numbers
            ntheory::AlgebraicNumber two(Q_sqrt2.generator()); // This would be √2 in a proper implementation

            // In a full implementation, we would do:
            // ntheory::AlgebraicNumber sqrt2(Q_sqrt2.generator()); // represents √2
            // ntheory::AlgebraicNumber two = sqrt2 * sqrt2;       // should equal 2

            std::cout << "   Algebraic number operations would be available" << std::endl;
            std::cout << "   (Full implementation in progress)" << std::endl << std::endl;
        }

        // Example 4: Class group and unit group (placeholders)
        std::cout << "4. Advanced invariants (to be implemented)" << std::endl;
        {
            std::cout << "   Class group computation: [Planned - Buchmann algorithm]" << std::endl;
            std::cout << "   Unit group computation: [Planned - Dirichlet's algorithm]" << std::endl;
            std::cout << "   Regulator computation: [Planned]" << std::endl << std::endl;
        }

        std::cout << "=== Example completed successfully ===" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}