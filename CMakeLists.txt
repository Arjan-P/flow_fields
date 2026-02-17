cmake_minimum_required(VERSION 3.16)
project(flow_fields LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add GLAD manually
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Find GLFW
find_package(glfw3 REQUIRED)

# Create executable
add_executable(app src/main.cpp src/shader.cpp src/compute.cpp)

# Link everything
target_link_libraries(app PRIVATE glad glfw)
