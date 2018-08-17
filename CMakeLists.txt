cmake_minimum_required(VERSION 2.8)

project(multihash LANGUAGES CXX)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

set(CMAKE_VERBOSE_MAKEFILE ON)
add_subdirectory(external/cryptopp)

include_directories(${CMAKE_BINARY_DIR}/include)
link_directories(${CMAKE_BINARY_DIR}/lib)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    if("${MSVC_RUNTIME}" STREQUAL "")
      set(MSVC_RUNTIME "static")
    endif()
	message(STATUS "Using MSVC_RUNTIME ${MSVC_RUNTIME}")
else()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")
endif()

SET(CRYPTOPP_LIBRARIES cryptopp)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	SET(CRYPTOPP_LIBRARIES cryptlib)
endif()

if(MSVC)
    # Default to statically-linked runtime.
    if("${MSVC_RUNTIME}" STREQUAL "")
      set(MSVC_RUNTIME "static")
    endif()
    # Set compiler options.
    set(variables
      CMAKE_C_FLAGS_DEBUG
      CMAKE_C_FLAGS_MINSIZEREL
      CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_RELWITHDEBINFO
      CMAKE_CXX_FLAGS_DEBUG
      CMAKE_CXX_FLAGS_MINSIZEREL
      CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_RELWITHDEBINFO
    )
    if(${MSVC_RUNTIME} STREQUAL "static")
      message(STATUS "MSVC -> forcing use of statically-linked runtime." )
      foreach(variable ${variables})
        if(${variable} MATCHES "/MD")
          string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
        endif()
      endforeach()
    else()
      message(STATUS "MSVC -> forcing use of dynamically-linked runtime." )
      foreach(variable ${variables})
        if(${variable} MATCHES "/MT")
          string(REGEX REPLACE "/MT" "/MD" ${variable} "${${variable}}")
        endif()
      endforeach()
    endif()
	foreach(variable ${variables})
	  message(STATUS "  '${variable}': ${${variable}}")
	endforeach()
endif()

message(STATUS "Initial build flags:")

add_library(libmultihash
        multihash/Algorithm.cpp multihash/Algorithm.h
        multihash/CryptoppImpl.cpp multihash/CryptoppImpl.h
        multihash/Hash.cpp multihash/Hash.h
        multihash/HashFunction.cpp multihash/HashFunction.h
        multihash/HashFunctionImpl.cpp multihash/HashFunctionImpl.h
        multihash/HashImpl.cpp multihash/HashImpl.h
        multihash/HashRawCodec.cpp multihash/HashRawCodec.h
        multihash/HashRawCodecImpl.cpp multihash/HashRawCodecImpl.h
        multihash/HashType.cpp multihash/HashType.h
        multihash/HashTypeImpl.cpp multihash/HashTypeImpl.h
        multihash/StringView.cpp multihash/StringView.h)
set_target_properties(libmultihash PROPERTIES OUTPUT_NAME multihash)
target_compile_features(libmultihash PUBLIC cxx_return_type_deduction)
target_include_directories(libmultihash PUBLIC "${multihash_SOURCE_DIR}")
add_dependencies(libmultihash libcryptopp)

if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
# Binary depends on getopt which is unavailable in Windows
add_executable(multihash multihash/main.cpp)
target_link_libraries(multihash libmultihash ${CRYPTOPP_LIBRARIES})
add_dependencies(multihash libcryptopp)

	# Component test for whole executable
	add_test (
		NAME MultihashBinTest WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            COMMAND multihash --hash-type sha1 ${CMAKE_CURRENT_SOURCE_DIR}/multihash/test/foo
	)
	set_tests_properties (
		MultihashBinTest PROPERTIES PASS_REGULAR_EXPRESSION
            "^1114f1d2d2f924e986ac86fdf7b36c94bcdf32beec15\ .*/multihash/test/foo"
		TIMEOUT 1
	)
endif()

install(TARGETS libmultihash
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY multihash DESTINATION include
        FILES_MATCHING PATTERN "*.h"
        PATTERN "*Impl.h" EXCLUDE)

# Compile and run all unit tests
enable_testing()
if (NOT TARGET gtest)
    add_subdirectory(external/googletest)
endif()
add_executable(MultihashTest multihash/test/MultihashTest.cpp multihash/test/main.cpp)
target_link_libraries(MultihashTest gtest libmultihash ${CRYPTOPP_LIBRARIES})
add_dependencies(MultihashTest libcryptopp)
set_target_properties(MultihashTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                      ${CMAKE_CURRENT_BINARY_DIR}/test)
add_test(NAME MultihashTest
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test/MultihashTest)
