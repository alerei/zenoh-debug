project(external)

set(CMAKE_ARGS -B build/ -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/out -DCMAKE_CXX_STANDARD=17 -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_BUILD_TYPE=Release)

# define source directories
set(ZENOH_C_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zenoh-c)

# Zenoh-C
add_custom_target(
    external.zenoh-c.build
    WORKING_DIRECTORY ${ZENOH_C_DIR}
    COMMAND ${CMAKE_COMMAND} ${CMAKE_ARGS}
    COMMAND ${CMAKE_COMMAND} --build build/
    COMMAND ${CMAKE_COMMAND} --install build/
)

# zenoh-c
add_library(external.zenoh-c SHARED IMPORTED GLOBAL)
set_property(TARGET external.zenoh-c PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/out/lib/libzenohc.so)
set_property(TARGET external.zenoh-c PROPERTY IMPORTED_NO_SONAME TRUE)
set_property(TARGET external.zenoh-c PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/out/include)
