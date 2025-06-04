cmake_minimum_required(VERSION 3.28)

# Tên project và ngôn ngữ
project(MARIOG LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Cài đặt chuẩn C++ (ví dụ C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Thêm file nguồn
add_executable(MARIOG src/main_app/main.cpp src/main_app/main-window.cpp src/main_app/pages/main-menu.cpp)

target_link_libraries(MARIOG PRIVATE SFML::Graphics)