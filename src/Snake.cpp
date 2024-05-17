#include "../include/Snake.hpp"

Snake::Snake(CoordinateStructures::Pixel coords, const CoordinateStructures::Steps &steps) : headPosition({coords.x, coords.y}) {
    body.push_front({headPosition.x - steps.cols, headPosition.y});
    body.push_front({body.front().x - steps.cols, headPosition.y});
}

void Snake::move(CoordinateStructures::Direction dir) {
    direction = dir;
}
