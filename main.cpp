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

int main() {
    std::cout << "Hello, World!" << std::endl;

    //create border

    //create snake



    //create food thread
    int inputMove;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        int ch = getchar();
        std::cout << "ch: " << ch << std::endl;
    }



    return 0;
}
