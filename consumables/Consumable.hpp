#ifndef SNAKE_CONSUMABLE_HPP
#define SNAKE_CONSUMABLE_HPP

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

    class Consumable {
    public:
        Consumable() = default;

        virtual ~Consumable() = default;

        [[nodiscard]] inline ConsumableType getType() const { return type; }

        [[nodiscard]] inline int getPoints() const { return points; }

        [[nodiscard]] inline const cv::Mat &getIcon() const { return icon; }

        [[nodiscard]] inline Effect getEffect() const { return effect; }

        [[nodiscard]] inline CoordinateStructures::Pixel getPosition() const { return position; }

        inline void setIcon(const cv::Mat &newIcon) { icon = newIcon; }

        inline void setPosition(const CoordinateStructures::Pixel &newPosition) { position = newPosition; }

        [[nodiscard]] virtual int getDisplayDuration() { return displayDuration; };

        [[nodiscard]] virtual int getEffectDuration() { return effectDuration; };

        bool operator==(const Consumable& other) const {
            return type == other.type;
        }

    protected:
        ConsumableType type{};
        int points{};
        cv::Mat icon{};
        Effect effect{};
        int displayDuration{};
        int effectDuration{};
        CoordinateStructures::Pixel position{};
    };

}

template <>
struct std::hash<Consumables::Consumable> {
    std::size_t operator()(const Consumables::Consumable& c) const {
        using std::hash;
        using std::size_t;
        using std::string;

        size_t res = 17;
        res = res * 31 + hash<int>()(static_cast<int>(c.getType()));
        return res;
    }
};

#endif //SNAKE_CONSUMABLE_HPP
