#include "Protein.hpp"

namespace Consumables {

    Protein::Protein() {
        type = ConsumableType::PROTEIN;
        points = 20;
        icon = cv::imread("icons/protein.png", cv::IMREAD_UNCHANGED);
        effect = Effect::FULL_GROWTH;
    }

}
