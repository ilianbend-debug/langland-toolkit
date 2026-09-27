#ifndef LANGLANDS_MODULAR_MODULAR_FORM_HPP
#define LANGLANDS_MODULAR_MODULAR_FORM_HPP

#include <vector>
#include <complex>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <langlands/core/precision.hpp>
#include <langlands/core/exceptions.hpp>
#include <langlands/ntheory/number_field.hpp>
#include <Eigen/Dense>

namespace langlands {
namespace modular {

using namespace langlands::core;
using namespace langlands::core::precision;
using namespace langlands::ntheory;

/**
 * @brief Groupe modulaire SL(2,ℤ) et ses sous-groupes de congruence
 */
namespace groups {

/**
 * @brief Groupe modulaire complet SL(2,ℤ)
 */
class SL2Z {
public:
    /**
     * @brief Vérifie si une matrice appartient à SL(2,ℤ)
     * @param a, b, c, d Éléments de la matrice [[a,b],[c,d]]
     * @return true si la matrice est dans SL(2,ℤ)
     */
    static bool contains(const int_type<>& a, const int_type<>& b,
                        const int_type<>& c, const int_type<>& d) {
        return (a * d - b * c) == 1;
    }

    /**
     * @brief Générateurs standards: S = [[0,-1],[1,0]] et T = [[1,1],[0,1]]
     */
    static std::array<int_type<>, 4> S() { return {0, -1, 1, 0}; }
    static std::array<int_type<>, 4> T() { return {1, 1, 0, 1}; }
};

/**
 * @brief Sous-groupe de congruence Γ₀(N) = {[a b; c d] ∈ SL(2,ℤ) | c ≡ 0 mod N}
 */
class Gamma0 {
private:
    int_type<> level_;  // Niveau N

public:
    explicit Gamma0(const int_type<>& level) : level_(level) {
        if (level_ <= 0) {
            throw InvalidInputException("Level must be positive");
        }
    }

    /**
     * @brief Vérifie l'appartenance à Γ₀(N)
     */
    bool contains(const int_type<>& a, const int_type<>& b,
                 const int_type<>& c, const int_type<>& d) const {
        return (groups::SL2Z::contains(a, b, c, d) &&
                (c % level_) == 0);
    }

    /**
     * @brief Indice [SL(2,ℤ) : Γ₀(N)]
     * @return Indice du sous-groupe
     */
    int_type<> index() const {
        if (level_ == 1) return 1;

        int_type<> result = level_;
        // Factorisation de N pour calculer φ(N) et l'indice
        auto factors = factor_int(level_);
        for (const auto& [p, exp] : factors) {
            result *= (p - 1) / p;
            result *= pow(p, exp - 1);
        }
        return result;
    }

    /**
     * @brief Largeur des cuspides
     * @return Vector des largeurs des cuspides équivalentes
     */
    std::vector<int_type<>> cusp_widths() const;

    int_type<> level() const { return level_; }
};

/**
 * @brief Sous-groupe de congruence Γ₁(N)
 */
class Gamma1 {
    // Implémentation similaire à Gamma0
};

/**
 * @brief Sous-groupe de congruence Γ(N)
 */
class GammaN {
    // Implémentation similaire
};

} // namespace groups

/**
 * @brief Espace des formes modulaires M_k(Γ)
 *
 * Représente l'espace vectoriel des formes modulaires de poids k
 * pour un sous-groupe de congruence Γ ⊆ SL(2,ℤ)
 */
template<typename GammaType>
class ModularFormsSpace {
private:
    const GammaType& group_;          // Sous-groupe de congruence
    const int_type<> weight_;         // Poids k (entier, peut être demi-entier pour les formes de poids demi-entier)
    const bool is_holomorphic_;       // true pour formes holomorphes, false pour formes faibles
    const bool is_cuspidal_;          // true pour formes cuspidales, false sinon

    // Base calculée de l'espace
    std::vector<std::function<complex_type<>(const complex_type<>&)>> basis_;
    size_t dimension_;                // Dimension de l'espace

