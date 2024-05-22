#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "include/Map.hpp"
#include "include/Game.hpp"
#include "include/Snake.hpp"

int main() {
    cv::Mat window = cv::Mat::zeros(720, 1080, CV_8UC3);

    CoordinateStructures::Size dimension = {500, 500};
    CoordinateStructures::Steps steps = {dimension.width / 20, dimension.height / 20};

    Game game(window);

//    while (true) {
//        cv::imshow("Window", window);
//        int key = cv::waitKey(1);
//        if (key == 27) exit(0);
//        game.onKeyPressed(key);
//
//        if (key == '1') {
//            std::cout << "move on"  << std::endl;
//            cv::destroyWindow("Window");
//            break;
//        }
//    }

    std::cout << "Game started" << std::endl;

    CoordinateStructures::Pixel coords = {dimension.width / 2, dimension.height / 2};
    Snake snake(coords, steps, [&game]() {
        game.gameOver();
    });

    //create border
    Map map(dimension, [&snake, &map](CoordinateStructures::Direction input) {
        snake.changeDirection(input);
        map.updateSnake(snake);
    }, [&game, &map, &snake](Food::Consumable consumable) {
        game.addPoints(consumable.points);
        map.spawnConsumable(consumable);
        snake.grow();
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