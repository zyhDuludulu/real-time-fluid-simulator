cmake_minimum_required(VERSION 3.10)

# set the project name
project(real-time-fluid-simulator)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCE "src/*.cpp")
file(GLOB CUDA "src/*.cu")
file(GLOB GLAD "lib/glad/src/glad.c")

# add the executable
add_executable(${PROJECT_NAME} ${SOURCE} ${CUDA} ${GLAD})

# glad
include_directories(${CMAKE_SOURCE_DIR}/lib/glad/include)

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

