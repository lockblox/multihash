cmake_minimum_required(VERSION 2.8)

project(cpp-multihash)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")
set(CMAKE_CXX_FLAGS_USAN "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
set(CMAKE_CXX_FLAGS_MSAN "${CMAKE_CXX_FLAGS} -fsanitize=memory")
set(CMAKE_CXX_FLAGS_ASAN "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_CXX_FLAGS_TSAN "${CMAKE_CXX_FLAGS} -fsanitize=thread")

include_directories(${cpp-multihash_SOURCE_DIR}/include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #create compilation database for clang
set(Boost_USE_STATIC_LIBS ON)

find_package (
    Boost REQUIRED COMPONENTS filesystem system unit_test_framework program_options 
)
include_directories (${Boost_INCLUDE_DIRS})

add_library(libmultihash STATIC src/MultihashImpl.cpp src/Type.cpp)
set_target_properties(libmultihash PROPERTIES OUTPUT_NAME multihash)

add_executable(multihash src/main.cpp)
target_link_libraries(multihash ${Boost_LIBRARIES} libmultihash crypto )

enable_testing()

string(COMPARE EQUAL "${CMAKE_SYSTEM_NAME}" "Linux" LINUX)
if(LINUX)
    # Test code formatting obeys convention
    add_test ( 
        NAME ClangFormat WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test
        COMMAND clang-format.sh
    )
    # Test code style obeys convention
    add_test ( 
        NAME ClangTidy WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test
        COMMAND clang-tidy.sh
    )
ENDIF(LINUX) 

# Component test for whole executable
add_test (
    NAME BinTest WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
    COMMAND multihash --hash-type sha1 ${CMAKE_CURRENT_SOURCE_DIR}/test/foo
)
set_tests_properties (
    BinTest PROPERTIES PASS_REGULAR_EXPRESSION
    "^1114f1d2d2f924e986ac86fdf7b36c94bcdf32beec15\ .*/test/foo"
    TIMEOUT 1
)
# Search OpenSSL
find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)

if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()

# Search OpenSSL not working for brew ( OSX )
if(APPLE)
    include_directories("/usr/local/Cellar/openssl/1.0.2/include")
endif()

# Compile and run all unit tests
file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)
foreach(testSrc ${TEST_SRCS})
    #Extract the filename without an extension (NAME_WE)
    get_filename_component(testName ${testSrc} NAME_WE)

    add_executable(${testName} ${testSrc})

    target_link_libraries(${testName} ${Boost_LIBRARIES} libmultihash crypto)

    set_target_properties(${testName} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/test)

    add_test(NAME ${testName}
             WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test
             COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/${testName} )
endforeach(testSrc)
