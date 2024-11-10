cmake_minimum_required(VERSION 3.26)
project(snake)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES
        gameModes/SinglePlayer.cpp
        gameModes/MultiPlayerLocal.cpp
        src/Game.cpp
        src/Snake.cpp
        consumables/Chicken.cpp
        consumables/Preworkout.cpp
        consumables/Creatine.cpp
        consumables/Steroids.cpp
        consumables/Genetics.cpp
        consumables/DragonBalls.cpp
)

file(COPY icons DESTINATION ${CMAKE_BINARY_DIR})

include_directories(${OpenCV_INCLUDE_DIRS})

add_library(snake_lib ${SOURCE_FILES})

target_link_libraries(snake_lib ${OpenCV_LIBS})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} snake_lib)

target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})

if(WIN32)
elseif(UNIX)
    find_package(X11 REQUIRED)
    include_directories(${X11_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})
endif()