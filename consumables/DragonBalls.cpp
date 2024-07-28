#include "DragonBalls.hpp"

namespace Consumables {

    DragonBalls::DragonBalls(cv::Size size) : size(size) {
        displayDuration = std::make_unique<DurationYes>();
        type = ConsumableType::DRAGONBALLS;
        points = 0;
        effect = Effect::NONE;
        displayDuration->setDuration(duration);
        setId();
        setIcon();
        resizeIcon();
    }

    void DragonBalls::setId() {
        id = std::rand() % 7 + 1; // NOLINT(cert-msc50-cpp)
    }

    void DragonBalls::setIcon() {
        icon = cv::imread("icons/db" + std::to_string(id) + ".png", cv::IMREAD_UNCHANGED);
    }

    void DragonBalls::setPosition(Helper::Pixel pixel) {
        position = pixel;
    }

    void DragonBalls::resizeIcon() {
        cv::resize(icon, icon, cv::Size(size.width, size.height));
    }
}