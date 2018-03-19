cmake_minimum_required(VERSION 2.8)

project(shax LANGUAGES CXX)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")
endif()

SET(GTEST_LIBRARIES gtest)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
        SET(GTEST_LIBRARIES gtestd)
    endif()
endif()

add_library(libshax STATIC src/MultihashImpl.cpp src/Type.cpp)
set_target_properties(libshax PROPERTIES OUTPUT_NAME shax)
target_compile_features(libshax PUBLIC cxx_return_type_deduction)
target_include_directories(libshax PUBLIC "${shax_SOURCE_DIR}/include")

add_executable(shax src/main.cpp)
target_link_libraries(shax libshax crypto )

add_subdirectory(external/gtest)
include_directories(${CMAKE_BINARY_DIR}/include)
link_directories(${CMAKE_BINARY_DIR}/lib)

target_include_directories(libshax INTERFACE
                           "${shax_SOURCE_DIR}/include")

install(TARGETS libshax 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY shax DESTINATION include FILES_MATCHING 
        PATTERN "*.h")

enable_testing()

# Component test for whole executable
add_test (
    NAME TestMultihashBin WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMAND shax --hash-type sha1 ${CMAKE_CURRENT_SOURCE_DIR}/test/foo
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

target_link_libraries(TestMultihash gtest libshax crypto)
add_dependencies(TestMultihash googletest)

set_target_properties(TestMultihash PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                      ${CMAKE_CURRENT_BINARY_DIR}/test)

add_test(NAME TestMultihash
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test/TestMultihash)
