/**
 * @file test_core.cpp
 * @brief Unit tests for core utilities
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <langlands/core/exceptions.hpp>
#include <langlands/core/precision.hpp>

using namespace langlands;
using namespace langlands::core;
using namespace langlands::core::precision;

TEST_CASE("PrecisionManager initialization", "[core][precision]") {
    // Test default initialization
    PrecisionManager::initialize();
    REQUIRE(PrecisionManager::get_current_precision() == default_digits10);

    // Test custom initialization
    PrecisionManager::initialize(50);
    REQUIRE(PrecisionManager::get_current_precision() == 50);

    // Test setting precision
    PrecisionManager::set_precision(30);
    REQUIRE(PrecisionManager::get_current_precision() == 30);

    // Test exceeding maximum precision
    REQUIRE_THROWS_AS(PrecisionManager::initialize(max_digits10 + 1), PrecisionException);
}

TEST_CODE("Exception hierarchy", "[core][exceptions]") {
    // Test base exception
    REQUIRE_THROWS_AS(
        throw LanglandsException("Test message"),
        LanglandsException
    );

    // Test derived exceptions
    REQUIRE_THROWS_AS(
        throw PrecisionException("Precision error"),
        PrecisionException
    );
    REQUIRE_THROWS_AS(
        throw AlgebraicException("Algebraic error"),
        AlgebraicException
    );
    REQUIRE_THROWS_AS(
        throw AnalysisException("Analysis error"),
        AnalysisException
    );

    // Test what() messages
    try {
        throw PrecisionException("Test precision error");
    } catch (const PrecisionException& e) {
        CHECK(std::string(e.what()).find("Precision error") != std::string::npos);
    }
}

TEST_CASE("Literal operators", "[core][precision]") {
    // Test integer literal
    auto i = 123_idq;
    REQUIRE(i.convert_to<int>() == 123);

    // Test long double literal
    auto ld = 3.14159_ldq;
    REQUIRE(ld.convert_to<long double>() == Approx(3.14159L));
}