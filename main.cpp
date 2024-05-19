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
 *  - steroids: 3 seconds to break 1 border or body (triggered by user).
 */

#include <iostream>
#include "include/Map.hpp"
#include "include/Game.hpp"
#include "include/Snake.hpp"

int main() {
    Game game;

    CoordinateStructures::Size mapSize = {500, 500};
    CoordinateStructures::Steps steps = {mapSize.width / 20, mapSize.height / 20};

    //create snake
    Snake snake({mapSize.width / 2, mapSize.height / 2}, steps);

    //create border
    Map map(mapSize, [&snake, &map](CoordinateStructures::Direction input) {
        snake.move(input);
        map.updateSnake(snake);
    }, [&game, &map, &snake](Food::Consumable consumable) {
        game.addPoints(consumable.points);
        map.spawnConsumable(consumable);
        snake.grow();
    });

    map.updateSnake(snake);
    Food::Consumable consumable = Food::Consumable{Food::ConsumableType::CHICKEN};
    map.spawnConsumable(consumable);

    return 0;
}