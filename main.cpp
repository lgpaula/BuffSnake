/*
 * Normal snake game.
 * Closed borders.
 *
 * Power-ups after x food. -> up for discussion.
 *
 * food:
 *  - chicken -> points += 5
 *
 * POWER-UPS:
 *  - creatine: has 25% chance of not growing. points += 20
 *  - protein: points += 20
 *
 *  - steroids: 3 seconds to break 1 border or body (not instant).
 */

#include <iostream>
#include <thread>
#include <chrono>
#include "include/Border.hpp"
#include "include/Snake.hpp"

#include <cstdio>
#include <termios.h>
#include <unistd.h>

int main() {
    std::cout << "Hello, World!" << std::endl;

    //create border
    Border border(CoordinateStructures::Size{10, 10});

    //create snake
    Snake snake(border.getCenter());

    //create input
    KeyboardInput keyboardInput;

    //create food thread


    return 0;
}