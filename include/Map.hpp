#ifndef SNAKE_MAP_HPP
#define SNAKE_MAP_HPP

#include <list>
#include <opencv2/core.hpp>
#include "CoordinateStructures.hpp"

class Snake;

class Map {

public:
    explicit Map(CoordinateStructures::Size coords);

    void show();

    [[nodiscard]] CoordinateStructures::Pixel getCenter() const;

    [[nodiscard]] inline CoordinateStructures::Steps getSteps() const { return steps; }

    void addSnake(Snake& snake);

private:
    void addBackground();
    void createBorder();
    void updateBorder();
    void fitToGrid(CoordinateStructures::Pixel &pixel) const;


private:
    cv::Mat map;
    CoordinateStructures::Steps steps{};
    std::list<std::pair<CoordinateStructures::Pixel, CoordinateStructures::Pixel>> border{};

};


#endif //SNAKE_MAP_HPP
