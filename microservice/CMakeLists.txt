add_executable(hasher_microservice)

set_target_properties(hasher_microservice PROPERTIES
    OUTPUT_NAME nx_homework_hasher_microservice
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
)

target_include_directories(hasher_microservice PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_sources(hasher_microservice PRIVATE
    src/server.cpp
    src/main.cpp
)

target_link_libraries(hasher_microservice PRIVATE
    hasher
    CONAN_PKG::boost
)
