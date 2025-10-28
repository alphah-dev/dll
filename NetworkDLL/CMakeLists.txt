add_library(NetworkDLL SHARED
    Network.cpp
    Network.h
)

target_compile_definitions(NetworkDLL PRIVATE NETWORK_EXPORTS)

if(WIN32)
    target_link_libraries(NetworkDLL ws2_32)
endif()

set_target_properties(NetworkDLL PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
