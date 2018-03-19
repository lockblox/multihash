cmake_minimum_required(VERSION 2.8)
project(googletest LANGUAGES CXX)
include(ExternalProject)

ExternalProject_Add (
    googletest
    GIT_REPOSITORY    https://github.com/google/googletest.git
    #GIT_REPOSITORY    git@localhost:campx/googletest.git
    GIT_TAG           master
    INSTALL_DIR       ${CMAKE_BINARY_DIR}
    CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> 
                      -Dgtest_force_shared_crt=ON
                      -Dgtest_disable_pthreads=ON
    PREFIX            ${CMAKE_CURRENT_BINARY_DIR}
    UPDATE_COMMAND    ""
)
