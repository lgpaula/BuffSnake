#include "Protein.hpp"

namespace Consumables {

    Protein::Protein(cv::Size size) : size(size) {
        displayDuration = std::make_unique<DurationYes>();
        type = ConsumableType::PROTEIN;
        points = 20;
        icon = cv::imread("icons/protein.png", cv::IMREAD_UNCHANGED);
        resizeIcon();
        effect = Effect::FULL_GROWTH;
        displayDuration->setDuration(duration);
    }

    void Protein::setPosition(CoordinateStructures::Pixel pixel) {
        position = pixel;
    }

    void Protein::resizeIcon() {
        cv::resize(icon, icon, cv::Size(size.width, size.height));
    }

}
