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

target_link_libraries(hasher PUBLIC
    CONAN_PKG::boost
    CONAN_PKG::nettle
)


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
    NAMESPACE nx_homework_
    FILE nx_homework_hasher-config.cmake
)
export(EXPORT hasher
    NAMESPACE nx_homework_
    FILE nx_homework_hasher-config.cmake
)
