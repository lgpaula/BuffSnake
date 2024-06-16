#include "../include/Game.hpp"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

Game::Game(int screenHeight, int screenWidth) : screenHeight(screenHeight), screenWidth(screenWidth) {
        fullscreenDisplay = cv::Mat(screenHeight, screenWidth, CV_8UC3, backgroundColor);

    setText();

    mainDisplayThread = std::thread([this]() {
        while (true) {
            if (gameRunning) {
                map->updateMap();
                overlayMap();
                cv::imshow("Game", fullscreenDisplay);
                continue;
            }
            cv::imshow("Game", fullscreenDisplay);
            int key = cv::waitKey(1);
            onKeyPressed(key);
            if (key == 27) {
                cv::destroyAllWindows();
                exit(0);
            }
        }
    });
}

void Game::overlayMap() {
    cv::rectangle(fullscreenDisplay, cv::Point(0, 0), cv::Point(fullscreenDisplay.cols, fullscreenDisplay.rows), backgroundColor, cv::FILLED);

    cv::Point mapPosition = {screenWidth / 2 - 250, screenHeight / 2 - 250};
    cv::Rect roi(mapPosition, map->getMap().size());
    map->getMap().copyTo(fullscreenDisplay(roi));
}

void Game::setText() {
    cv::rectangle(fullscreenDisplay, cv::Point(0, 0), cv::Point(fullscreenDisplay.cols, fullscreenDisplay.rows), backgroundColor, cv::FILLED);
    cv::putText(fullscreenDisplay, "Snake Game", titlePosition, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 0), 2);
    cv::putText(fullscreenDisplay, "Start", mainMenuFirstOption, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(fullscreenDisplay, "Instructions", mainMenuSecondOption, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    addSelector();
}

void Game::addSelector() {
    cv::rectangle(fullscreenDisplay, cv::Point(screenWidth / 2 - 500, screenHeight / 2 - 300),
                  cv::Point(screenWidth / 2 - 475, screenHeight / 2 - 200), backgroundColor, cv::FILLED);
    cv::putText(fullscreenDisplay, ">", selectorPosition,
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
}

void Game::onKeyPressed(int key) {
    switch (key) {
        case 8:
            std::cout << "back" << std::endl;
            if (onInstructions) {
                onInstructions = false;
                setText();
            }
            break;
        case 82:
            if (onInstructions) break;
            if (selectorPosition.y == mainMenuSecondOption.y) {
                selectorPosition.y -= 50;
                addSelector();
            }
            break;
        case 84:
            if (onInstructions) break;
            if (selectorPosition.y == mainMenuFirstOption.y) {
                selectorPosition.y += 50;
                addSelector();
            }
            break;
        case 13:
            if (selectorPosition.y == mainMenuFirstOption.y) {
                startGame();
            } else if (selectorPosition.y == mainMenuSecondOption.y) {
                onHowToPlay();
            }
            break;
        default:
            break;
    }
}

void Game::onHowToPlay() {
    onInstructions = true;
    cv::rectangle(fullscreenDisplay, cv::Point(0, 0), cv::Point(fullscreenDisplay.cols, fullscreenDisplay.rows), backgroundColor, cv::FILLED);
    cv::putText(fullscreenDisplay, "How to play", cv::Point(fullscreenDisplay.cols / 2 - 450, fullscreenDisplay.rows / 2 - 300),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 0), 2);
    cv::putText(fullscreenDisplay, "Move the snake with the arrow keys", cv::Point(fullscreenDisplay.cols / 2 - 450, fullscreenDisplay.rows / 2 - 250),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(fullscreenDisplay, "Eating chicken makes you grow and give you 5 points",
                cv::Point(fullscreenDisplay.cols / 2 - 450, fullscreenDisplay.rows / 2 - 200),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(fullscreenDisplay, "Eating protein makes you grow and give you 20 points",
                cv::Point(fullscreenDisplay.cols / 2 - 450, fullscreenDisplay.rows / 2 - 150),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(fullscreenDisplay, "Eating creatine gives you 20 points and makes you grow 75% of the times",
                cv::Point(fullscreenDisplay.cols / 2 - 450, fullscreenDisplay.rows / 2 - 100),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(fullscreenDisplay, "Using steroid lets you destroy one border piece or cut through your body",
                cv::Point(fullscreenDisplay.cols / 2 - 450, fullscreenDisplay.rows / 2 - 50),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(fullscreenDisplay, "Press 'S' to activate the steroid effect. It lasts 3 seconds and costs 10 points",
                cv::Point(fullscreenDisplay.cols / 2 - 450, fullscreenDisplay.rows / 2),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(fullscreenDisplay, "Genetics gives you 0 points and slows down the snake",
                cv::Point(fullscreenDisplay.cols / 2 - 450, fullscreenDisplay.rows / 2 + 50),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(fullscreenDisplay, "Backspace to return",
                cv::Point(fullscreenDisplay.cols / 2 - 450, fullscreenDisplay.rows / 2 + 150),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::putText(fullscreenDisplay, "ESC to exit",
                cv::Point(fullscreenDisplay.cols / 2 - 450, fullscreenDisplay.rows / 2 + 200),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);

    int key = cv::waitKey(1);
    onKeyPressed(key);
}

void Game::startGame() {
    auto size = CoordinateStructures::Size{500, 500};

    snake = std::make_shared<Snake>(CoordinateStructures::Pixel{size.width / 2, size.height / 2}, CoordinateStructures::Steps{size.width / 20, size.height / 20});
    map = std::make_unique<Map>(snake, size, [this](const Food::Consumable& consumable) {
        addPoints(consumable.points);
    }, [this]() {
        gameOver();
    });

    gameRunning = true;
}

void Game::addPoints(int points) {
    gamePoints += points;
}

int Game::getPoints() const {
    return gamePoints;
}

void Game::gameOver() {
    gameRunning = false;
    std::cout << "Game over" << std::endl;
    exit(0);
}

Game::~Game() noexcept {
    cv::destroyAllWindows();
    if (mainDisplayThread.joinable()) mainDisplayThread.join();
}
