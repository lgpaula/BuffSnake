#ifndef SNAKE_CONSUMABLES_HPP
#define SNAKE_CONSUMABLES_HPP

#include "CoordinateStructures.hpp"
#include <iostream>

namespace Food {

    enum ConsumableType{
        CHICKEN,
        PROTEIN,
        CREATINE,
        STEROIDS,
        GENETICS
    };

    struct Consumable {
        explicit Consumable(ConsumableType type) : type(type) {
            setVars();
        }

        ConsumableType type{};
        CoordinateStructures::Pixel position{};
        int points{};
        cv::Mat icon;

        void setVars() {
            switch (type) {
                case CHICKEN:
                    points = 5;
                    icon = cv::imread("../icons/chicken.png", cv::IMREAD_COLOR);
                    break;
                case PROTEIN:
                    points = 20;
                    icon = cv::imread("../icons/protein.png", cv::IMREAD_COLOR);
                    break;
                case CREATINE:
                    points = 20;
                    icon = cv::imread("../icons/creatine.png", cv::IMREAD_COLOR);
                    break;
                case STEROIDS:
                    points = -10;
                    icon = cv::imread("../icons/steroids.png", cv::IMREAD_COLOR);
                    break;
                case GENETICS:
                    points = 0;
                    icon = cv::imread("../icons/genetics.png", cv::IMREAD_COLOR);
                    //slows down the snake
                    break;
                default:
                    break;
            }
        }

        bool operator==(const Consumable& other) const {
            return type == other.type;
        }

    };

}

template <>
struct std::hash<Food::Consumable> {
    std::size_t operator()(const Food::Consumable& c) const {
        using std::hash;
        using std::size_t;
        using std::string;

        size_t res = 17;
        res = res * 31 + hash<int>()(static_cast<int>(c.type));
        return res;
    }
};

#endif //SNAKE_CONSUMABLES_HPP
