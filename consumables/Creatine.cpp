#include "Creatine.hpp"

namespace Consumables {

    Creatine::Creatine() {
        type = ConsumableType::CREATINE;
        points = 20;
        icon = cv::imread("icons/creatine.png", cv::IMREAD_UNCHANGED);
        effect = Effect::PARTIAL_GROWTH;
    }

}
