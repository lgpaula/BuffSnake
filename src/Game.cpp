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
                updateScore();
                cv::imshow("Game", fullscreenDisplay);
                continue;
            }
            if (onGameOver && resetElementsFlag) {
                resetElementsFlag = false;
                setGameOverScreen();
                cv::imshow("Game", fullscreenDisplay);
                continue;
            }
            cv::imshow("Game", fullscreenDisplay);
            int key = cv::waitKey(1);
            onKeyPressed(key);
        }
    });
}

void Game::updateScore() {
    cv::Point point = {mapPosition.x + 5, mapPosition.y - 5};
    cv::putText(fullscreenDisplay, "SCORE: " + std::to_string(gamePoints), point, cv::FONT_HERSHEY_SIMPLEX, 0.75, black, 2);
}

void Game::overlayMap() {
    cv::rectangle(fullscreenDisplay, cv::Point(0, 0), cv::Point(fullscreenDisplay.cols, fullscreenDisplay.rows), backgroundColor, cv::FILLED);

    cv::Rect roi(mapPosition, map->getMap().size());
    map->getMap().copyTo(fullscreenDisplay(roi));
}

void removeAlpha(cv::Mat& roi, const cv::Mat& mat) {
    std::vector<cv::Mat> channels;
    cv::split(mat, channels);
    cv::Mat alpha = channels[3];
    channels.pop_back();
    cv::Mat bgr;
    cv::merge(channels, bgr);
    cv::Mat mask;
    cv::cvtColor(alpha, mask, cv::COLOR_GRAY2BGR);
    cv::Mat blended;
    cv::multiply(bgr, mask, blended, 1.0 / 255);
    cv::multiply(roi, cv::Scalar::all(255) - mask, roi, 1.0 / 255);
    cv::add(roi, blended, roi);
}

void Game::addBackground(const std::string& path) {
    cv::Mat logo = cv::imread(path, cv::IMREAD_UNCHANGED);
    cv::Mat roi = fullscreenDisplay(cv::Rect(0, 0, logo.cols, logo.rows));
    removeAlpha(roi, logo);
}

void Game::setMainMenuText() {
    addBackground("icons/mainMenuSnake.png");
    onMainMenu = true;
    onGameOver = false;
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
    cv::putText(fullscreenDisplay, "Press 'S' to activate the steroid effect. It lasts 3 seconds and costs 50 points",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2), cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Genetic slows down the snake",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2 + 50), cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Space to pause",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2 + 100), cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Backspace to return",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2 + 200), cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "ESC to exit",
                cv::Point(screenWidth / 2 - 450, screenHeight / 2 + 250), cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);

    int key = cv::waitKey(1);
    onKeyPressed(key);
}

void Game::startGame() {
    resetElements();
    auto size = CoordinateStructures::Size{25, 25};

    snake = std::make_shared<Snake>(CoordinateStructures::Pixel{size.width / 2, size.height / 2});
    map = std::make_unique<Map>(snake, size, [this](const Consumables::Consumable& consumable) {
        addPoints(consumable.getPoints());
    }, [this]() {
        gameOver();
    });

    gameRunning = true;
}

void Game::resetElements() {
    gamePoints = 0;
    if (map != nullptr) map.reset();
    if (snake != nullptr) snake.reset();
}

void Game::addPoints(int points) {
    gamePoints += points;
}

void Game::setGameOverScreen() {
    addBackground("icons/gameOverSnake2.png");
    cv::putText(fullscreenDisplay, "Score: " + std::to_string(gamePoints), finalScorePosition, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
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
