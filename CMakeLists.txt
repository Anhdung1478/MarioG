cmake_minimum_required(VERSION 3.16)

# Tên project và ngôn ngữ
project(MARIOG LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(SFML_ROOT "C:/SFML")
set(CMAKE_PREFIX_PATH "${SFML_ROOT}/lib/cmake/SFML" ${CMAKE_PREFIX_PATH})

set(SFML_STATIC_LIBRARIES ON)

find_package(SFML 3.0 REQUIRED COMPONENTS System Window Graphics Audio)

set(BOX2D_DIR "C:/BOX2D")
set(BOX2D_LIBRARY "C:/BOX2D/build/src/libbox2d.a")

set(EVENTPP_DIR "C:/eventpp")

# Thêm file nguồn
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
add_executable(MARIOG ${SOURCE_FILES})

# Set include directories after target creation
target_include_directories(MARIOG PRIVATE "${BOX2D_DIR}/include")
target_include_directories(MARIOG PRIVATE "${EVENTPP_DIR}/include")

# Link libraries
target_link_libraries(MARIOG PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)

# Link Box2D library
target_link_libraries(MARIOG PRIVATE ${BOX2D_LIBRARY})