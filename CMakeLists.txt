cmake_minimum_required(VERSION 3.1)

include("3rdparty/gate/cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.83.tar.gz"
    SHA1 "12dec078717539eb7b03e6d2a17797cba9be9ba9"
)

project(multihash LANGUAGES CXX)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

hunter_add_package(cryptopp)
find_package(cryptopp CONFIG REQUIRED)
hunter_add_package(Microsoft.GSL)
find_package(Microsoft.GSL CONFIG REQUIRED)

add_library(libmultihash
        multihash/multihash.cpp
        multihash/multihash.h
        multihash/algorithm.cpp
        multihash/algorithm.h
        multihash/detail/cryptopp_impl.cpp
        multihash/detail/cryptopp_impl.h
        multihash/digest.cpp
        multihash/digest.h
        multihash/multihash.cpp
        multihash/multihash.h
        multihash/static_function.cpp
        multihash/static_function.h
        multihash/function.cpp
        multihash/function.h)
set_target_properties(libmultihash PROPERTIES OUTPUT_NAME multihash)
target_link_libraries(libmultihash PUBLIC cryptopp-static)
target_link_libraries(libmultihash PUBLIC Microsoft.GSL::GSL)
target_include_directories(libmultihash PUBLIC "${multihash_SOURCE_DIR}")

enable_testing()
if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    target_compile_options(libmultihash PRIVATE -Wall -Werror -Wextra)
    # Binary depends on getopt which is unavailable in Windows
    add_executable(multihash multihash/main.cpp)
    target_link_libraries(multihash libmultihash)
    # Component test for whole executable
    add_test(
            NAME MultihashBinTest WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            COMMAND multihash --hash-type sha1 ${CMAKE_CURRENT_SOURCE_DIR}/test/foo
    )
    set_tests_properties(
            MultihashBinTest PROPERTIES PASS_REGULAR_EXPRESSION
            "1114f1d2d2f924e986ac86fdf7b36c94bcdf32beec15"
            TIMEOUT 1
    )
endif ()

# Compile and run all unit tests
hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)
add_executable( MultihashTest
                test/MultihashTest.cpp
                test/HashCodeTest.cpp
                test/main.cpp )
target_link_libraries(MultihashTest PUBLIC libmultihash PUBLIC GTest::main)
set_target_properties(MultihashTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}/test)
add_test(NAME MultihashTest
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test/MultihashTest)

install(TARGETS libmultihash
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY ipfs DESTINATION include
        FILES_MATCHING PATTERN "*.h"
        PATTERN "*Impl.h" EXCLUDE)

