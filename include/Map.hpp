#ifndef SNAKE_MAP_HPP
#define SNAKE_MAP_HPP

#include <list>
#include <opencv2/core.hpp>
#include <thread>
#include <unordered_set>
#include <random>
#include <opencv2/imgcodecs.hpp>
#include "CoordinateStructures.hpp"
#include "../consumables/IConsumable.hpp"

class Snake;

class Map {

public:

    using OnConsumableEaten = std::function<void(Consumables::IConsumable consumable)>;
    using OnGameOver = std::function<void()>;

    Map(std::shared_ptr<Snake>& snake, CoordinateStructures::Size dimension,
        OnConsumableEaten onConsumableEaten, OnGameOver onGameOver);

    void updateSnake();

    void updateTimer();

    void updateMap();

    ~Map() noexcept = default;

    inline cv::Mat getMap() const { return map; }

private:
    void updateBackground();
    void createBorder();
    void updateBorder();
    static void fitToGrid(CoordinateStructures::Pixel& pixel) ;
    void onKeyPressed(int key);
    void checkOutOfBounds();
    void checkCollisionWithConsumable(CoordinateStructures::Pixel &head);
    void setConsumablePosition(Consumables::IConsumable &consumable);
    void updateOccupiedSpaces();
    void updateConsumables();
    static cv::Mat resizeIcon(const cv::Mat& icon);
    CoordinateStructures::Pixel generatePosition();
    void spawnConsumableOverTime();
    cv::Scalar randomize();
    void removeBorderInX(const CoordinateStructures::Pixel &head);
    void removeBorderInY(const CoordinateStructures::Pixel &head);
    void updateGameTick();
    void showPointsOnConsumable(const Consumables::IConsumable& consumable);
    void borderCollision();
    void onSnakeMove();
    void onConsumableCollision(const Consumables::IConsumable& consumable);
    void checkCollisionWithBody();
    void spawnConsumable(Consumables::IConsumable& consumable);
    static void removeAlpha(cv::Mat& roi, const cv::Mat& icon);

private:
    std::shared_ptr<Snake> snake;
    cv::Mat map;
    cv::Mat wall = cv::imread("icons/wall.png", cv::IMREAD_UNCHANGED);
    OnConsumableEaten onConsumableEaten;
    OnGameOver onGameOver;
    static const int pixelPerSquare = 25;
    std::list<CoordinateStructures::Pixel> border{};
    std::chrono::time_point<std::chrono::steady_clock> lastUpdate = std::chrono::steady_clock::now();
    std::unordered_set<Consumables::IConsumable> consumables;
    std::unordered_set<CoordinateStructures::Pixel> occupiedSpaces;
    std::mt19937 engine{std::random_device{}()};
    int consumablesEaten = 0;
    int timeToMove = 400;
    bool paused = false;
};


#endif //SNAKE_MAP_HPP
