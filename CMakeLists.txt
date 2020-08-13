cmake_minimum_required(VERSION 3.15)
project(softchat CXX)

find_package(Qt5 REQUIRED Core Widgets WebSockets Sql)
set(CMAKE_AUTOMOC ON)

add_compile_definitions($<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Release>:RELEASE>)

set(client_name client)
file(GLOB_RECURSE client_srcs ${CMAKE_SOURCE_DIR}/client/*.cpp)
add_executable(${client_name} ${client_srcs})
target_include_directories(${client_name} PRIVATE ${CMAKE_SOURCE_DIR}/client ${CMAKE_SOURCE_DIR}/client/gui)
target_link_libraries(${client_name} PRIVATE Qt5::Widgets Qt5::WebSockets)
set_target_properties(${client_name} PROPERTIES LINK_FLAGS_RELEASE "/subsystem:windows /entry:mainCRTStartup")

set(server_name server)
aux_source_directory(${CMAKE_SOURCE_DIR}/server server_srcs)
add_executable(${server_name} ${server_srcs})
target_link_libraries(${server_name} PRIVATE Qt5::Core Qt5::WebSockets Qt5::Sql)