cmake_minimum_required(VERSION 3.11)

include(GNUInstallDirs)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS VERSION)
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" PROJECT_VERSION)
project(nx_homework_hasher VERSION ${PROJECT_VERSION})

include(CTest)

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup(TARGETS)

add_subdirectory(hasher)
add_subdirectory(hasher_microservice)

