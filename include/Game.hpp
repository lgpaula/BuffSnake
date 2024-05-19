#ifndef SNAKE_GAME_HPP
#define SNAKE_GAME_HPP

#include "Display.hpp"

class Game {
public:
    Game(Display &display);

    void addPoints(int points);

    [[nodiscard]] int getPoints() const;

    void gameOver();

private:
    int gamePoints = 0;
    Display &display;
};


#endif //SNAKE_GAME_HPP
