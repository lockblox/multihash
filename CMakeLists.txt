cmake_minimum_required(VERSION 2.8)

project(hashix LANGUAGES CXX)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -Werror")

include_directories(${hashix_SOURCE_DIR}/include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #create compilation database for clang

add_library(libhashix STATIC src/MultihashImpl.cpp src/Type.cpp)
set_target_properties(libhashix PROPERTIES OUTPUT_NAME hashix)

add_executable(hashix src/main.cpp)
target_link_libraries(hashix libhashix crypto )

add_subdirectory(external/gtest)

target_include_directories(libhashix INTERFACE
                           "${hashix_SOURCE_DIR}/include")

enable_testing()

# Component test for whole executable
add_test (
    NAME TestMultihashBin WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMAND hashix --hash-type sha1 ${CMAKE_CURRENT_SOURCE_DIR}/test/foo
)
set_tests_properties (
    TestMultihashBin PROPERTIES PASS_REGULAR_EXPRESSION
    "^1114f1d2d2f924e986ac86fdf7b36c94bcdf32beec15\ .*/test/foo"
    TIMEOUT 1
)
# Search OpenSSL not working for brew ( OSX )
if(APPLE)
    link_directories("/usr/local/opt/openssl/lib")
    include_directories("/usr/local/opt/openssl/include")
else()
    # Search OpenSSL
    find_package(PkgConfig REQUIRED)
    pkg_search_module(OPENSSL REQUIRED openssl)
    if( OPENSSL_FOUND )
        include_directories(${OPENSSL_INCLUDE_DIRS})
        message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
    endif()
endif()

# Compile and run all unit tests
add_executable(TestMultihash test/TestMultihash.cpp test/main.cpp)

target_link_libraries(TestMultihash gtest libhashix crypto)

set_target_properties(TestMultihash PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                      ${CMAKE_CURRENT_BINARY_DIR}/test)

add_test(NAME TestMultihash
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test/TestMultihash)
