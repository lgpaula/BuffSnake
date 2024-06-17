#ifndef SNAKE_CONSUMABLES_HPP
#define SNAKE_CONSUMABLES_HPP

#include "CoordinateStructures.hpp"
#include <iostream>
#include <opencv2/highgui.hpp>

namespace Food {

    enum ConsumableType{
        CHICKEN,
        PROTEIN,
        CREATINE,
        STEROIDS,
        GENETICS
    };

    enum Effect {
        FULL_GROWTH,
        PARTIAL_GROWTH,
        RAMPAGE,
        TIME_SLOW
    };

    struct Consumable {
        explicit Consumable(ConsumableType type) : type(type) {
            setVars();
        }

        ConsumableType type{};
        int points{};
        cv::Mat icon;
        Effect effect{};
        CoordinateStructures::Pixel position{-1, -1};
        //todo add effect duration
        //todo add display duration

        void setVars() {
            switch (type) {
                case CHICKEN:
                    points = 5;
                    icon = cv::imread("icons/chicken.png", cv::IMREAD_UNCHANGED);
                    effect = Effect::FULL_GROWTH;
                    break;
                case PROTEIN:
                    points = 20;
                    icon = cv::imread("icons/protein.png", cv::IMREAD_UNCHANGED);
                    effect = Effect::FULL_GROWTH;
                    break;
                case CREATINE:
                    points = 20;
                    icon = cv::imread("icons/creatine.png", cv::IMREAD_UNCHANGED);
                    effect = Effect::PARTIAL_GROWTH;
                    break;
                case STEROIDS:
                    points = -50;
                    icon = cv::imread("icons/steroids.png", cv::IMREAD_UNCHANGED);
                    effect = Effect::RAMPAGE;
                    break;
                case GENETICS:
                    points = 0;
                    icon = cv::imread("icons/genetics.png", cv::IMREAD_UNCHANGED);
                    effect = Effect::TIME_SLOW;
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
