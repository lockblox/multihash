cmake_minimum_required(VERSION 2.8)
project(libcryptopp LANGUAGES CXX)
include(ExternalProject)
include(ProcessorCount)

set(CRYPTOPP_PROJECT libcryptopp)
set(CRYPTOPP_GIT_REPO https://github.com/weidai11/cryptopp.git)
set(CRYPTOPP_GIT_TAG master)
set(CRYPTOPP_BUILD_IN_SOURCE true)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	if (${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
		set (CMAKE_BUILD_TYPE Release)
	endif()
	string(FIND ${CMAKE_BINARY_DIR} ${CMAKE_BUILD_TYPE} BUILD_TYPE_POS)
	math(EXPR BUILD_TYPE_POS ${BUILD_TYPE_POS}-4)
	string(SUBSTRING ${CMAKE_BINARY_DIR} ${BUILD_TYPE_POS} 3 PLATFORM)
	if (NOT ${PLATFORM} STREQUAL x64)
		set(PLATFORM Win32)
	endif()
	message(STATUS ${PLATFORM})
	message(STATUS ${CMAKE_BINARY_DIR})
	ExternalProject_Add (
		${CRYPTOPP_PROJECT}
		GIT_REPOSITORY    ${CRYPTOPP_GIT_REPO} 
		GIT_TAG           ${CRYPTOPP_GIT_TAG}
		INSTALL_DIR       ${CMAKE_BINARY_DIR}
		CONFIGURE_COMMAND ""
		BUILD_IN_SOURCE   ${CRYPTOPP_BUILD_IN_SOURCE}
		BUILD_COMMAND     msbuild /p:Configuration=${CMAKE_BUILD_TYPE} /p:Platform=${PLATFORM} cryptlib.vcxproj
		INSTALL_COMMAND   xcopy /y /i ${PLATFORM}\\Output\\${CMAKE_BUILD_TYPE}\\* ..\\..\\..\\..\\lib 
						  && xcopy /y /i *.h ..\\..\\..\\..\\include\\cryptopp
		PREFIX            ${CMAKE_CURRENT_BINARY_DIR}
		UPDATE_COMMAND    ""
	)
	# need to determine target architecture from cmake/env and pass to msbuild
else()
	ExternalProject_Add (
		${CRYPTOPP_PROJECT}
		GIT_REPOSITORY    ${CRYPTOPP_GIT_REPO} 
		GIT_TAG           ${CRYPTOPP_GIT_TAG}
		INSTALL_DIR       ${CMAKE_BINARY_DIR}
		CONFIGURE_COMMAND ""
		BUILD_IN_SOURCE   ${CRYPTOPP_BUILD_IN_SOURCE}
		INSTALL_COMMAND   ${CMAKE_MAKE_PROGRAM} install PREFIX=<INSTALL_DIR>
		PREFIX            ${CMAKE_CURRENT_BINARY_DIR}
		UPDATE_COMMAND    ""
	)
endif()
