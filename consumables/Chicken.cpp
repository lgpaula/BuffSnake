#include "Chicken.hpp"

namespace Consumables {

    Chicken::Chicken() {
        type = ConsumableType::CHICKEN;
        points = 5;
        icon = cv::imread("icons/chicken.png", cv::IMREAD_UNCHANGED);
        effect = Effect::FULL_GROWTH;
    }

}
