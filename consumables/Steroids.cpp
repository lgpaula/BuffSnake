#include "Steroids.hpp"

namespace Consumables {

    Steroids::Steroids() {
        type = ConsumableType::STEROIDS;
        points = -50;
        icon = cv::imread("icons/steroids.png", cv::IMREAD_UNCHANGED);
        effect = Effect::RAMPAGE;
        displayDuration = 5000;
    }

    int Steroids::getDisplayDuration() {
        return displayDuration;
    }

}
