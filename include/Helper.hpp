#ifndef SNAKE_HELPER_HPP
#define SNAKE_HELPER_HPP

#include <iostream>
#include <opencv2/core.hpp>

namespace Helper {

    struct Size {
        int width;
        int height;

        Size operator*(int multiplier) const {
            return {height * multiplier, width * multiplier};
        }
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

        Pixel operator*(int multiplier) const {
            return {x * multiplier, y * multiplier};
        }

        void operator*=(int multiplier) {
            x *= multiplier;
            y *= multiplier;
        }
    };

    enum Direction {
        RIGHT,
        DOWN,
        UP,
        LEFT
    };

    struct RGB {
        int b, g, r;

        [[nodiscard]] cv::Scalar toScalar() const {
            cv::Scalar scalar = cv::Scalar(b, g, r);
            return scalar;
        }
    };

    const RGB BLUE = {255, 0, 0};
    const RGB GREEN = {0, 255, 0};
    const RGB RED = {0, 0, 255};
    const RGB YELLOW = {0, 255, 255};
    const RGB COLORS[] = { BLUE, GREEN, RED, YELLOW };

}

template <>
struct std::hash<Helper::Pixel> {
    std::size_t operator()(const Helper::Pixel& pixel) const noexcept {
        std::size_t h1 = std::hash<int>{}(pixel.x);
        std::size_t h2 = std::hash<int>{}(pixel.y);
        return h1 ^ (h2 << 1);
    }
};

#endif //SNAKE_HELPER_HPP
