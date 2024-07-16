#ifndef SNAKE_SNAKE_HPP
#define SNAKE_SNAKE_HPP

#include <queue>
#include <functional>
#include <random>
#include "CoordinateStructures.hpp"
#include "../consumables/Consumable.hpp"

class Snake {

public:
    explicit Snake(CoordinateStructures::Pixel coords);

    bool changeDirection(CoordinateStructures::Direction direction);

    void move();

    void grow();

    void applyEffect(Consumables::Effect effect);

    void setOnSteroids(bool value);

    void setHeadPosition(CoordinateStructures::Pixel newPos);

    [[nodiscard]] inline CoordinateStructures::Pixel &getHeadPosition() { return headPosition; }

    [[nodiscard]] inline std::deque<CoordinateStructures::Pixel> &getBody() { return body; }

    [[nodiscard]] inline CoordinateStructures::Direction getDirection() { return direction; }

    inline bool isOnSteroids() const { return onSteroids; };

private:
    CoordinateStructures::Pixel headPosition;
    std::deque<CoordinateStructures::Pixel> body;
    CoordinateStructures::Direction direction = CoordinateStructures::Direction::RIGHT;
    mutable std::mt19937 engine{std::random_device{}()};
    bool onSteroids = false;
    int steroidTimer = 3000;

};


#endif //SNAKE_SNAKE_HPP
