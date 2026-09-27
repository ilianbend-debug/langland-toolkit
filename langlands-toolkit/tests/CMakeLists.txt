# Tests configuration
if(BUILD_TESTS)
    # Find Catch2 for unit testing
    find_package(Catch2 3 QUIET)
    if(NOT Catch2_FOUND)
        message(STATUS "Catch2 not found, disabling tests")
        set(BUILD_TESTS OFF)
    else()
        # Enable testing
        enable_testing()

        # Include directories for tests
        include_directories(${CMAKE_SOURCE_DIR}/include)

        # Test: Number Theory
        add_executable(test_ntheory
            test_number_field.cpp
        )
        target_link_libraries(test_ntheory
            PRIVATE
                langlands
                Catch2::Catch2WithMain
        )
        add_test(NAME test_ntheory COMMAND test_ntheory)

        # Test: Modular Forms
        add_executable(test_modular
            test_modular_form.cpp
        )
        target_link_libraries(test_modular
            PRIVATE
                langlands
                Catch2::Catch2WithMain
        )
        add_test(NAME test_modular COMMAND test_modular)

        # Test: Core utilities
        add_executable(test_core
            test_core.cpp
        )
        target_link_libraries(test_core
            PRIVATE
                langlands
                Catch2::Catch2WithMain
        )
        add_test(NAME test_core COMMAND test_core)
    endif()
endif()