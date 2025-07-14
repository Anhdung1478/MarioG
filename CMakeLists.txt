cmake_minimum_required(VERSION 3.5)

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

include(FetchContent)
FetchContent_Declare(
    eventpp
    GIT_REPOSITORY https://github.com/wqking/eventpp
    GIT_TAG        d2db8af2a46c79f8dc75759019fba487948e9442 # v0.1.3
)
FetchContent_MakeAvailable(eventpp)

FetchContent_Declare(
	box2d
	GIT_REPOSITORY https://github.com/erincatto/box2d.git
	GIT_TAG main
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(box2d)

# Cài đặt chuẩn C++ (ví dụ C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Thêm file nguồn
add_executable(MARIOG src/main_app/main.cpp src/main_app/main-window.cpp src/main_app/pages/main-menu.cpp src/main_app/pages/levels.cpp src/widget_toolkit/resource/TextureManager.cpp src/widget_toolkit/sound/SoundManager.cpp)

# target_link_directories(MARIOG PRIVATE ${CMAKE_SOURCE_DIR}/extern/include/SFML/ ${CMAKE_SOURCE_DIR}/extern/include/BOX2D/ ${CMAKE_SOURCE_DIR}/extern/include/)
target_link_libraries(MARIOG PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio box2d eventpp::eventpp)
