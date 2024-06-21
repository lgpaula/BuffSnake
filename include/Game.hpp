#ifndef SNAKE_GAME_HPP
#define SNAKE_GAME_HPP

#include <opencv2/core.hpp>
#include <thread>
#include "Map.hpp"
#include "Snake.hpp"
#include "CoordinateStructures.hpp"

class Game {
public:
    Game(int screenHeight, int screenWidth);

    void addPoints(int points);

    void gameOver();

    void onKeyPressed(int key);

    ~Game() noexcept;

private:
    void onHowToPlay();
    void setMainMenuText();
    void addSelector(int height);
    void startGame();
    void overlayMap();
    void setGameOverScreen();
    void handleBackspace();
    void handleKeyUp();
    void handleKeyDown();
    void handleEnter();
    void moveSelectorUp();
    void moveSelectorDown();

private:
    int screenHeight;
    int screenWidth;
    cv::Mat fullscreenDisplay;
    std::thread mainDisplayThread{};

    cv::Point selectorPosition = {screenWidth / 2 - 500, screenHeight / 2 - 250};
    cv::Point titlePosition = {screenWidth / 2 - 450, screenHeight / 2 - 300};
    cv::Point mainMenuFirstOption = {screenWidth / 2 - 450, screenHeight / 2 - 250};
    cv::Point mainMenuSecondOption = {screenWidth / 2 - 450, screenHeight / 2 - 200};
    cv::Point gameOverPosition = {screenWidth / 2 - 450, screenHeight / 2 - 300};
    cv::Point scorePosition = {screenWidth / 2 - 450, screenHeight / 2 - 250};
    cv::Point playAgainPosition = {screenWidth / 2 - 450, screenHeight / 2 - 200};
    cv::Point returnToMenuPosition = {screenWidth / 2 - 450, screenHeight / 2 - 150};
    cv::Point mapPosition = {screenWidth / 2 - 250, screenHeight / 2 - 250};

    std::shared_ptr<Snake> snake = nullptr;
    std::unique_ptr<Map> map = nullptr;

    int gamePoints = 0;
    bool onMainMenu = true;
    bool onGameOver = false;
    bool gameRunning = false;
    bool resetElementsFlag = true;

    static constexpr int KEY_BACKSPACE = 8;
    static constexpr int KEY_UP = 82;
    static constexpr int KEY_DOWN = 84;
    static constexpr int KEY_ENTER = 13;
    const cv::Scalar backgroundColor = {73, 209, 162};
    const cv::Scalar white = {255, 255, 255};
    const cv::Scalar black = {0, 0, 0};

};


#endif //SNAKE_GAME_HPP
