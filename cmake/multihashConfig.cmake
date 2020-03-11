@PACKAGE_INIT@

include("${CMAKE_CURRENT_LIST_DIR}/multihashTargets.cmake")
find_package(cryptopp CONFIG REQUIRED)
check_required_components("@PROJECT_NAME@")