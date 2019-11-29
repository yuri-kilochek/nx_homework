add_executable(hasher_microservice)

set_target_properties(hasher_microservice PROPERTIES
    OUTPUT_NAME nx_homework_hasher_microservice
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
)

target_compile_definitions(hasher_microservice PRIVATE
    "NX_HOMEWORK_HASHER_MICROSERVICE_VERSION_STRING=\"${PROJECT_VERSION}\""
)

target_include_directories(hasher_microservice PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_sources(hasher_microservice PRIVATE
    src/session.cpp
    src/server.cpp
    src/options.cpp
    src/main.cpp
)

target_link_libraries(hasher_microservice PRIVATE
    hasher
    CONAN_PKG::boost
)


install(TARGETS hasher hasher_microservice EXPORT hasher_microservice
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
install(EXPORT hasher_microservice
    DESTINATION "${CMAKE_INSTALL_DATADIR}/"
    NAMESPACE nx_homework_
    FILE nx_homework_hasher_microservice-config.cmake
)
export(EXPORT hasher
    NAMESPACE nx_homework_hasher_
    FILE nx_homework_hasher_microservice-config.cmake
)
