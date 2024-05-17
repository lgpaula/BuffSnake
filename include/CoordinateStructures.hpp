#ifndef SNAKE_COORDINATESTRUCTURES_HPP
#define SNAKE_COORDINATESTRUCTURES_HPP

namespace CoordinateStructures {
    struct Pixel {
        int x;
        int y;
    };

    struct Size {
        int height;
        int width;
    };

    enum Direction {
        Up, Down, Left, Right
    };

}


#endif //SNAKE_COORDINATESTRUCTURES_HPP
