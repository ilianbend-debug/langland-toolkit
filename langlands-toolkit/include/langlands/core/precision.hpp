#ifndef LANGLANDS_CORE_PRECISION_HPP
#define LANGLANDS_CORE_PRECISION_HPP

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/gmp.hpp>
#include <stdexcept>
#include <limits>
#include <iostream>
#include <iomanip>
#include <langlands/core/exceptions.hpp>

namespace langlands {
namespace core {

/**
 * @brief Types de précision arbitraire configurables
 */
namespace precision {

// Précision par défaut (peut être overridée par les templates)
static constexpr std::size_t default_digits10 = 100;
static constexpr std::size_t max_digits10 = 10000;

/**
 * @brief Nombre à virgule flottante de précision arbitraire
 * Utilise Boost.Multiprecision avec backends sélectionnables
 */
template<std::size_t Digits10 = default_digits10>
using float_type =
    typename boost::multiprecision::number<
        boost::multiprecision::cpp_dec_float<Digits10>
    >::type;

/**
 * @brief Entier de précision arbitraire
 */
template<std::size_t Digits10 = default_digits10>
using int_type =
    typename boost::multiprecision::number<
        boost::multiprecision::cpp_int,
        boost::multiprecision::et_off
    >::type;

/**
 * @brief Nombre rationnel de précision arbitraire
 */
template<std::size_t Digits10 = default_digits10>
using rational_type =
    typename boost::multiprecision::number<
        boost::multiprecision::cpp_rational<
            boost::multiprecision::cpp_int
        >
    >::type;

/**
 * @brief Nombre complexe de précision arbitraire
 */
template<std::size_t Digits10 = default_digits10>
using complex_type =
    std::complex<float_type<Digits10>>;

/**
 * @brief Classe de gestion de précision globale
 */
class PrecisionManager {
private:
    static std::size_t current_digits10_;
    static bool initialized_;

public:
    /**
     * @brief Initialise le gestionnaire de précision
     * @param digits10 Nombre de chiffres décimaux de précision
     */
    static void initialize(std::size_t digits10 = default_digits10) {
        if (digits10 > max_digits10) {
            throw PrecisionException("Requested precision too high: " +
                                   std::to_string(digits10) +
                                   " > " + std::to_string(max_digits10));
        }
        current_digits10_ = digits10;
        initialized_ = true;

        // Configuration de std::cout pour la précision
        std::cout << std::setprecision(digits10);
    }

    /**
     * @brief Obtient la précision actuelle
     * @return Nombre de chiffres décimaux de précision
     */
    static std::size_t get_current_precision() {
        if (!initialized_) {
            initialize();  // Initialisation par défaut
        }
        return current_digits10_;
    }

    /**
     * @brief Définit une nouvelle précision
     * @param digits10 Nouveau nombre de chiffres décimaux
     */
    static void set_precision(std::size_t digits10) {
        initialize(digits10);
    }
};

// Initialisation des variables statiques
std::size_t PrecisionManager::current_digits10_ = default_digits10;
bool PrecisionManager::initialized_ = false;

/**
 * @brief Littéraux définis pour faciliter l'utilisation
 */
namespace literals {

// Littéral pour float_type
constexpr float_type<0> operator"" _ldq(long double val) {
    return float_type<0>(val);
}

// Littéral pour int_type
constexpr int_type<0> operator"" _idq(unsigned long long val) {
    return int_type<0>(val);
}

} // namespace literals

} // namespace precision
} // namespace core
} // namespace langlands

#endif // LANGLANDS_CORE_PRECISION_HPP