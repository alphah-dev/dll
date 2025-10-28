add_executable(Client Client.cpp)

target_link_libraries(Client NetworkDLL)

if(WIN32)
    target_link_libraries(Client ws2_32)
endif()

set_target_properties(Client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(TARGET Client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:NetworkDLL>
    $<TARGET_FILE_DIR:Client>
)