    /**
     * @brief Formule de dimension pour Γ₀(N) (poids paire ≥ 2)
     * @return Dimension de M_k(Γ₀(N))
     */
    size_t compute_dimension_gamma0_even() const;

    /**
     * @brief Formule de dimension pour Γ₀(N) (poids impair)
     * @return Dimension de M_k(Γ₀(N))
     */
    size_t compute_dimension_gamma0_odd() const;

    /**
     * @brief Calcule une base par symbole modulaire
     * @return Base de l'espace des formes modulaires
     */
    void compute_basis_via_modular_symbols();

    /**
     * @brief Calcule une base par séries q-expansion
     * @return Base de l'espace des formes modulaires
     */
    void compute_basis_via_q_expansion();

public:
    /**
     * @brief Constructeur
     * @param group Sous-groupe de congruence (ex: Gamma0(N))
     * @param weight Poids k de la forme modulaire
     * @param holomorphic True si forme holomorphe
     * @param cuspidal True si forme cuspidale
     */
    ModularFormsSpace(const GammaType& group,
                      const int_type<>& weight,
                      bool holomorphic = true,
                      bool cuspidal = false)
        : group_(group), weight_(weight),
          is_holomorphic_(holomorphic), is_cuspidal_(cuspidal) {
        if (weight_ < 0) {
            throw InvalidInputException("Weight must be non-negative");
        }
        compute_dimension_and_basis();
    }

    /**
     * @brief Obtient le poids
     * @return Poids k
     */
    int_type<> weight() const { return weight_; }

    /**
     * @brief Obtient le sous-groupe
     * @return Référence au sous-groupe de congruence
     */
    const GammaType& group() const { return group_; }

    /**
     * @brief Obtient la dimension de l'espace
     * @return Dimension de M_k(Γ)
     */
    size_t dimension() const { return dimension_; }

    /**
     * @brief Vérifie si l'espace est celui des formes cuspidales
     * @return true si S_k(Γ), false sinon
     */
    bool is_cuspidal_space() const { return is_cuspidal_; }

    /**
     * @brief Obtient la i-ième forme de base
     * @param index Index de la forme de base (0 ≤ index < dimension())
     * @return Fonction représentant la forme modulaire
     */
    const std::function<complex_type<>(const complex_type<>&)>&
    basis_element(size_t index) const {
        if (index >= basis_.size()) {
            throw std::out_of_range("Basis index out of range");
        }
        return basis_[index];
    }

    /**
     * @brief Évalue une forme modulaire en un point τ ∈ ℍ
     * @param form_index Index de la forme à évaluer
     * @param tau Point dans le demi-plan supérieur (Im(τ) > 0)
     * @return Valeur de la forme modulaire en τ
     */
    complex_type<> evaluate(size_t form_index,
                           const complex_type<>& tau) const {
        if (std::imag(tau) <= 0) {
            throw AnalysisException("Point must be in upper half-plane");
        }
        if (form_index >= basis_.size()) {
            throw std::out_of_range("Form index out of range");
        }
        return basis_[form_index](tau);
    }

    /**
     * @brief q-expansion d'une forme modulaire
     * @param form_index Index de la forme
     * @param precision Nombre de termes q^n à calculer
     * @return Vector des coefficients a_n tels que f(τ) = Σ a_n q^n
     */
    std::vector<complex_type<>>
    q_expansion(size_t form_index, size_t precision) const;

    /**
     * @brief Opérateur de Hecke T_n agissant sur l'espace
     * @param n Indice de l'opérateur de Hecke
     * @return Matrice représentant T_n dans la base calculée
     */
    Eigen::MatrixXd hecke_operator(int_type<> n) const;

    /**
     * @brief Vérifie si une forme est propre (propre à tous les T_n avec (n,N)=1)
     * @param form_index Index de la forme à tester
     * @return true si la forme est propre
     */
    bool is_eigenform(size_t form_index) const;

    /**
     * @brief Valeurs propres de Hecke pour une forme propre
     * @param form_index Index de la forme propre
     * @return Vector des valeurs propres λ_n pour T_n
     */
    std::vector<complex_type<>>
    hecke_eigenvalues(size_t form_index) const;

