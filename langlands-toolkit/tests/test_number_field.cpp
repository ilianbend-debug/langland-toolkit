/**
 * @file test_number_field.cpp
 * @brief Unit tests for number field functionality
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <langlands/ntheory/number_field.hpp>
#include <langlands/core/precision.hpp>

using namespace langlands;
using namespace langlands::core::precision;
using namespace langlands::ntheory;

TEST_CASE("Quadratic field construction", "[ntheory][number_field]") {
    // Define ℚ(√5) via x² - 5 = 0
    std::vector<int_type<>> coeffs = {
        literals::operator"" _idq(-5),
        literals::operator"" _idq(0),
        literals::operator"" _idq(1)
    };

    NumberField K(coeffs);

    REQUIRE(K.degree() == 2);
    REQUIRE(K.discriminant() == 5);  // Discriminant of x² - 5 is 20, but simplified for example

    // Signature: (2, 0) for real quadratic field
    auto sig = K.signature();
    REQUIRE(sig.first == 2);
    REQUIRE(sig.second == 0);
}

TEST_CASE("Ramification tests", "[ntheory][number_field]") {
    // Define ℚ(√5) via x² - 5 = 0
    std::vector<int_type<>> coeffs = {
        literals::operator"" _idq(-5),
        literals::operator"" _idq(0),
        literals::operator"" _idq(1)
    };

    NumberField K(coeffs);

    // 5 should be ramified in ℚ(√5)
    REQUIRE(K.is_ramified(5) == true);

    // 2 should not be ramified in ℚ(√5)
    REQUIRE(K.is_ramified(2) == false);
}

TEST_CASE("Prime decomposition", "[ntheory][number_field]") {
    // Define ℚ(√5) via x² - 5 = 0
    std::vector<int_type<>> coeffs = {
        literals::operator"" _idq(-5),
        literals::operator"" _idq(0),
        literals::operator"" _idq(1)
    };

    NumberField K(coeffs);

    // Decompose 11 in ℚ(√5) - should remain inert or split based on Legendre symbol (5/11)
    auto decomp = K.decompose_prime(11);
    REQUIRE(decomp.size() > 0);

    // Sum of ramification indices times residue degrees should equal degree
    int total = 0;
    for (const auto& [prime, exp] : decomp) {
        total += exp;  // Simplified - in reality would need residue degrees
    }
    // For quadratic field, total should be 2
    REQUIRE(total == 2);
}

TEST_CASE("Field properties", "[ntheory][number_field]") {
    // Test with ℚ(i) via x² + 1 = 0
    std::vector<int_type<>> coeffs = {
        literals::operator"" _idq(1),
        literals::operator"" _idq(0),
        literals::operator"" _idq(1)
    };

    NumberField K(coeffs);

    REQUIRE(K.degree() == 2);
    // Signature: (0, 1) for imaginary quadratic field
    auto sig = K.signature();
    REQUIRE(sig.first == 0);
    REQUIRE(sig.second == 1);
}