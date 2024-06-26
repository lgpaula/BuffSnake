#ifndef SNAKE_COORDINATESTRUCTURES_HPP
#define SNAKE_COORDINATESTRUCTURES_HPP

#include <iostream>

namespace CoordinateStructures {

    struct Size {
        int height;
        int width;
    };

    struct Pixel {
        int x;
        int y;

        Pixel() = default;
        Pixel(int x, int y) : x(x), y(y) {}
        explicit Pixel(Size size) : x(size.width), y(size.height) {}

        friend std::ostream& operator<<(std::ostream& os, const Pixel& pixel) {
            os << "x: " << pixel.x << " y: " << pixel.y;
            return os;
        }

        bool operator==(const Pixel& other) const {
            return x == other.x && y == other.y;
        }

        Pixel operator*(int steps) const {
            return {x * steps, y * steps};
        }

        void operator*=(int steps) {
            x *= steps;
            y *= steps;
        }
    };

    enum Direction {
        UP, DOWN, LEFT, RIGHT
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
