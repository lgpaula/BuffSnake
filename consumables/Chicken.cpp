#include "Chicken.hpp"

namespace Consumables {

    Chicken::Chicken(cv::Size size) : size(size) {
        displayDuration = std::make_unique<DurationNo>();
        type = ConsumableType::CHICKEN;
        points = 5;
        icon = cv::imread("icons/chicken.png", cv::IMREAD_UNCHANGED);
        resizeIcon();
        effect = Effect::FULL_GROWTH;
        id = 0;
    }

    void Chicken::setPosition(Helper::Pixel pixel) {
        position = pixel;
    }

    void Chicken::resizeIcon() {
        cv::resize(icon, icon, cv::Size(size.width, size.height));
    }

}