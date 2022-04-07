#
# **NOTE** This example currently requires the LibSerial be built using
# autotools as the CMake build of LibSerial does not install a pkg-config
# file yet. Support for pkg-config will be added to CMake build shortly.
#
cmake_minimum_required(VERSION 3.8)

project(rriv-cli LANGUAGES C CXX VERSION 1.0.0)

# If you've installed LibSerial in a non-standard location, please add it
# to PKG_CONFIG_PATH first. For example, if LibSerial is installed under
# /opt/libserial, set PKG_CONFIG_PATH environment variable to 
# /opt/libserial/lib/pkgconfig/ before running cmake for this example.
#
# export PKG_CONFIG_PATH=/opt/libserial/lib/pkgconfig/
#
find_package(PkgConfig)
pkg_check_modules(SERIAL libserial)

add_subdirectory(lib)

add_executable(rriv-cli rriv-cli.cpp)
target_include_directories(rriv-cli PRIVATE ${SERIAL_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/lib")
target_link_libraries(rriv-cli PRIVATE ${SERIAL_LDFLAGS} ${CMAKE_THREAD_LIBS_INIT} Cmd)
