#include "Preworkout.hpp"

namespace Consumables {

    Preworkout::Preworkout(cv::Size size) : size(size) {
        displayDuration = std::make_unique<DurationYes>();
        type = ConsumableType::PREWORKOUT;
        points = 0;
        icon = cv::imread("icons/preworkout.png", cv::IMREAD_UNCHANGED);
        resizeIcon();
        effect = Effect::SPEED;
        displayDuration->setDuration(duration);
        id = 0;
    }

    void Preworkout::setPosition(Helper::Pixel pixel) {
        position = pixel;
    }

    void Preworkout::resizeIcon() {
        cv::resize(icon, icon, cv::Size(size.width, size.height));
    }

}
