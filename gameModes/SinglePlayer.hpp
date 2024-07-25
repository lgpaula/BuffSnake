#ifndef SNAKE_SINGLEPLAYER_HPP
#define SNAKE_SINGLEPLAYER_HPP

#include <list>
#include <opencv2/core.hpp>
#include <unordered_set>
#include <random>
#include "../include/Helper.hpp"
#include "../consumables/Consumable.hpp"
#include "GameMode.hpp"

class SinglePlayer : public GameMode {

public:
    SinglePlayer(Helper::Size dimension,
                 OnConsumableEaten onConsumableEaten, OnGameOverSP onGameOver, OnSteroidConsumed onSteroidConsumed);

    void updateSnake() override;

    void updateMap() override;

    ~SinglePlayer() override;

    inline cv::Mat getMap() const override { return map; }

private:
    void initialize();
    void updateBackground() override;
    void createBorder() override;
    void updateBorder() override;
    void fitToGrid(Helper::Pixel& pixel)  override;
    void onKeyPressed(int key) override;
    void checkOutOfBounds() override;
    void checkCollisionWithConsumable();
    void setConsumablePosition(Consumables::Consumable& consumable) override;
    void updateOccupiedSpaces() override;
    void updateConsumables() override;
    Helper::Pixel generatePosition() override;
    void spawnConsumableOverTime() override;
    cv::Scalar randomize();
    void removeBorderInX(const Helper::Pixel &px) override;
    void removeBorderInY(const Helper::Pixel &px) override;
    void updateGameTick() override;
    void showPointsOnConsumable(const std::shared_ptr<Consumables::Consumable>& consumable);
    void borderCollision() override;
    void onSnakeMove();
    void onConsumableCollision(const std::shared_ptr<Consumables::Consumable>& consumable);
    void checkCollisionWithBody() override;
    void spawnConsumable(const std::shared_ptr<Consumables::Consumable>& consumable) override;
    void removeAlpha(cv::Mat& roi, const cv::Mat& icon) override;
    bool consumableAlreadyExists(Consumables::ConsumableType type) override;
    void removeCorners() override;

private:
    std::shared_ptr<Snake> snake;
    cv::Mat map;
    cv::Mat wall = cv::imread("icons/wall.png", cv::IMREAD_UNCHANGED);
    OnConsumableEaten onConsumableEaten;
    OnGameOverSP onGameOver;
    OnSteroidConsumed onSteroidConsumed;
    static const int pixelPerSquare = 25;
    cv::Size iconSize = cv::Size(pixelPerSquare - 4, pixelPerSquare - 4);
    std::list<Helper::Pixel> border{};
    std::unordered_set<std::shared_ptr<Consumables::Consumable>> consumables;
    std::unordered_set<Helper::Pixel> occupiedSpaces;
    std::mt19937 engine{std::random_device{}()};
    int consumablesEaten = 0;
    bool paused = false;
    int steroidsStored = 0;
    std::vector<Helper::Pixel> corners = {};
};


#endif //SNAKE_SINGLEPLAYER_HPP
