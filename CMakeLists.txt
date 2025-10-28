cmake_minimum_required(VERSION 3.10)
project(ClientServerDLL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(NetworkDLL)
add_subdirectory(Server)
add_subdirectory(Client)
