#ifndef SNAKE_GAME_HPP
#define SNAKE_GAME_HPP

#include <opencv2/core.hpp>

class Game {
public:
    explicit Game(cv::Mat &window);

    void addPoints(int points);

    [[nodiscard]] int getPoints() const;

    void gameOver();

    void onKeyPressed(int key);

private:
    void setInitialScreen();
    void onHowToPlay();

private:
    int gamePoints = 0;
    cv::Mat window;
    bool onInstructions = false;
    bool initialScreen = true;
};


#endif //SNAKE_GAME_HPP
