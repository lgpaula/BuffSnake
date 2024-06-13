#include <iostream>
#include "include/Map.hpp"
#include "include/Game.hpp"
#include "include/Snake.hpp"

int main() {
    cv::Mat window = cv::Mat::zeros(720, 1080, CV_8UC3);

    CoordinateStructures::Size dimension = {500, 500};
    CoordinateStructures::Steps steps = {dimension.width / 20, dimension.height / 20};

    Game game(window);

    CoordinateStructures::Pixel coords = {dimension.width / 2, dimension.height / 2};
    Snake snake(coords, steps, [&game]() {
        game.gameOver();
    });

    Map map(dimension, [&snake, &map](CoordinateStructures::Direction input) {
        if (snake.changeDirection(input)) map.updateTimer();
        map.updateSnake(snake);
    }, [&game, &map, &snake](const Food::Consumable& consumable) {
        game.addPoints(consumable.points);
        if (consumable.type == Food::CHICKEN) map.spawnConsumable(consumable);
        snake.applyEffect(consumable.effect);
        map.updatePoints(game.getPoints());
    }, [&game]() {
        game.gameOver();
    }, [&snake, &map]() {
        snake.move();
        map.updateSnake(snake);
    });

    map.updateSnake(snake);
    Food::Consumable consumable = Food::Consumable{Food::ConsumableType::CHICKEN};
    map.spawnConsumable(consumable);

    return 0;
}