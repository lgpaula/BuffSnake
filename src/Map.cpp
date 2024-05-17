#include "../include/Map.hpp"

Map::Map(CoordinateStructures::Size dimension) : map(cv::Mat::zeros(dimension.height, dimension.width, CV_8UC3)) {}

CoordinateStructures::Pixel Map::getCenter() const {
    return CoordinateStructures::Pixel{map.rows / 2, map.cols / 2};
}
