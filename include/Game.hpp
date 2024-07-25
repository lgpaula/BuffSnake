#ifndef SNAKE_GAME_HPP
#define SNAKE_GAME_HPP

#include <opencv2/core.hpp>
#include <thread>
#include "../gameModes/SinglePlayer.hpp"
#include "../gameModes/MultiPlayerLocal.hpp"
#include "Helper.hpp"
#include "../consumables/DragonBalls.hpp"
#include <set>

class Game {
public:
    Game(int screenHeight, int screenWidth);

    ~Game() noexcept;

private:
    void updateScore();
    void singlePlayerInstructions();
    void multiPlayerInstructions();
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
    void addBackground(const std::string& path);
    void resetElements();
    void updateSteroidCount(Consumables::PowerUpConsumed power);
    void updateConsumables();
    int removeLife(int snakeId);
    int addLife(int snakeId, int consumableId);
    void gameOverMultiPlayer(int snakeId);
    void gameOver();
    void addPoints(int points);
    void onKeyPressed(int key);
    void onPlayerSelection(int key);
    void resizeIcons();
    void addPowerUp(Consumables::Effect effect, int snakeId);
    bool removePowerUp(Consumables::Effect effect, int snakeId);
    bool updatePowerUps(Consumables::Effect effect, Consumables::PowerUpConsumed power, int snakeId);
    void setGameOverScreenSinglePlayer();
    void setGameOverScreenMultiPlayer();
    void nopeScreen();

private:
    int screenHeight;
    int screenWidth;
    cv::Mat fullscreenDisplay;
    std::thread mainDisplayThread{};

    cv::Point selectorPosition = {screenWidth / 2 - 500, screenHeight / 2 - 250};
    cv::Point mainMenuFirstOption = {screenWidth / 2 - 450, screenHeight / 2 - 250};
    cv::Point mainMenuSecondOption = {screenWidth / 2 - 450, screenHeight / 2 - 200};
    cv::Point mainMenuThirdOption = {screenWidth / 2 - 450, screenHeight / 2 - 150};
    cv::Point mainMenuFourthOption = {screenWidth / 2 - 450, screenHeight / 2 - 100};
//    cv::Point mainMenuFifthOption = {screenWidth / 2 - 450, screenHeight / 2 - 50};

    cv::Point finalScorePosition = {screenWidth / 2 - 450, screenHeight / 2 - 250};
    cv::Point playAgainPosition = {screenWidth / 2 - 450, screenHeight / 2 - 200};
    cv::Point returnToMenuPosition = {screenWidth / 2 - 450, screenHeight / 2 - 150};
    cv::Point mapPosition = {screenWidth / 2 - 250, screenHeight / 2 - 250};
    cv::Point steroidPosition = {screenWidth / 2 + 350, screenHeight / 2 - 280};

    std::unique_ptr<GameMode> gameMode = nullptr;
    Mode mode{};

    int gamePoints = 0;
    bool onMainMenu = true;
    bool onInstructions = false;
    bool onGameOver = false;
    bool gameRunning = false;
    bool resetElementsFlag = true;
    int steroidCounter = 0;
    int amountOfPlayers{};
    int winner{};

    static constexpr int KEY_BACKSPACE = 8;
    static constexpr int KEY_UP = 82;
    static constexpr int KEY_DOWN = 84;
    static constexpr int KEY_ENTER = 13;
    const cv::Scalar backgroundColor = {73, 209, 162};
    const cv::Scalar white = {255, 255, 255};
    const cv::Scalar black = {0, 0, 0};

    const std::array<std::vector<cv::Point>, 4> dbPositions = {
            std::vector<cv::Point>{cv::Point(360, 61), cv::Point(390, 61), cv::Point(420, 61), cv::Point(450, 61), cv::Point(480, 61), cv::Point(510, 61), cv::Point(540, 61)},
            std::vector<cv::Point>{cv::Point(1613, 90), cv::Point(1613, 120), cv::Point(1613, 150), cv::Point(1613, 180), cv::Point(1613, 210), cv::Point(1613, 240), cv::Point(1613, 270)},
            std::vector<cv::Point>{cv::Point(331, 939), cv::Point(331, 909), cv::Point(331, 879), cv::Point(331, 849), cv::Point(331, 819), cv::Point(331, 789), cv::Point(331, 759)},
            std::vector<cv::Point>{cv::Point(1584, 968), cv::Point(1554, 968), cv::Point(1524, 968), cv::Point(1494, 968), cv::Point(1464, 968), cv::Point(1434, 968), cv::Point(1404, 968)}
    };

