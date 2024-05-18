#ifndef SNAKE_MAP_HPP
#define SNAKE_MAP_HPP

#include <list>
#include <opencv2/core.hpp>
#include <thread>
#include "CoordinateStructures.hpp"

class Snake;

class Map {

public:

    using OnDirectionChange = std::function<void(CoordinateStructures::Direction input)>;

    Map(CoordinateStructures::Size dimension, const OnDirectionChange& onDirectionChange);

    [[nodiscard]] CoordinateStructures::Pixel getCenter() const;

    [[nodiscard]] inline CoordinateStructures::Steps getSteps() const { return steps; }

    void updateSnake(Snake& snake);

    ~Map() noexcept;

private:
    void addBackground();
    void createBorder();
    void updateBorder();
    void fitToGrid(CoordinateStructures::Pixel &pixel) const;
    void onKeyPressed(int key);

private:
    cv::Mat map;
    OnDirectionChange onDirectionChange;
    CoordinateStructures::Steps steps{};
    std::list<std::pair<CoordinateStructures::Pixel, CoordinateStructures::Pixel>> border{};
    std::thread displayThread;


};


#endif //SNAKE_MAP_HPP
