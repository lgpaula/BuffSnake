cmake_minimum_required(VERSION 3.26)
project(snake)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(X11 REQUIRED)

set(SOURCE_FILES
        src/Map.cpp
        src/Game.cpp
        src/Snake.cpp
        consumables/Chicken.cpp
        consumables/Protein.cpp
        consumables/Creatine.cpp
        consumables/Steroids.cpp
        consumables/Genetics.cpp
        main.cpp)

file(COPY icons DESTINATION ${CMAKE_BINARY_DIR})

# Create a library target named 'snake_lib' from the source files
add_library(snake_lib ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${X11_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link the 'snake_lib' library to the 'snake' executable
target_link_libraries(${PROJECT_NAME} snake_lib ${OpenCV_LIBS} ${X11_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})