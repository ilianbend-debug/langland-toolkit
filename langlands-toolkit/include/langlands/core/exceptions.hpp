#ifndef LANGLANDS_CORE_EXCEPTIONS_HPP
#define LANGLANDS_CORE_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>
#include <sstream>

namespace langlands {
namespace core {

/**
 * @brief Exception de base pour toutes les erreurs du toolkit
 */
class LanglandsException : public std::runtime_error {
public:
    explicit LanglandsException(const std::string& message)
        : std::runtime_error(message) {}
};

/**
 * @brief Exception pour les erreurs de précision arbitraire
 */
class PrecisionException : public LanglandsException {
public:
    explicit PrecisionException(const std::string& message)
        : LanglandsException("Precision error: " + message) {}
};

/**
 * @brief Exception pour les erreurs algébriques (corps, anneaux, etc.)
 */
class AlgebraicException : public LanglandsException {
public:
    explicit AlgebraicException(const std::string& message)
        : LanglandsException("Algebraic error: " + message) {}
};

/**
 * @brief Exception pour les erreurs d'analyse (séries, intégrales, etc.)
 */
class AnalysisException : public LanglandsException {
public:
    explicit AnalysisException(const std::string& message)
        : LanglandsException("Analysis error: " + message) {}
};

/**
 * @brief Exception pour les erreurs de convergence
 */
class ConvergenceException : public LanglandsException {
public:
    explicit ConvergenceException(const std::string& message)
        : LanglandsException("Convergence error: " + message) {}
};

/**
 * @brief Exception pour les entrées invalides
 */
class InvalidInputException : public LanglandsException {
public:
    explicit InvalidInputException(const std::string& message)
        : LanglandsException("Invalid input: " + message) {}
};

/**
 * @brief Exception pour les fonctionnalités non encore implémentées
 */
class NotImplementedException : public LanglandsException {
public:
    explicit NotImplementedException(const std::string& feature)
        : LanglandsException("Not implemented: " + feature) {}
};

} // namespace core
} // namespace langlands

#endif // LANGLANDS_CORE_EXCEPTIONS_HPP