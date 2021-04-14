cmake_minimum_required(VERSION 3.10)

project(multihash VERSION 1.0 LANGUAGES CXX)

option(BUILD_TESTING "Build unit tests" ON)

find_package(cryptopp CONFIG REQUIRED)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(HEADER_FILES
        multihash/algorithm.h;
        multihash/algorithm_identifier.h
        multihash/algorithm_factory.h
        multihash/multihash.h;
        multihash/function.h;
        multihash/code.h;)
add_library(libmultihash
        multihash/algorithm.cpp
        multihash/algorithm_identifier.cpp
        multihash/algorithm_factory.cpp
        multihash/detail/cryptopp_impl.h
        multihash/function.cpp
        multihash/code.cpp
        multihash/multihash.cpp
        ${HEADER_FILES})
target_include_directories(libmultihash PUBLIC
        $<BUILD_INTERFACE:${multihash_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>)
set_target_properties(libmultihash PROPERTIES OUTPUT_NAME multihash)
target_link_libraries(libmultihash PUBLIC cryptopp-static)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${multihash_BINARY_DIR}/multihashConfigVersion.cmake"
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion)

install(TARGETS libmultihash
        EXPORT multihashTargets
        INCLUDES DESTINATION include
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include/multihash)

include(CMakePackageConfigHelpers)
configure_package_config_file(
        "${multihash_SOURCE_DIR}/cmake/multihashConfig.cmake"
        "${multihash_BINARY_DIR}/multihashConfig.cmake"
        INSTALL_DESTINATION share/cmake/multihash
)

install(EXPORT multihashTargets DESTINATION share/cmake/multihash)
install(FILES "${multihash_BINARY_DIR}/multihashConfigVersion.cmake"
        "${multihash_BINARY_DIR}/multihashConfig.cmake"
        DESTINATION share/cmake/multihash)

if (MSVC)
    target_compile_options(libmultihash PRIVATE /W4 /WX /MP)
else ()
    if (BUILD_COVERAGE)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(COVERAGE_FLAGS "--coverage")
        endif ()
    endif ()
    target_compile_options(libmultihash PRIVATE -Wall -Wextra -pedantic -Werror
            ${COVERAGE_FLAGS})
endif ()
if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DNDEBUG)
endif ()


if (BUILD_TESTING)
    enable_testing()
    find_package(GTest MODULE REQUIRED)
    add_executable(multihash_test
            test/multihash_test.cpp
            test/hash_code_test.cpp
            test/main.cpp
            multihash/detail/cryptopp_impl.cpp)
    target_link_libraries(multihash_test PRIVATE libmultihash GTest::GTest GTest::Main)
    gtest_discover_tests(multihash_test)
    if (MSVC)
        target_compile_options(multihash_test PRIVATE /W4 /WX /MP)
    else ()
        target_compile_options(multihash_test PRIVATE -Wall -Wextra -pedantic
                -Werror ${COVERAGE_FLAGS})
        target_link_libraries(multihash_test PRIVATE ${COVERAGE_FLAGS})
    endif ()

    if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
        target_compile_options(libmultihash PRIVATE -Wall -Werror -pedantic -Wextra)
        # Binary depends on getopt which is unavailable in Windows
        add_executable(multihash multihash/main.cpp)
        target_link_libraries(multihash libmultihash)
        # Component test for whole executable
        add_test(
                NAME multihash_bin_test WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                COMMAND multihash --hash-type sha1 ${CMAKE_CURRENT_SOURCE_DIR}/test/foo
        )
        set_tests_properties(
                multihash_bin_test PROPERTIES PASS_REGULAR_EXPRESSION
                "1114f1d2d2f924e986ac86fdf7b36c94bcdf32beec15"
                TIMEOUT 1
        )
    endif()
endif()
