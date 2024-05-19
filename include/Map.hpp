#ifndef SNAKE_MAP_HPP
#define SNAKE_MAP_HPP

#include <list>
#include <opencv2/core.hpp>
#include <thread>
#include <unordered_set>
#include <random>
#include "CoordinateStructures.hpp"
#include "IConsumables.hpp"

class Snake;

class Map {

public:

    using OnDirectionChange = std::function<void(CoordinateStructures::Direction input)>;
    using OnConsumableEaten = std::function<void(Food::Consumable consumable)>;

    Map(CoordinateStructures::Size dimension, const OnDirectionChange& onDirectionChange, const OnConsumableEaten& onConsumableEaten);

    [[nodiscard]] CoordinateStructures::Pixel getCenter() const;

    [[nodiscard]] inline CoordinateStructures::Steps getSteps() const { return steps; }

    void updateSnake(Snake& snake);

    void spawnConsumable(Food::Consumable consumable);

    ~Map() noexcept;

private:
    void addBackground();
    void createBorder();
    void updateBorder();
    void fitToGrid(CoordinateStructures::Pixel &pixel) const;
    void onKeyPressed(int key);
    void checkCollisionWithBorder(CoordinateStructures::Pixel &head) const;
    void checkCollisionWithConsumable(CoordinateStructures::Pixel &head);
    void setConsumablePosition(Food::Consumable &consumable);
    void updateOccupiedSpaces(Snake &snake);
    void updateConsumables();
    CoordinateStructures::Pixel generatePosition();

private:
    cv::Mat map;
    OnDirectionChange onDirectionChange;
    OnConsumableEaten onConsumableEaten;
    CoordinateStructures::Steps steps{};
    std::list<std::pair<CoordinateStructures::Pixel, CoordinateStructures::Pixel>> border{};
    std::thread displayThread{};
    std::unordered_set<Food::Consumable> consumables;
    std::unordered_set<CoordinateStructures::Pixel> occupiedSpaces;
    mutable std::mt19937 engine{std::random_device{}()};
    //empty space

};


#endif //SNAKE_MAP_HPP
