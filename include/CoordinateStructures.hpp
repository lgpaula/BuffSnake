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
        UP, DOWN, LEFT, RIGHT
    };

    struct Steps {
        int rows;
        int cols;
    };

}


#endif //SNAKE_COORDINATESTRUCTURES_HPP
