cmake_minimum_required(VERSION 3.7)

project(beyondsprites)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)


set(COMMON_SRC
    "main.cpp"
    "engine.cpp"
    "font.cpp"
    "Mesh_Obj.cpp"
    "gameplay.cpp"
    "tga.cpp"
    )

include_directories(
    "."
    "${OPENGL_INCLUDE_DIR}"
    ${GLFW_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIR}
    )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

add_definitions("-DGL_GLEXT_PROTOTYPES")

add_executable(${CMAKE_PROJECT_NAME} ${COMMON_SRC})

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${GLFW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${OPENAL_LIBRARY}
    )

message("openal:${OPENAL_LIBRARIES}")
find_program(iwyu_path NAMES include-what-you-use iwyu)
if(iwyu_path)
    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY C_INCLUDE_WHAT_YOU_USE ${iwyu_path})
    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()
