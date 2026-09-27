cmake_minimum_required(VERSION 3.18)
project(LanglandsExplorerToolkit VERSION 1.0.0 LANGUAGES CXX)

# C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options de compilation
option(BUILD_EXAMPLES "Build example programs" ON)
option(BUILD_TESTS "Build unit tests" ON)
option(ENABLE_VISUALIZATION "Enable visualization modules" ON)
option(USE_SYSTEM_PARI "Use system PARI/GP library if available" OFF)

# Dépendances requises
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Boost 1.75 COMPONENTS system filesystem program_options thread chrono regex REQUIRED)

# Optionnel: PARI/GP pour vérification croisée
if(USE_SYSTEM_PARI)
    find_package(PARI)
    if(PARI_FOUND)
        list(APPEND EXTRA_LIBS ${PARI_LIBRARIES})
        list(APPEND EXTRA_INCLUDES ${PARI_INCLUDE_DIRS})
        add_definitions(-DUSE_PARI)
    endif()
endif()

# Configuration de sortie
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Inclure les répertoires d'en-tête
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${Eigen3_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${EXTRA_INCLUDES}
)

# Bibliothèques principales
add_library(ntheory STATIC
    src/ntheory/number_field.cpp
    src/ntheory/class_group.cpp
    src/ntheory/unit_group.cpp
    src/ntheory/hilbert_symbol.cpp
    src/ntheory/ideal_arithmetic.cpp
)

add_library(modular STATIC
    src/modular/modular_form.cpp
    src/modular/hecke_operator.cpp
    src/modular/modular_symbols.cpp
    src/modular/eichler_shimura.cpp
    src/modular/space_gamma0.cpp
)

add_library(automorphic STATIC
    src/automorphic/adeles.cpp
    src/automorphic/local_representation.cpp
    src/automorphic/global_representation.cpp
    src/automorphic/hecke_algebra.cpp
)

add_library(lfunctions STATIC
    src/lfunctions/dirichlet_l.cpp
    src/lfunctions/modular_form_l.cpp
    src/lfunctions/galois_representation_l.cpp
    src/lfunctions/functional_equation.cpp
    src/lfunctions/derivatives.cpp
)

add_library(galois STATIC
    src/galois/padic_representation.cpp
    src/galois/galois_cohomology.cpp
    src/galois/local_global_compatibility.cpp
)

# Bibliothèque principale
add_library(langlands-core STATIC
    src/core/exceptions.cpp
    src/core/precision.cpp
    src/core/algebra.cpp
    src/core/analysis.cpp
)

# Linker les dépendances
target_link_libraries(ntheory
    PRIVATE
        Eigen3::Eigen
        Boost::system
        Boost::filesystem
)

target_link_libraries(modular
    PRIVATE
        Eigen3::Eigen
        Boost::system
        ntheory
)

target_link_libraries(automorphic
    PRIVATE
        Boost::thread
        modular
        ntheory
)

target_link_libraries(lfunctions
    PRIVATE
        Eigen3::Eigen
        Boost::system
        Boost::thread
        automorphic
        modular
        ntheory
)

target_link_libraries(galois
    PRIVATE
        ntheory
        lfunctions
)

target_link_libraries(langlands-core
    PRIVATE
        Boost::system
        Boost::chrono
)

# Bibliothèque finale
add_library(langlands
    STATIC
)
target_link_libraries(langlands
    langlands-core
    ntheory
    modular
    automorphic
    lfunctions
    galois
    ${EXTRA_LIBS}
)

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Visualization (optionnel)
if(ENABLE_VISUALIZATION)
    find_package(Qwt 6.1 QUIET)
    if(Qwt_FOUND)
        add_subdirectory(visualization)
        list(APPEND EXTRA_LIBS ${QWT_LIBRARIES})
        list(APPEND EXTRA_INCLUDES ${Qwt_INCLUDE_DIRS})
        add_definitions(-DENABLE_VISUALIZATION)
    else()
        message(WARNING "Qwt not found, disabling visualization")
    endif()
endif()

# Installation
install(TARGETS langlands
        ARCHIVE DESTINATION lib
        )
install(DIRECTORY include/ DESTINATION include/langlands)
install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION .)
install(FILES ${PROJECT_SOURCE_DIR}/README.md DESTINATION .)

# Configuration pour VS Code
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)