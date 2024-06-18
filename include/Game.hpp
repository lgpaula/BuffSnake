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
    void addSelector();
    void startGame();
    void overlayMap();
    void gameOverScreen();
    void handleBackspace();
    void handleKeyR();
    void handleKeyT();
    void handleEnter();
    void moveSelectorUp(int positionY);
    void moveSelectorDown(int positionY);


private:
    int screenHeight;
    int screenWidth;
    cv::Mat fullscreenDisplay;
    std::thread mainDisplayThread{};

    cv::Point selectorPosition = {screenWidth / 2 - 500, screenHeight / 2 - 250};
    cv::Point titlePosition = {screenWidth / 2 - 450, screenHeight / 2 - 300};
    cv::Point mainMenuFirstOption = {screenWidth / 2 - 450, screenHeight / 2 - 250};
    cv::Point mainMenuSecondOption = {screenWidth / 2 - 450, screenHeight / 2 - 200};
    cv::Point gameOverPosition = {fullscreenDisplay.cols / 2 - 450, fullscreenDisplay.rows / 2 - 300};
    cv::Point scorePosition = {fullscreenDisplay.cols / 2 - 450, fullscreenDisplay.rows / 2 - 250};
    cv::Point playAgainPosition = {fullscreenDisplay.cols / 2 - 450, fullscreenDisplay.rows / 2 - 200};
    cv::Point returnToMenuPosition = {fullscreenDisplay.cols / 2 - 450, fullscreenDisplay.rows / 2 - 150};

    std::shared_ptr<Snake> snake = nullptr;
    std::unique_ptr<Map> map = nullptr;

    int gamePoints = 0;
    bool onMainMenu = true;
    bool onGameOver = false;
    bool gameRunning = false;

    static constexpr int KEY_BACKSPACE = 8;
    static constexpr int KEY_UP = 82;
    static constexpr int KEY_DOWN = 84;
    static constexpr int KEY_ENTER = 13;
    const cv::Scalar backgroundColor = {73, 209, 162};
    const cv::Scalar white = {255, 255, 255};
    const cv::Scalar black = {0, 0, 0};

};


#endif //SNAKE_GAME_HPP