    const std::array<std::vector<cv::Point>, 4> consumablePositions = {
            std::vector<cv::Point>{cv::Point(360, 31), cv::Point(390, 31), cv::Point(420, 31), cv::Point(450, 31), cv::Point(480, 31)},
            std::vector<cv::Point>{cv::Point(1643, 90), cv::Point(1643, 120), cv::Point(1643, 150), cv::Point(1643, 180), cv::Point(1643, 210)},
            std::vector<cv::Point>{cv::Point(301, 939), cv::Point(301, 909), cv::Point(301, 879), cv::Point(301, 849), cv::Point(301, 819)},
            std::vector<cv::Point>{cv::Point(1584, 998), cv::Point(1554, 998), cv::Point(1524, 998), cv::Point(1494, 998), cv::Point(1464, 998)}
    };

    std::array<std::set<int>, 4> dragonBalls{
            std::set<int>{4},
            std::set<int>{4},
            std::set<int>{4},
            std::set<int>{4}
    };

    std::array<std::vector<Consumables::Effect>, 4> powerUps{
            std::vector<Consumables::Effect>{},
            std::vector<Consumables::Effect>{},
            std::vector<Consumables::Effect>{},
            std::vector<Consumables::Effect>{}
    };

    cv::Mat steroidIcon = cv::imread("icons/steroids.png", cv::IMREAD_UNCHANGED);
    cv::Mat preworkoutIcon = cv::imread("icons/preworkout.png", cv::IMREAD_UNCHANGED);
    cv::Mat db1Icon = cv::imread("icons/db1.png", cv::IMREAD_UNCHANGED);
    cv::Mat db2Icon = cv::imread("icons/db2.png", cv::IMREAD_UNCHANGED);
    cv::Mat db3Icon = cv::imread("icons/db3.png", cv::IMREAD_UNCHANGED);
    cv::Mat db4Icon = cv::imread("icons/db4.png", cv::IMREAD_UNCHANGED);
    cv::Mat db5Icon = cv::imread("icons/db5.png", cv::IMREAD_UNCHANGED);
    cv::Mat db6Icon = cv::imread("icons/db6.png", cv::IMREAD_UNCHANGED);
    cv::Mat db7Icon = cv::imread("icons/db7.png", cv::IMREAD_UNCHANGED);

    std::array<cv::Mat, 7> dbIcons = {db1Icon, db2Icon, db3Icon, db4Icon, db5Icon, db6Icon, db7Icon};

    cv::Mat bigBlue = cv::imread("icons/big_blue_snake.png", cv::IMREAD_UNCHANGED);
    cv::Mat bigGreen = cv::imread("icons/big_green_snake.png", cv::IMREAD_UNCHANGED);
    cv::Mat bigRed = cv::imread("icons/big_red_snake.png", cv::IMREAD_UNCHANGED);
    cv::Mat bigYellow = cv::imread("icons/big_yellow_snake.png", cv::IMREAD_UNCHANGED);
    cv::Mat smallBlue = cv::imread("icons/sad_blue_snake.png", cv::IMREAD_UNCHANGED);
    cv::Mat smallGreen = cv::imread("icons/sad_green_snake.png", cv::IMREAD_UNCHANGED);
    cv::Mat smallRed = cv::imread("icons/sad_red_snake.png", cv::IMREAD_UNCHANGED);
    cv::Mat smallYellow = cv::imread("icons/sad_yellow_snake.png", cv::IMREAD_UNCHANGED);

    std::array<cv::Mat, 4> bigSnakes = {bigBlue, bigRed, bigGreen, bigYellow};
    std::array<cv::Mat, 4> smallSnakes = {smallBlue, smallRed, smallGreen, smallYellow};

    const std::array<std::vector<cv::Point>, 4> snakeDisplayPositions = {
            std::vector<cv::Point>{cv::Point(887, 25), cv::Point(1030, 540)},
            std::vector<cv::Point>{cv::Point(887, 25), cv::Point(911, 540), cv::Point(1158, 540)},
            std::vector<cv::Point>{cv::Point(887, 25), cv::Point(710, 540), cv::Point(1030, 540), cv::Point(1350, 540)},
            std::vector<cv::Point>{cv::Point(550, 540), cv::Point(870, 540), cv::Point(1190, 540), cv::Point(1510, 540)}
    };
};


#endif //SNAKE_GAME_HPP
