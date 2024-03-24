cmake_minimum_required(VERSION 3.10)

# set the project name
project(real-time-fluid-simulator)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCE "src/*.cpp")

# add the executable
add_executable(${PROJECT_NAME} ${SOURCE})

# glfw
option(GLFW_BUILD_TESTS off)
option(GLFW_BUILD_EXAMPLES off)
option(GLFW_BUILD_DOCS off)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glfw)
include_directories(${CMAKE_SOURCE_DIR}/lib/glfw/include)
target_link_libraries(${PROJECT_NAME} glfw ${glfw_LIBRARIES})

# glew
option(BUILD_UTILS off)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glew/build/cmake)
include_directories(${CMAKE_SOURCE_DIR}/lib/glew/include)
target_link_libraries(${PROJECT_NAME} glew_s)

## glm
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glm)
target_link_libraries(${PROJECT_NAME} glm)

