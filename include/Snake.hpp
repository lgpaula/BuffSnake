#ifndef SNAKE_SNAKE_HPP
#define SNAKE_SNAKE_HPP

#include "CoordinateStructures.hpp"

class Snake {

public:
    explicit Snake(CoordinateStructures::Pixel coords);

    void move(CoordinateStructures::Direction direction);


private:
    CoordinateStructures::Pixel headPosition;
    CoordinateStructures::Direction direction;

};


#endif //SNAKE_SNAKE_HPP
