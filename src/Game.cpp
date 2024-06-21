#include "../include/Game.hpp"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

Game::Game(int screenHeight, int screenWidth) : screenHeight(screenHeight), screenWidth(screenWidth) {
        fullscreenDisplay = cv::Mat(screenHeight, screenWidth, CV_8UC3, backgroundColor);

    setMainMenuText();

    mainDisplayThread = std::thread([this]() {
        while (true) {
            if (gameRunning) {
                map->updateMap();
                overlayMap();
                cv::imshow("Game", fullscreenDisplay);
                continue;
            }
            if (onGameOver && resetElementsFlag) {
                resetElementsFlag = false;
                setGameOverScreen();
                if (map != nullptr) map.reset();
                if (snake != nullptr) snake.reset();
                cv::imshow("Game", fullscreenDisplay);
                continue;
            }
            cv::imshow("Game", fullscreenDisplay);
            int key = cv::waitKey(1);
            onKeyPressed(key);
        }
    });
}

void Game::overlayMap() {
    cv::rectangle(fullscreenDisplay, cv::Point(0, 0), cv::Point(fullscreenDisplay.cols, fullscreenDisplay.rows), backgroundColor, cv::FILLED);

    cv::Rect roi(mapPosition, map->getMap().size());
    map->getMap().copyTo(fullscreenDisplay(roi));
}

void Game::setMainMenuText() {
    onMainMenu = true;
    onGameOver = false;
    cv::rectangle(fullscreenDisplay, cv::Point(0, 0), cv::Point(fullscreenDisplay.cols, fullscreenDisplay.rows), backgroundColor, cv::FILLED);
    cv::putText(fullscreenDisplay, "Snake Game", titlePosition, cv::FONT_HERSHEY_SIMPLEX, 1, black, 2);
    cv::putText(fullscreenDisplay, "Start", mainMenuFirstOption, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Instructions", mainMenuSecondOption, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    addSelector(mainMenuFirstOption.y);
}

void Game::addSelector(int height) {
    if (!onMainMenu && !onGameOver) return;
    selectorPosition.y = height;
    cv::rectangle(fullscreenDisplay, cv::Point(screenWidth / 2 - 500, screenHeight / 2 - 300),
                  cv::Point(screenWidth / 2 - 475, screenHeight / 2 - 100), backgroundColor, cv::FILLED);
    cv::putText(fullscreenDisplay, ">", selectorPosition, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
}

void Game::onKeyPressed(int key) { //NOLINT
    switch (key) {
        case KEY_BACKSPACE:
            handleBackspace();
            break;
        case KEY_UP:
            handleKeyUp();
            break;
        case KEY_DOWN:
            handleKeyDown();
            break;
        case KEY_ENTER:
            handleEnter();
            break;
        case 27:
            cv::destroyAllWindows();
            exit(0);
        default:
            break;
    }
}

void Game::handleBackspace() {
    if (!onMainMenu && !onGameOver) setMainMenuText();
}

void Game::handleKeyUp() {
    moveSelectorUp();
}

void Game::handleKeyDown() {
    moveSelectorDown();
}

void Game::handleEnter() { //NOLINT
    if (onMainMenu) {
        if (selectorPosition.y == mainMenuFirstOption.y) {
            startGame();
        } else if (selectorPosition.y == mainMenuSecondOption.y) {
            onHowToPlay();
        }
    } else if (onGameOver) {
        if (selectorPosition.y == playAgainPosition.y) {
            startGame();
        } else if (selectorPosition.y == returnToMenuPosition.y) {
            setMainMenuText();
        }
    }
}

void Game::moveSelectorUp() {
    selectorPosition.y -= 50;
    if (onMainMenu) {
        selectorPosition.y = std::clamp(selectorPosition.y, mainMenuFirstOption.y, mainMenuSecondOption.y);
    }
    if (onGameOver) {
        selectorPosition.y = std::clamp(selectorPosition.y, playAgainPosition.y, returnToMenuPosition.y);
    }
    addSelector(selectorPosition.y);
}

void Game::moveSelectorDown() {
    selectorPosition.y += 50;
    if (onMainMenu) {
        selectorPosition.y = std::clamp(selectorPosition.y, mainMenuFirstOption.y, mainMenuSecondOption.y);
    }
    if (onGameOver) {
        selectorPosition.y = std::clamp(selectorPosition.y, playAgainPosition.y, returnToMenuPosition.y);
    }
    addSelector(selectorPosition.y);
}

void Game::onHowToPlay() { //NOLINT
    onMainMenu = false;
    //todo replace with image
    cv::rectangle(fullscreenDisplay, cv::Point(0, 0), cv::Point(fullscreenDisplay.cols, fullscreenDisplay.rows), backgroundColor, cv::FILLED);
    cv::putText(fullscreenDisplay, "How to play",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2 - 300), cv::FONT_HERSHEY_SIMPLEX, 1, black, 2);
    cv::putText(fullscreenDisplay, "Move the snake with the arrow keys",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2 - 250), cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Eating chicken makes you grow and give you 5 points",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2 - 200), cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Eating protein makes you grow and give you 20 points",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2 - 150), cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Eating creatine gives you 20 points and makes you grow 75% of the times",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2 - 100), cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Using steroid lets you destroy one border piece or cut through your body",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2 - 50), cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Press 'S' to activate the steroid effect. It lasts 3 seconds and costs 10 points",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2), cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Genetics gives you 0 points and slows down the snake",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2 + 50), cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Backspace to return",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2 + 150), cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "ESC to exit",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2 + 200), cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);

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

void Game::setGameOverScreen() {
    cv::rectangle(fullscreenDisplay, cv::Point(0, 0), cv::Point(fullscreenDisplay.cols, fullscreenDisplay.rows), backgroundColor, cv::FILLED);
    cv::putText(fullscreenDisplay, "Game Over", gameOverPosition, cv::FONT_HERSHEY_SIMPLEX, 1, black, 2);
    cv::putText(fullscreenDisplay, "Score: " + std::to_string(gamePoints), scorePosition, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Play again", playAgainPosition, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Return to main menu", returnToMenuPosition, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    addSelector(playAgainPosition.y);
}

void Game::gameOver() {
    std::cout << "Game over" << std::endl;
    onGameOver = true;
    onMainMenu = false;
    gameRunning = false;
    resetElementsFlag = true;
}

Game::~Game() noexcept {
    cv::destroyAllWindows();
    if (mainDisplayThread.joinable()) mainDisplayThread.join();
}
