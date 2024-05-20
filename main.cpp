#include <iostream>
#include <opencv2/highgui.hpp>
#include "include/Map.hpp"
#include "include/Game.hpp"
#include "include/Snake.hpp"

int main() {
    cv::Mat window = cv::Mat::zeros(720, 1080, CV_8UC3);

    CoordinateStructures::Size dimension = {500, 500};
    CoordinateStructures::Steps steps = {dimension.width / 20, dimension.height / 20};

    Game game(window);

    while (true) {
        cv::imshow("Window", window);
        int key = cv::waitKey(1);
        if (key == 27) exit(0);
        game.onKeyPressed(key);

        if (key == '1') {
            std::cout << "move on"  << std::endl;
            cv::destroyWindow("Window");
            break;
        }
    }

    std::cout << "Game started" << std::endl;

    //actual game not starting

    //create snake
    Snake snake({dimension.width / 2, dimension.height / 2}, steps, [&game]() {
        game.gameOver();
    });

    //create border
    Map map(dimension, [&snake, &map](CoordinateStructures::Direction input) {
        snake.move(input);
        map.updateSnake(snake);
    }, [&game, &map, &snake](Food::Consumable consumable) {
        game.addPoints(consumable.points);
        map.spawnConsumable(consumable);
        snake.grow();
    }, [&game]() {
        game.gameOver();
    });

    map.updateSnake(snake);
    Food::Consumable consumable = Food::Consumable{Food::ConsumableType::CHICKEN};
    map.spawnConsumable(consumable);

    return 0;
}