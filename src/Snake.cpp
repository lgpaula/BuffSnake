
#include "../include/Snake.hpp"

Snake::Snake(CoordinateStructures::Pixel coords) : headPosition({coords.x, coords.y}) {}

void Snake::move(CoordinateStructures::Heading direction) {
    heading = direction;
}
