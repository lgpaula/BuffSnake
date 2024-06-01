#ifndef SNAKE_SNAKE_HPP
#define SNAKE_SNAKE_HPP

#include <queue>
#include <functional>
#include <random>
#include "CoordinateStructures.hpp"
#include "IConsumables.hpp"

class Snake {

public:
    using OnGameOver = std::function<void()>;

    Snake(CoordinateStructures::Pixel coords, const CoordinateStructures::Steps &steps, OnGameOver onGameOver);

    bool changeDirection(CoordinateStructures::Direction direction);

    void move();

    void grow();

    void applyEffect(Food::Effect effect);

    [[nodiscard]] inline CoordinateStructures::Pixel &getHeadPosition() { return headPosition; }

    [[nodiscard]] inline std::deque<CoordinateStructures::Pixel> &getBody() { return body; }

    inline bool isOnSteroids() const { return onSteroids; };

    void setOnSteroids(bool value);

    void setHeadPosition(CoordinateStructures::Pixel newPos);

private:
    void checkCollision();

private:
    CoordinateStructures::Pixel headPosition;
    std::deque<CoordinateStructures::Pixel> body;
    CoordinateStructures::Steps steps;
    OnGameOver onGameOver;
    CoordinateStructures::Direction direction = CoordinateStructures::Direction::RIGHT;
    mutable std::mt19937 engine{std::random_device{}()};
    bool onSteroids = false;
    int rampageTimer = 3000;

};


#endif //SNAKE_SNAKE_HPP
