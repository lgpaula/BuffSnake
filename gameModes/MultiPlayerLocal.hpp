#ifndef SNAKE_MULTIPLAYERLOCAL_HPP
#define SNAKE_MULTIPLAYERLOCAL_HPP

#include <unordered_set>
#include <random>
#include <list>
#include "GameMode.hpp"
#include <memory>
#include <set>

class MultiPlayerLocal : public GameMode {

public:
    MultiPlayerLocal(int amountOfPlayers, Helper::Size dimension, OnGameOverMP onGameOver, OnConsumableConsumedMP onConsumableConsumed,
                     OnLifeIncrease onLifeIncrease, OnLifeDecrease onLifeDecrease);

    ~MultiPlayerLocal() override;

    void updateSnake() override;

    void updateMap() override;

    inline cv::Mat getMap() const override { return map; }

private:
    void createSnakes();
    void initialize();
    void instantiateStartingPositions(Helper::Size dimension);
    void updateBackground() override;
    void createBorder() override;
    void updateBorder() override;
    void fitToGrid(Helper::Pixel &pixel) override;
    void onKeyPressed(int key) override;
    void checkOutOfBounds() override;
    void checkCollisionWithConsumable(const std::shared_ptr<Snake>& snake);
    void setConsumablePosition(Consumables::Consumable &consumable) override;
    void updateOccupiedSpaces() override;
    void updateConsumables() override;
    Helper::Pixel generatePosition() override;
    void spawnConsumableOverTime() override;
    void removeBorderInX(const Helper::Pixel &px) override;
    void removeBorderInY(const Helper::Pixel &px) override;
    void updateGameTick() override;
    void borderCollision() override;
    void onSnakeMove(const std::shared_ptr<Snake>& snake);
    void onConsumableCollision(const std::shared_ptr<Consumables::Consumable> &consumable, const std::shared_ptr<Snake>& snake);
    void checkCollisionWithBody() override;
    void spawnConsumable(const std::shared_ptr<Consumables::Consumable> &consumable) override;
    void removeAlpha(cv::Mat &roi, const cv::Mat &icon) override;
    bool consumableAlreadyExists(Consumables::ConsumableType type) override;
    void removeCorners() override;
    void generateKeyMappings();
    void checkCollisionWithOtherSnake();
    cv::Scalar randomize();
    void checkDeadSnakes();

private:
    int amountOfPlayers;
    OnGameOverMP onGameOver;
    OnConsumableConsumedMP onConsumableConsumed;
    OnLifeIncrease onLifeIncrease;
    OnLifeDecrease onLifeDecrease;
    std::vector<Helper::Pixel> startingPositions = {};
    std::unordered_set<std::shared_ptr<Snake>> snakes;
    cv::Mat map;
    cv::Mat wall = cv::imread("icons/wall.png", cv::IMREAD_UNCHANGED);
    OnConsumableEaten onConsumableEaten;
    static const int pixelPerSquare = 25;
    cv::Size iconSize = cv::Size(pixelPerSquare - 4, pixelPerSquare - 4);
    std::list<Helper::Pixel> border{};
    std::unordered_set<std::shared_ptr<Consumables::Consumable>> consumables;
    std::unordered_set<Helper::Pixel> occupiedSpaces;
    std::mt19937 engine{std::random_device{}()};
    int consumablesEaten = 0;
    bool paused = false;
    std::vector<Helper::Pixel> corners = {};
    static const int amountOfRandomWalls = 30;
    std::vector<Helper::Pixel> randomWalls = {};
    std::set<int> deadSnakes;
    std::unordered_map<std::shared_ptr<Snake>, std::unordered_map<int, Helper::Direction>> movementKeyMappings;
    std::unordered_map<std::shared_ptr<Snake>, std::unordered_map<int, Consumables::Effect>> effectKeyMappings;
};

#endif //SNAKE_MULTIPLAYERLOCAL_HPP
