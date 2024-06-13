#ifndef SNAKE_MAP_HPP
#define SNAKE_MAP_HPP

#include <list>
#include <opencv2/core.hpp>
#include <thread>
#include <unordered_set>
#include <random>
#include <opencv2/imgcodecs.hpp>
#include "CoordinateStructures.hpp"
#include "IConsumables.hpp"

class Snake;

class Map {

public:

    using OnDirectionChange = std::function<void(CoordinateStructures::Direction input)>;
    using OnConsumableEaten = std::function<void(Food::Consumable consumable)>;
    using OnGameOver = std::function<void()>;
    using OnSnakeMove = std::function<void()>;

    Map(CoordinateStructures::Size dimension, OnDirectionChange onDirectionChange,
        OnConsumableEaten onConsumableEaten, OnGameOver onGameOver, OnSnakeMove onSnakeMove);

    void updateSnake(Snake& snake);

    void updateTimer();

    void spawnConsumable(Food::Consumable consumable);

    void updatePoints(int points);

    ~Map() noexcept;

private:
    void updateBackground();
    void createBorder();
    void updateBorder();
    void fitToGrid(CoordinateStructures::Pixel &pixel) const;
    void onKeyPressed(int key);
    void checkCollisionWithBorder(Snake &head);
    void checkCollisionWithConsumable(CoordinateStructures::Pixel &head);
    void setConsumablePosition(Food::Consumable &consumable);
    void updateOccupiedSpaces(Snake &snake);
    void updateConsumables();
    void resizeIcon(Food::Consumable& consumable) const;
    CoordinateStructures::Pixel generatePosition();
    void spawnConsumableOverTime();
    cv::Scalar randomize();
    void removeBorderInX(const CoordinateStructures::Pixel &head);
    void removeBorderInY(const CoordinateStructures::Pixel &head);
    void updateGameTick();
    void showPointsOnConsumable(const Food::Consumable& consumable);
    bool borderCollision(Snake snake);

private:
    cv::Mat map;
    OnDirectionChange onDirectionChange;
    OnConsumableEaten onConsumableEaten;
    OnGameOver onGameOver;
    OnSnakeMove onSnakeMove;
    CoordinateStructures::Steps steps{};
    std::list<std::pair<CoordinateStructures::Pixel, CoordinateStructures::Pixel>> border{};
    std::chrono::time_point<std::chrono::steady_clock> lastUpdate;
    std::thread displayThread{};
    std::unordered_set<Food::Consumable> consumables;
    std::unordered_set<CoordinateStructures::Pixel> occupiedSpaces;
    std::mt19937 engine{std::random_device{}()};
    int consumablesEaten = 0;
    int timeToMove = 400;
    int currentPoints = 0;
};


#endif //SNAKE_MAP_HPP
