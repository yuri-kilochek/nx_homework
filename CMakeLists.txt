cmake_minimum_required(VERSION 3.11)

include(GNUInstallDirs)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS VERSION)
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" PROJECT_VERSION)
project(nx_homework_hasher VERSION ${PROJECT_VERSION})


add_library(hasher SHARED)

set_target_properties(hasher PROPERTIES
    OUTPUT_NAME nx_homework_hasher
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_VISIBILITY_PRESET hidden
)

target_compile_definitions(hasher
    PRIVATE NX_HOMEWORK_HASHER_EXPORT
    INTERFACE NX_HOMEWORK_HASHER_IMPORT
)

target_include_directories(hasher PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_sources(hasher PRIVATE
    src/hasher.cpp
)

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup(TARGETS)
target_link_libraries(hasher PUBLIC
    CONAN_PKG::boost
    CONAN_PKG::nettle
)


add_subdirectory(microservice)


install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
target_include_directories(hasher PUBLIC
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
install(TARGETS hasher hasher_microservice EXPORT hasher
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
install(EXPORT hasher
    DESTINATION "${CMAKE_INSTALL_DATADIR}/"
    NAMESPACE nx_homework_hasher::
    FILE nx_homework_hasher-config.cmake
)
export(EXPORT hasher
    NAMESPACE nx_homework_hasher::
    FILE nx_homework_hasher-config.cmake
)
