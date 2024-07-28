#ifndef SNAKE_SNAKE_HPP
#define SNAKE_SNAKE_HPP

#include <queue>
#include <functional>
#include <random>
#include <unordered_set>
#include "Helper.hpp"
#include "../consumables/Consumable.hpp"
#include "../consumables/DragonBalls.hpp"

class Snake {

public:
    explicit Snake(Helper::Pixel coords, Helper::RGB color = Helper::BLUE,
                   Helper::Direction direction = Helper::Direction::RIGHT, int id = 0);

    bool changeDirection(Helper::Direction direction);

    void move();

    void grow();

    void applyEffect(Consumables::Effect effect);

    void setOnSteroids(bool value);

    void setHeadPosition(Helper::Pixel newPos);

    void setTimeToMove(int time);

    void clampTick();

    void revokeEffect(Consumables::Effect effect);

    void startEffectThread(Consumables::Effect effect);

    void setLastUpdate(std::chrono::time_point<std::chrono::steady_clock> time);

    void createInitialBody();

    [[nodiscard]] inline Helper::Pixel &getHeadPosition() { return headPosition; }

    [[nodiscard]] inline std::deque<Helper::Pixel> &getBody() { return body; }

    [[nodiscard]] inline Helper::Direction getDirection() { return direction; }

    [[nodiscard]] inline int getTimeToMove() const { return timeToMove; }

    [[nodiscard]] inline Helper::RGB getColor() const { return color; }

    [[nodiscard]] inline int getId() const { return id; }

    [[nodiscard]] inline std::chrono::time_point<std::chrono::steady_clock> getLastUpdate() const { return lastUpdate; }

    inline bool isOnSteroids() const { return onSteroids; };

private:
    Helper::Pixel headPosition;
    std::deque<Helper::Pixel> body;
    Helper::Direction direction = Helper::Direction::RIGHT;
    int id;
    mutable std::mt19937 engine{std::random_device{}()};
    bool onSteroids = false;
    int timeToMove = 400;
    Helper::RGB color;
    std::chrono::time_point<std::chrono::steady_clock> lastUpdate = std::chrono::steady_clock::now();
};

template <>
struct std::hash<Snake> {
    std::size_t operator()(const Snake& snake) const {
        using std::hash;
        using std::size_t;

        size_t res = 17;
        res = res * 31 + hash<int>()(static_cast<int>(snake.getId() + 1));
        return res;
    }
};


#endif //SNAKE_SNAKE_HPP
