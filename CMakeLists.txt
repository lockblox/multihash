cmake_minimum_required(VERSION 2.8)

project(shax LANGUAGES CXX)
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

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    SET(GTEST_LIBRARIES gtestd)
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

add_library(libshax
            shax/Algorithm.cpp          shax/Algorithm.h
			shax/CryptoppImpl.cpp		 shax/CryptoppImpl.h
            shax/Hash.cpp               shax/Hash.h
            shax/HashFunction.cpp       shax/HashFunction.h
            shax/HashFunctionImpl.cpp   shax/HashFunctionImpl.h
            shax/HashImpl.cpp           shax/HashImpl.h
            shax/HashRawCodec.cpp       shax/HashRawCodec.h
            shax/HashRawCodecImpl.cpp   shax/HashRawCodecImpl.h
            shax/HashType.cpp           shax/HashType.h
            shax/HashTypeImpl.cpp       shax/HashTypeImpl.h
			shax/StringView.cpp			 shax/StringView.h)
set_target_properties(libshax PROPERTIES OUTPUT_NAME shax)
target_compile_features(libshax PUBLIC cxx_return_type_deduction)
target_include_directories(libshax PUBLIC "${shax_SOURCE_DIR}")
add_dependencies(libshax libcryptopp)

if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
# Binary depends on getopt which is unavailable in Windows
	add_executable(shax shax/main.cpp)
    target_link_libraries(shax libshax ${CRYPTOPP_LIBRARIES})
    add_dependencies(shax libcryptopp)

	# Component test for whole executable
	add_test (
		NAME TestMultihashBin WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND shax --hash-type sha1 ${CMAKE_CURRENT_SOURCE_DIR}/shax/test/foo
	)
	set_tests_properties (
		TestMultihashBin PROPERTIES PASS_REGULAR_EXPRESSION
		"^1114f1d2d2f924e986ac86fdf7b36c94bcdf32beec15\ .*/shax/test/foo"
		TIMEOUT 1
	)
endif()

install(TARGETS libshax 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY shax DESTINATION include 
        FILES_MATCHING PATTERN "*.h"
        PATTERN "*Impl.h" EXCLUDE)

enable_testing()

# Compile and run all unit tests
add_executable(TestMultihash shax/test/TestMultihash.cpp shax/test/main.cpp)
add_subdirectory(external/gtest)
target_link_libraries(TestMultihash ${GTEST_LIBRARIES} libshax
                      ${CRYPTOPP_LIBRARIES})
                      
add_dependencies(TestMultihash googletest libcryptopp)
set_target_properties(TestMultihash PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                      ${CMAKE_CURRENT_BINARY_DIR}/test)
add_test(NAME TestMultihash
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test/TestMultihash)
