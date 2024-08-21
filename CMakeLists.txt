cmake_minimum_required(VERSION 3.26)
project(snake)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(X11 REQUIRED)
find_package(Boost 1.66 REQUIRED COMPONENTS system)

#if(NOT Boost_FOUND)
#    message(STATUS "Boost not found, attempting to install it.")
#    include(ExternalProject)
#    ExternalProject_Add(boost_external
#            URL https://boostorg.jfrog.io/artifactory/main/release/1.76.0/source/boost_1_76_0.tar.gz
#            PREFIX ${CMAKE_BINARY_DIR}/boost
#            CONFIGURE_COMMAND ./bootstrap.sh
#            BUILD_COMMAND ./b2 install
#            BUILD_IN_SOURCE 1
#            INSTALL_DIR ${CMAKE_BINARY_DIR}/boost_install
#    )
#    set(BOOST_ROOT ${CMAKE_BINARY_DIR}/boost_install)
#    find_package(Boost 1.66 REQUIRED COMPONENTS system PATHS ${BOOST_ROOT} NO_DEFAULT_PATH)
#    include_directories(${BOOST_ROOT}/include)
#    link_directories(${BOOST_ROOT}/lib)
#    set(BOOST_EXTERNAL_TARGET boost_external)
#else()
#    set(BOOST_EXTERNAL_TARGET "")
#endif()

set(SOURCE_FILES
        gameModes/SinglePlayer.cpp
        gameModes/MultiPlayerLocal.cpp
        gameModes/MultiPlayerLAN.cpp
        src/Game.cpp
        src/Snake.cpp
        consumables/Chicken.cpp
        consumables/Preworkout.cpp
        consumables/Creatine.cpp
        consumables/Steroids.cpp
        consumables/Genetics.cpp
        consumables/DragonBalls.cpp
        main.cpp)

file(COPY icons DESTINATION ${CMAKE_BINARY_DIR})

# Create a library target named 'snake_lib' from the source files
add_library(snake_lib ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${X11_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link the 'snake_lib' library to the 'snake' executable
target_link_libraries(${PROJECT_NAME} snake_lib ${OpenCV_LIBS} ${X11_LIBRARIES} ${Boost_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})

if(BOOST_EXTERNAL_TARGET)
    add_dependencies(messaging_app ${BOOST_EXTERNAL_TARGET})
endif()