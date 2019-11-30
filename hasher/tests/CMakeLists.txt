include(Catch)

add_executable(hasher_tests)

set_target_properties(hasher_tests PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

target_link_libraries(hasher_tests PRIVATE
    CONAN_PKG::catch2
    hasher
)

target_sources(hasher_tests PRIVATE
    main.cpp
    basic.cpp
)

catch_discover_tests(hasher_tests)
