#include "Steroids.hpp"

namespace Consumables {

    Steroids::Steroids(cv::Size size) : size(size) {
        displayDuration = std::make_unique<DurationYes>();
        type = ConsumableType::STEROIDS;
        points = -50;
        icon = cv::imread("icons/steroids.png", cv::IMREAD_UNCHANGED);
        resizeIcon();
        effect = Effect::RAMPAGE;
        displayDuration->setDuration(duration);
    }

    void Steroids::setPosition(CoordinateStructures::Pixel pixel) {
        position = pixel;
    }

    void Steroids::resizeIcon() {
        cv::resize(icon, icon, cv::Size(size.width, size.height));
    }

}