#include "Genetics.hpp"

namespace Consumables {

    Genetics::Genetics(cv::Size size) : size(size) {
        displayDuration = std::make_unique<DurationNo>();
        type = ConsumableType::GENETICS;
        points = 0;
        icon = cv::imread("icons/genetics.png", cv::IMREAD_UNCHANGED);
        resizeIcon();
        effect = Effect::TIME_SLOW;
    }

    void Genetics::setPosition(CoordinateStructures::Pixel pixel) {
        position = pixel;
    }

    void Genetics::resizeIcon() {
        cv::resize(icon, icon, cv::Size(size.width, size.height));
    }

}
