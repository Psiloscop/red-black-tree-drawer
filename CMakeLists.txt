cmake_minimum_required(VERSION 3.7)
add_compile_options(-std=c++11)

set(PROJECT_NAME RedBlackTree)
project(${PROJECT_NAME})

# Include all project files
include_directories(include)
add_executable(${PROJECT_NAME} src/main.cpp include/RBTree.h include/RBNode.h include/Drawer.h include/UIConsole.h)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules" ${CMAKE_MODULE_PATH})
# Find any version 2.X of SFML
# See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
else()
    # Linking SFML statically
    set(SFML_STATIC_LIBRARIES TRUE)

    # In most cases better set in the CMake cache
#    set(SFML_DIR "./libs/SFML-2.5.1/lib/cmake/SFML")
    set(SFML_DIR "./libs/SFML-2.5.1-32bit/lib/cmake/SFML")

    target_link_libraries(${PROJECT_NAME} sfml-graphics)
endif()