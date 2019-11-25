cmake_minimum_required(VERSION 3.11)

include(GNUInstallDirs)

include("${CMAKE_CURRENT_BINARY_DIR}/conan_paths.cmake")

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS VERSION)
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" PROJECT_VERSION)
project(nx_homework_hasher VERSION ${PROJECT_VERSION})


add_library(hasher)

set_target_properties(hasher PROPERTIES
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME nx_homework_hasher
)
if(BUILD_SHARED_LIBS)
    set_target_properties(hasher PROPERTIES
        C_VISIBILITY_PRESET hidden
    )
    target_compile_definitions(hasher
        PRIVATE NX_HOMEWORK_HASHER_EXPORT
        INTERFACE NX_HOMEWORK_HASHER_IMPORT
    )
endif()

target_include_directories(hasher PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_sources(hasher PRIVATE
    src/hasher.cpp
)

find_package(boost REQUIRED)
target_link_libraries(hasher PUBLIC boost::boost)

find_package(nettle REQUIRED)
target_link_libraries(hasher PUBLIC nettle::nettle)


install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
target_include_directories(hasher PUBLIC
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
install(TARGETS hasher EXPORT hasher
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
