#ifndef SNAKE_COORDINATESTRUCTURES_HPP
#define SNAKE_COORDINATESTRUCTURES_HPP

namespace CoordinateStructures {
    struct Pixel {
        int x;
        int y;

        Pixel() = default;
        Pixel(int x, int y) : x(x), y(y) {}

        bool operator==(const Pixel& other) const {
            return x == other.x && y == other.y;
        }
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

template <>
struct std::hash<CoordinateStructures::Pixel> {
    std::size_t operator()(const CoordinateStructures::Pixel& pixel) const noexcept {
        std::size_t h1 = std::hash<int>{}(pixel.x);
        std::size_t h2 = std::hash<int>{}(pixel.y);
        return h1 ^ (h2 << 1);
    }
};

#endif //SNAKE_COORDINATESTRUCTURES_HPP
