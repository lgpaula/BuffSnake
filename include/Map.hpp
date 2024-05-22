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

    [[nodiscard]] CoordinateStructures::Pixel getCenter() const;

    [[nodiscard]] inline CoordinateStructures::Steps getSteps() const { return steps; }

    void updateSnake(Snake& snake);

    void updateTimer();

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
    void resizeIcons();
    CoordinateStructures::Pixel generatePosition();

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
    mutable std::mt19937 engine{std::random_device{}()};

    cv::Mat chickenLogo = cv::imread("../icons/chicken.png", cv::IMREAD_COLOR);
    cv::Mat proteinLogo = cv::imread("../icons/protein.png", cv::IMREAD_COLOR);
    cv::Mat creatineLogo = cv::imread("../icons/creatine.png", cv::IMREAD_COLOR);
    cv::Mat steroidsLogo = cv::imread("../icons/steroids.png", cv::IMREAD_COLOR);
};


#endif //SNAKE_MAP_HPP
