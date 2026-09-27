/**
 * @example modular_form_example.cpp
 * @brief Example demonstrating basic modular form computations
 *
 * This example shows how to:
 * 1. Create a space of modular forms
 * 2. Compute its dimension
 * 3. Access basis elements
 * 4. Evaluate forms at points in the upper half-plane
 * 5. Compute Hecke operators
 */

#include <langlands/modular/modular_form.hpp>
#include <langlands/core/precision.hpp>
#include <iostream>
#include <iomanip>

using namespace langlands;
using namespace langlands::core::precision;

int main() {
    try {
        // Set precision for output
        PrecisionManager::set_precision(50);
        std::cout << std::setprecision(50);

        std::cout << "=== Langlands Explorer Toolkit ===" << std::endl;
        std::cout << "Example: Modular Forms for Γ₀(11)" << std::endl << std::endl;

        // Create the congruence subgroup Γ₀(11)
        modular::Gamma0 gamma0(11);
        std::cout << "Level: " << gamma0.level() << std::endl;
        std::cout << "Index [SL(2,ℤ):Γ₀(11)]: " << gamma0.index() << std::endl << std::endl;

        // Create space of weight 2 cusp forms for Γ₀(11)
        // This corresponds to the space associated with the elliptic curve y² + y = x³ - x²
        modular::ModularFormsSpace<modular::Gamma0> space(gamma0, 2, true, true);

        std::cout << "Space: S₂(Γ₀(11))" << std::endl;
        std::cout << "Dimension: " << space.dimension() << std::endl << std::endl;

        // Access the first basis element
        if (space.dimension() > 0) {
            auto f = space.basis_element(0);
            std::cout << "First basis element f(τ) accessible" << std::endl;

            // Evaluate at several points in the upper half-plane
            std::cout << "\nEvaluations of f(τ) at various points:" << std::endl;

            // Point τ = i
            std::complex<float_type<>> tau_i(0.0, 1.0);
            std::complex<float_type<>> val_i = f(tau_i);
            std::cout << "f(i) = " << val_i << std::endl;

            // Point τ = 2i
            std::complex<float_type<>> tau_2i(0.0, 2.0);
            std::complex<float_type<>> val_2i = f(tau_2i);
            std::cout << "f(2i) = " << val_2i << std::endl;

            // Point τ = (1 + i√3)/2 (primitive 6th root of unity in ℍ)
            std::complex<float_type<>> tau_rho(0.5, std::sqrt(3.0L)/2.0);
            std::complex<float_type<>> val_rho = f(tau_rho);
            std::cout << "f(ρ) = " << val_rho << std::endl;

            // Compute q-expansion (first 10 terms)
            std::cout << "\nq-expansion of f(τ) (first 10 terms):" << std::endl;
            auto qexp = space.q_expansion(0, 10);
            std::cout << "f(τ) = ";
            for (size_t n = 0; n < qexp.size(); ++n) {
                if (n > 0) std::cout << " + ";
                std::cout << qexp[n];
                if (n > 0) std::cout << "·q^" << n;
            }
            std::cout << " + O(q^10)" << std::endl;

            // Compute Hecke operator T₂
            std::cout << "\nHecke operator T₂:" << std::endl;
            auto T2_matrix = space.hecke_operator(2);
            std::cout << "T₂ = \n" << T2_matrix << std::endl;

            // Apply T₂ to the basis element f
            std::cout << "T₂(f) = ";
            // In a real implementation, we would compute this properly
            std::cout << "[Linear combination of basis elements]" << std::endl;

        } else {
            std::cout << "Warning: Space dimension is zero!" << std::endl;
        }

        std::cout << "\n=== Example completed successfully ===" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}