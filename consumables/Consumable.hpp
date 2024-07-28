#ifndef SNAKE_CONSUMABLE_HPP
#define SNAKE_CONSUMABLE_HPP

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <thread>
#include "../include/Helper.hpp"
#include "Time.hpp"

namespace Consumables {
    enum ConsumableType{
        CHICKEN,
        PREWORKOUT,
        CREATINE,
        STEROIDS,
        GENETICS,
        DRAGONBALLS
    };

    enum Effect {
        FULL_GROWTH,
        PARTIAL_GROWTH,
        SPEED,
        RAMPAGE,
        TIME_SLOW,
        NONE
    };

    enum PowerUpConsumed {
        STORED,
        USED
    };

    class Consumable : public std::enable_shared_from_this<Consumable> {
    public:
        Consumable() = default;

        virtual ~Consumable() = default;

        [[nodiscard]] virtual ConsumableType getType() const = 0;

        [[nodiscard]] virtual int getPoints() const = 0;

        [[nodiscard]] virtual const cv::Mat &getIcon() const = 0;

        [[nodiscard]] virtual Effect getEffect() const = 0;

        [[nodiscard]] virtual Helper::Pixel getPosition() const = 0;

        [[nodiscard]] virtual int getId() const = 0;

        [[nodiscard]] virtual const std::shared_ptr<ITime>& getDisplayDuration() const = 0;

        virtual void setPosition(Helper::Pixel) = 0;
    };
}

template <>
struct std::hash<Consumables::Consumable> {
    std::size_t operator()(const Consumables::Consumable& c) const {
        using std::hash;
        using std::size_t;

        size_t res = 17;
        res = res * 31 + hash<int>()(static_cast<int>(c.getType()));
        return res;
    }
};

#endif //SNAKE_CONSUMABLE_HPP
