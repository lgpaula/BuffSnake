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

    [[nodiscard]] int getPoints() const;

    void gameOver();

    void onKeyPressed(int key);

    ~Game() noexcept;

private:
    void onHowToPlay();
    void setText();
    void addSelector();
    void startGame();
    void overlayMap();

private:
    int screenHeight;
    int screenWidth;
    cv::Mat fullscreenDisplay;
    std::thread mainDisplayThread{};

    cv::Point selectorPosition = {screenWidth / 2 - 500, screenHeight / 2 - 250};
    cv::Point titlePosition = {screenWidth / 2 - 450, screenHeight / 2 - 300};
    cv::Point mainMenuFirstOption = {screenWidth / 2 - 450, screenHeight / 2 - 250};
    cv::Point mainMenuSecondOption = {screenWidth / 2 - 450, screenHeight / 2 - 200};
    cv::Scalar backgroundColor = {81, 215, 170};

    std::shared_ptr<Snake> snake = nullptr;
    std::unique_ptr<Map> map = nullptr;

    int gamePoints = 0;
    bool onInstructions = false;
    bool gameRunning = false;
};


#endif //SNAKE_GAME_HPP
