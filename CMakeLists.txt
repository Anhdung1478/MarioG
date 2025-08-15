cmake_minimum_required(VERSION 3.16)

# Tên project và ngôn ngữ
project(MARIOG LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Thêm file nguồn
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
add_executable(MARIOG ${SOURCE_FILES})

target_include_directories(MARIOG PRIVATE "C:/eventpp/include")

# Link libraries
target_link_libraries(MARIOG PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)
target_include_directories(MARIOG PRIVATE "${EVENTPP_DIR}/include")