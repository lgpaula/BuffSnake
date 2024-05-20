#ifndef SNAKE_SNAKE_HPP
#define SNAKE_SNAKE_HPP

#include <queue>
#include <functional>
#include "CoordinateStructures.hpp"

class Snake {

public:
    using OnGameOver = std::function<void()>;

    Snake(CoordinateStructures::Pixel coords, const CoordinateStructures::Steps &steps, OnGameOver onGameOver);

    void move(CoordinateStructures::Direction direction);

    void grow();

    [[nodiscard]] inline CoordinateStructures::Pixel &getHeadPosition() { return headPosition; }

    [[nodiscard]] inline std::deque<CoordinateStructures::Pixel> &getBody() { return body; }

private:
    void checkCollision();

private:
    CoordinateStructures::Pixel headPosition;
    std::deque<CoordinateStructures::Pixel> body;
    CoordinateStructures::Steps steps;
    OnGameOver onGameOver;
    CoordinateStructures::Direction direction = CoordinateStructures::Direction::RIGHT;

};


#endif //SNAKE_SNAKE_HPP
