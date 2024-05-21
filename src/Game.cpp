#include "../include/Game.hpp"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

Game::Game(cv::Mat &window) : window(window) {
    setInitialScreen();
    std::cout << "Game created" << std::endl;
}

void Game::setInitialScreen() {
    cv::rectangle(window, cv::Point(0, 0), cv::Point(window.cols, window.rows), cv::Scalar(73, 209, 162), cv::FILLED);
    cv::putText(window, "Snake Game", cv::Point(window.cols / 2 - 450, window.rows / 2 - 300),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 0), 2);
    cv::putText(window, "Press 1 to start", cv::Point(window.cols / 2 - 450, window.rows / 2 - 250),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(window, "Press 2 for instructions", cv::Point(window.cols / 2 - 450, window.rows / 2 - 200),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
}

void Game::onKeyPressed(int key) {
    switch (key) {
        case 8:
            if (onInstructions) {
                onInstructions = false;
                setInitialScreen();
            }
            break;
        case '1':
            if (!onInstructions) initialScreen = false;
            break;
        case '2':
            if (!onInstructions) onHowToPlay();
            break;
        default:
            break;
    }
}

void Game::onHowToPlay() {
    onInstructions = true;
    cv::rectangle(window, cv::Point(0, 0), cv::Point(window.cols, window.rows), cv::Scalar(73, 209, 162), cv::FILLED);
    cv::putText(window, "How to play", cv::Point(window.cols / 2 - 450, window.rows / 2 - 300),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 0), 2);
    cv::putText(window, "Move the snake with the arrow keys", cv::Point(window.cols / 2 - 450, window.rows / 2 - 250),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(window, "Eating chicken makes you grow and give you 5 points",
                cv::Point(window.cols / 2 - 450, window.rows / 2 - 200),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(window, "Eating protein makes you grow and give you 20 points",
                cv::Point(window.cols / 2 - 450, window.rows / 2 - 150),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(window, "Eating creatine gives you 20 points and makes you grow 75% of the times",
                cv::Point(window.cols / 2 - 450, window.rows / 2 - 100),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(window, "Using steroid lets you destroy one border piece or cut through your body",
                cv::Point(window.cols / 2 - 450, window.rows / 2 - 50),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(window, "Press 'S' to activate the steroid effect. It lasts 3 seconds and costs 10 points",
                cv::Point(window.cols / 2 - 450, window.rows / 2),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(window, "Press '1' to start the game", cv::Point(window.cols / 2 - 450, window.rows / 2 + 50),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);

    int key = cv::waitKey(1);
    onKeyPressed(key);
}

void Game::addPoints(int points) {
    gamePoints += points;
}

int Game::getPoints() const {
    return gamePoints;
}

void Game::gameOver() {
    std::cout << "Game over" << std::endl;
}
