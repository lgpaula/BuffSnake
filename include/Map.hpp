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

    using OnConsumableEaten = std::function<void(Food::Consumable consumable)>;
    using OnGameOver = std::function<void()>;

    Map(std::shared_ptr<Snake>& snake, CoordinateStructures::Size dimension,
        OnConsumableEaten onConsumableEaten, OnGameOver onGameOver);

    void updateSnake();

    void updateTimer();

    void spawnConsumable(Food::Consumable consumable);

    cv::Mat getMap() const;

    void updateMap();

    ~Map() noexcept;

private:
    void updateBackground();
    void createBorder();
    void updateBorder();
    void fitToGrid(CoordinateStructures::Pixel &pixel) const;
    void onKeyPressed(int key);
    void checkCollisionWithBorder();
    void checkCollisionWithConsumable(CoordinateStructures::Pixel &head);
    void setConsumablePosition(Food::Consumable &consumable);
    void updateOccupiedSpaces();
    void updateConsumables();
    void resizeIcon(Food::Consumable& consumable) const;
    CoordinateStructures::Pixel generatePosition();
    void spawnConsumableOverTime();
    cv::Scalar randomize();
    void removeBorderInX(const CoordinateStructures::Pixel &head);
    void removeBorderInY(const CoordinateStructures::Pixel &head);
    void updateGameTick();
    void showPointsOnConsumable(const Food::Consumable& consumable);
    bool borderCollision();
    void onSnakeMove();
    void onConsumableCollision(const Food::Consumable& consumable);
    void checkCollisionWithBody();

private:
    std::shared_ptr<Snake> snake;
    cv::Mat map;
    OnConsumableEaten onConsumableEaten;
    OnGameOver onGameOver;
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