    /**
     * @brief Affichage de l'espace
     * @return Description de l'espace des formes modulaires
     */
    std::string to_string() const;
};

/**
 * @brief Forme modulaire spécifique
 *
 * Représente une forme modulaire f ∈ M_k(Γ) comme combinaison linéaire
 * d'une base connue
 */
template<typename GammaType>
class ModularForm {
private:
    const ModularFormsSpace<GammaType>& space_;  // Espace auquel appartient la forme
    Eigen::VectorXd coefficients_;               // Coefficients dans la base de l'espace
    bool is_normalized_;                         // True si a_1 = 1 (normalisée)

public:
    /**
     * @brief Constructeur à partir de coefficients dans une base
     * @param space Espace modulaire contenant la forme
     * @param coeffs Coefficients dans la base de l'espace
     */
    ModularForm(const ModularFormsSpace<GammaType>& space,
                const Eigen::VectorXd& coeffs)
        : space_(space), coefficients_(coeffs) {
        if (coeffs.size() != space.dimension()) {
            throw std::invalid_argument(
                "Coefficient vector size must match space dimension");
        }
        is_normalized_ = false;  // À déterminer par calcul
    }

    /**
     * @brief Constructeur pour la forme nulle
     */
    explicit ModularForm(const ModularFormsSpace<GammaType>& space)
        : space_(space),
          coefficients_(Eigen::VectorXd::Zero(space.dimension())),
          is_normalized_(false) {}

    /**
     * @brief Évalue la forme en τ ∈ ℍ
     */
    complex_type<> operator()(const complex_type<>& tau) const {
        complex_type<> result = 0;
        for (size_t i = 0; i < space_.dimension(); ++i) {
            result += coefficients_[i] * space_.basis_element(i)(tau);
        }
        return result;
    }

    /**
     * @brief q-expansion de la forme
     */
    std::vector<complex_type<>>
    q_expansion(size_t precision) const {
        return space_.q_expansion(0, precision);  // À généraliser
    }

    /**
     * @brief Opérateurs de Hecke
     */
    ModularForm apply_hecke_operator(int_type<> n) const;

    /**
     * @brief Produit scalaire de Petersson (si défini)
     */
    // À implémenter: nécessite une mesure sur l'espace fondamental
};

/**
 * @brief Opérateur de Hecke T_n
 *
 * Représente l'opérateur de Hecke agissant sur les espaces de formes modulaires
 */
template<typename GammaType>
class HeckeOperator {
private:
    const ModularFormsSpace<GammaType>& space_;
    const int_type<> index_;  // n dans T_n

public:
    HeckeOperator(const ModularFormsSpace<GammaType>& space,
                  int_type<> n)
        : space_(space), index_(n) {
        if (n <= 0) {
            throw InvalidInputException("Hecke index must be positive");
        }
    }

    /**
     * @brief Applique l'opérateur à une forme modulaire
     */
    ModularForm<GammaType>
    operator()(const ModularForm<GammaType>& f) const {
        return f.apply_hecke_operator(index_);
    }

    /**
     * @brief Matrice de l'opérateur dans une base donnée
     */
    Eigen::MatrixXd matrix() const {
        return space_.hecke_operator(index_);
    }

    int_type<> index() const { return index_; }
};

/**
 * @brief Identifie les formes propres simultanées pour une famille d'opérateurs de Hecke
 */
template<typename GammaType>
class SimultaneousEigenforms {
private:
    const ModularFormsSpace<GammaType>& space_;
    std::set<int_type<>> hecke_indices_;  // Ensemble des T_n considérés

public:
    SimultaneousEigenforms(const ModularFormsSpace<GammaType>& space,
                           const std::vector<int_type<>>& indices)
        : space_(space), hecke_indices_(indices.begin(), indices.end()) {}

    /**
     * @brief Trouve toutes les formes propres simultanées
     * @return Vector de paires (forme, valeurs propres)
     */
    std::vector<std::pair<
        ModularForm<GammaType>,
        std::map<int_type<>, complex_type<>>
    >> find_eigenforms() const;
};

} // namespace modular
} // namespace langlands

#endif // LANGLANDS_MODULAR_MODULAR_FORM_HPP