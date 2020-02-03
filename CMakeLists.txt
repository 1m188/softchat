cmake_minimum_required(VERSION 3.15)
project(softchat)

find_package(Qt5 REQUIRED Core Widgets WebSockets Sql)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_compile_definitions($<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Release>:RELEASE>)

set(client_name client)
aux_source_directory(./client client_srcs)
add_executable(${client_name} ${client_srcs})
target_include_directories(${client_name} PRIVATE ./client ./client/gui)
file(GLOB gui_srcs ./client/gui/*.cpp)
target_sources(${client_name} PRIVATE ./client/client.qrc ${gui_srcs})
target_link_libraries(${client_name} PRIVATE Qt5::Widgets Qt5::WebSockets)

set(server_name server)
aux_source_directory(./server server_srcs)
add_executable(${server_name} ${server_srcs})
target_sources(${server_name} PRIVATE ./server/server.qrc)
target_link_libraries(${server_name} PRIVATE Qt5::Core Qt5::WebSockets Qt5::Sql)

if(WIN32)
    set_target_properties(${client_name} PROPERTIES LINK_FLAGS_RELEASE "/subsystem:windows /entry:mainCRTStartup")
endif()