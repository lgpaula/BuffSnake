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

#include "include/Map.hpp"
#include "include/Snake.hpp"

int main() {
    //create border
    Map map(CoordinateStructures::Size{500, 500});

    //create snake
    Snake snake(map.getCenter(), map.getSteps());

    map.addSnake(snake);
    map.show();

    //create food thread


    return 0;
}