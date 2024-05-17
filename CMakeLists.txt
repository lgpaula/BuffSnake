cmake_minimum_required(VERSION 3.26)
project(snake)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        src/Border.cpp
        src/Snake.cpp
        src/Consumables.cpp)

# Create a library target named 'snake_lib' from the source files
add_library(snake_lib ${SOURCE_FILES})

add_executable(${PROJECT_NAME} main.cpp)

# Link the 'snake_lib' library to the 'snake' executable
target_link_libraries(snake snake_lib)