#ifndef SNAKE_SNAKE_HPP
#define SNAKE_SNAKE_HPP

#include <queue>
#include "CoordinateStructures.hpp"

class Snake {

public:
    explicit Snake(CoordinateStructures::Pixel coords, const CoordinateStructures::Steps &steps);

    void move(CoordinateStructures::Direction direction);

    [[nodiscard]] inline CoordinateStructures::Pixel &getHeadPosition() { return headPosition; }

    [[nodiscard]] inline std::deque<CoordinateStructures::Pixel> &getBody() { return body; }


private:
    CoordinateStructures::Pixel headPosition;
    std::deque<CoordinateStructures::Pixel> body;
    CoordinateStructures::Steps steps;
    CoordinateStructures::Direction direction = CoordinateStructures::Direction::RIGHT;

};


#endif //SNAKE_SNAKE_HPP
