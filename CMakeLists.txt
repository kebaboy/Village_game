cmake_minimum_required(VERSION 3.26)
project(Village)

set(CMAKE_CXX_STANDARD 17)

add_executable(Village main.cpp
        Game.cpp
        ResourceManager.cpp
        GameObject.cpp
        UI.cpp
        UIElement.cpp
        Map.cpp
        Menu.cpp
        Resources.cpp
)

find_package(raylib 4.2.0 REQUIRED)
target_link_libraries(Village PRIVATE raylib)
