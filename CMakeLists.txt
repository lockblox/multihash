cmake_minimum_required(VERSION 3.1)

project(multihash VERSION 1.0 LANGUAGES CXX)
include(CTest)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

find_package(cryptopp CONFIG REQUIRED)

set(HEADER_FILES 
    multihash/algorithm.h;
    multihash/digest.h;
    multihash/multihash.h;
    multihash/function.h;
    multihash/static_function.h)
add_library(libmultihash
            multihash/algorithm.cpp
            multihash/detail/cryptopp_impl.h
            multihash/digest.cpp
            multihash/function.cpp
            multihash/code.cpp 
            multihash/code.h
            ${HEADER_FILES})
set_target_properties(libmultihash PROPERTIES OUTPUT_NAME multihash)
target_include_directories(libmultihash PUBLIC 
                           $<BUILD_INTERFACE:${multihash_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:include>)
target_link_libraries(libmultihash PUBLIC cryptopp-static)
if (MSVC) 
    target_compile_options(libmultihash PRIVATE /W4 /WX /MP)
else() 
    target_compile_options(libmultihash PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DNDEBUG)
endif()

if(BUILD_TESTING)
    find_package(GTest MODULE REQUIRED)
    add_executable(MultihashTest 
                   test/MultihashTest.cpp 
                   test/HashCodeTest.cpp 
                   test/main.cpp 
                   multihash/static_function.cpp
                   multihash/detail/cryptopp_impl.cpp
                   multihash/multihash.cpp)
    target_link_libraries(MultihashTest PRIVATE libmultihash GTest::GTest GTest::Main)
    add_test(NAME GTest COMMAND MultihashTest)
    if (MSVC) 
        target_compile_options(MultihashTest PRIVATE /W4 /WX /MP)
    else() 
        target_compile_options(MultihashTest PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()

    if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
        target_compile_options(libmultihash PRIVATE -Wall -Werror -pedantic -Wextra)
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
    endif()
endif()

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

