#ifndef SNAKE_ICONSUMABLE_HPP
#define SNAKE_ICONSUMABLE_HPP

#include <iostream>
#include <opencv2/highgui.hpp>
#include "../include/CoordinateStructures.hpp"

namespace Consumables {
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

    class IConsumable {
    public:
        IConsumable() = default;

        virtual ~IConsumable() = default;

        [[nodiscard]] inline ConsumableType getType() const { return type; }

        [[nodiscard]] inline int getPoints() const { return points; }

        [[nodiscard]] inline const cv::Mat &getIcon() const { return icon; }

        [[nodiscard]] inline Effect getEffect() const { return effect; }

        [[nodiscard]] inline CoordinateStructures::Pixel getPosition() const { return position; }

        void setIcon(const cv::Mat &newIcon) {
            IConsumable::icon = newIcon;
        }

        void setPosition(const CoordinateStructures::Pixel &newPosition) {
            IConsumable::position = newPosition;
        }

        bool operator==(const IConsumable& other) const {
            return type == other.type;
        }

    protected:
        ConsumableType type{};
        int points{};
        cv::Mat icon{};
        Effect effect{};
        CoordinateStructures::Pixel position{};
    };

}

template <>
struct std::hash<Consumables::IConsumable> {
    std::size_t operator()(const Consumables::IConsumable& c) const {
        using std::hash;
        using std::size_t;
        using std::string;

        size_t res = 17;
        res = res * 31 + hash<int>()(static_cast<int>(c.getType()));
        return res;
    }
};

#endif //SNAKE_ICONSUMABLE_HPP
