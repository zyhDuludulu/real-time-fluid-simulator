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

# if you have any libraries, link them here
# target_link_libraries(${PROJECT_NAME} your_library)

## glm
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glm)
target_link_libraries(${PROJECT_NAME} glm)

