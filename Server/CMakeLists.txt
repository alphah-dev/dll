add_executable(Server Server.cpp)

target_link_libraries(Server NetworkDLL)

if(WIN32)
    target_link_libraries(Server ws2_32)
endif()

set_target_properties(Server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(TARGET Server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:NetworkDLL>
    $<TARGET_FILE_DIR:Server>
)
