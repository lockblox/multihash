cmake_minimum_required(VERSION 3.3)

project(cpp-multihash)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(${cpp-multihash_SOURCE_DIR}/include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #create compilation database for clang
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost REQUIRED COMPONENTS filesystem system unit_test_framework 
             program_options )
include_directories (${Boost_INCLUDE_DIRS})

add_library(libmultihash STATIC src/Multihash.cpp src/Type.cpp)
set_target_properties(libmultihash PROPERTIES OUTPUT_NAME multihash)

add_executable(multihash src/main.cpp)
target_link_libraries(multihash ${Boost_LIBRARIES} libmultihash crypto )

enable_testing()

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)

unset(Boost_LIBRARIES)
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost REQUIRED COMPONENTS filesystem unit_test_framework 
             program_options )
# Compile and run all tests
foreach(testSrc ${TEST_SRCS})
        #Extract the filename without an extension (NAME_WE)
        get_filename_component(testName ${testSrc} NAME_WE)

        add_executable(${testName} ${testSrc})

        target_link_libraries(${testName} ${Boost_LIBRARIES} libmultihash crypto)

        set_target_properties(${testName} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/bin)

        add_test(NAME ${testName}
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
                 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/${testName} )
endforeach(testSrc)
