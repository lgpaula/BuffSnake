#include "Creatine.hpp"

namespace Consumables {

    Creatine::Creatine(cv::Size size) : size(size)  {
        displayDuration = std::make_unique<DurationYes>();
        type = ConsumableType::CREATINE;
        points = 20;
        icon = cv::imread("icons/creatine.png", cv::IMREAD_UNCHANGED);
        resizeIcon();
        effect = Effect::PARTIAL_GROWTH;
        displayDuration->setDuration(duration);
    }

    void Creatine::setPosition(CoordinateStructures::Pixel pixel) {
        position = pixel;
    }

    void Creatine::resizeIcon() {
        cv::resize(icon, icon, cv::Size(size.width, size.height));
    }


}
