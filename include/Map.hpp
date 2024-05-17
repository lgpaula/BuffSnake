#ifndef SNAKE_MAP_HPP
#define SNAKE_MAP_HPP

#include <list>
#include <opencv2/core.hpp>
#include "CoordinateStructures.hpp"

class Map {

public:
    explicit Map(CoordinateStructures::Size coords);

    [[nodiscard]] CoordinateStructures::Pixel getCenter() const;

private:
    cv::Mat map;
    std::list<std::pair<int, int>> border;

};


#endif //SNAKE_MAP_HPP
