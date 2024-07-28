#ifndef SNAKE_GAMEMODE_HPP
#define SNAKE_GAMEMODE_HPP

#include "../include/Helper.hpp"
#include "../consumables/Consumable.hpp"
#include "../consumables/DragonBalls.hpp"

class Snake;

enum class Mode {
    SINGLE_PLAYER,
    MULTI_PLAYER_LOCAL,
    MULTI_PLAYER_LAN,
    MULTI_PLAYER_ONLINE
};

class GameMode {

public:
    using OnConsumableEaten = std::function<void(int points)>;
    using OnGameOverSP = std::function<void()>;
    using OnGameOverMP = std::function<void(int snakeId)>;
    using OnSteroidConsumed = std::function<void(Consumables::ConsumableType type, Consumables::PowerUpConsumed power)>;
    using OnConsumableConsumedMP = std::function<bool(Consumables::Effect effect, Consumables::PowerUpConsumed power, int snakeId)>;
    using OnLifeIncrease = std::function<int(int snakeId, int consumableId)>;
    using OnLifeDecrease = std::function<int(int snakeId)>;

    GameMode() = default;

    virtual ~GameMode() noexcept = default;

    virtual void updateSnake() = 0;

    virtual void updateMap() = 0;

    [[nodiscard]] virtual inline cv::Mat getMap() const = 0;

private:
    virtual void updateBackground() = 0;
    virtual void createBorder() = 0;
    virtual void updateBorder() = 0;
    virtual void fitToGrid(Helper::Pixel& pixel)  = 0;
    virtual void onKeyPressed(int key) = 0;
    virtual void checkOutOfBounds() = 0;
    virtual void setConsumablePosition(Consumables::Consumable& consumable) = 0;
    virtual void updateOccupiedSpaces() = 0;
    virtual void updateConsumables() = 0;
    [[nodiscard]] virtual Helper::Pixel generatePosition() = 0;
    virtual void spawnConsumableOverTime() = 0;
//    virtual void removeBorderInX(const Helper::Pixel &px) = 0;
//    virtual void removeBorderInY(const Helper::Pixel &px) = 0;
    virtual void updateGameTick() = 0;
    virtual void borderCollision() = 0;
    virtual void checkCollisionWithBody() = 0;
    virtual void spawnConsumable(const std::shared_ptr<Consumables::Consumable>& consumable) = 0;
    virtual void removeAlpha(cv::Mat& roi, const cv::Mat& icon) = 0;
    [[nodiscard]] virtual bool consumableAlreadyExists(Consumables::ConsumableType type) = 0;
//    virtual void removeCorners() = 0;
};





#endif //SNAKE_GAMEMODE_HPP
