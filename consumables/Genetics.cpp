#include "Genetics.hpp"

namespace Consumables {

    Genetics::Genetics() {
        type = ConsumableType::GENETICS;
        points = 0;
        icon = cv::imread("icons/genetics.png", cv::IMREAD_UNCHANGED);
        effect = Effect::TIME_SLOW;
    }

}
