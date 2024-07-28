#include "../include/Snake.hpp"
#include <algorithm>

Snake::Snake(Helper::Pixel coords, Helper::RGB color, Helper::Direction direction, int id) :
        headPosition(coords), color(color), direction(direction), id(id) {

    createInitialBody();
}

void Snake::createInitialBody() {
    switch (direction) {
        case Helper::RIGHT:
            body.emplace_front(headPosition.x - 1, headPosition.y);
            body.emplace_front(body.front().x - 1, headPosition.y);
            break;
        case Helper::UP:
            body.emplace_front(headPosition.x, headPosition.y + 1);
            body.emplace_front(headPosition.x, body.front().y + 1);
            break;
        case Helper::DOWN:
            body.emplace_front(headPosition.x, headPosition.y - 1);
            body.emplace_front(headPosition.x, body.front().y - 1);
            break;
        case Helper::LEFT:
            body.emplace_front(headPosition.x + 1, headPosition.y);
            body.emplace_front(body.front().x + 1, headPosition.y);
            break;
    }
}

bool Snake::changeDirection(Helper::Direction dir) {
    const auto prevHead = headPosition;
    switch (dir) {
        case Helper::Direction::UP:
            if (direction == Helper::Direction::DOWN || direction == Helper::Direction::UP) return false;
            --headPosition.y;
            break;
        case Helper::DOWN:
            if (direction == Helper::Direction::UP || direction == Helper::Direction::DOWN) return false;
            ++headPosition.y;
            break;
        case Helper::LEFT:
            if (direction == Helper::Direction::RIGHT || direction == Helper::Direction::LEFT) return false;
            --headPosition.x;
            break;
        case Helper::RIGHT:
            if (direction == Helper::Direction::LEFT || direction == Helper::Direction::RIGHT) return false;
            ++headPosition.x;
            break;
    }

    body.push_back(prevHead);
    body.pop_front();
    direction = dir;

    return true;
}

void Snake::move() {
    const auto prevHead = headPosition;
    switch (direction) {
        case Helper::Direction::UP:
            --headPosition.y;
            break;
        case Helper::DOWN:
            ++headPosition.y;
            break;
        case Helper::LEFT:
            --headPosition.x;
            break;
        case Helper::RIGHT:
            ++headPosition.x;
            break;
    }

    body.push_back(prevHead);
    body.pop_front();
}

void Snake::applyEffect(Consumables::Effect effect) {
    switch (effect) {
        case Consumables::Effect::FULL_GROWTH:
            grow();
            break;
        case Consumables::Effect::PARTIAL_GROWTH: {
            std::uniform_int_distribution<> chance(1, 4);
            if (chance(engine) != 1) grow();
            break;
        }
        case Consumables::SPEED:
            timeToMove -= 100;
            startEffectThread(effect);
            break;
        case Consumables::RAMPAGE:
            setOnSteroids(true);
            startEffectThread(effect);
            break;
        case Consumables::TIME_SLOW:
        case Consumables::NONE:
            break;
    }
}

void Snake::startEffectThread(Consumables::Effect effect) {
    std::thread effectThread([this, effect]() {
        auto now = std::chrono::steady_clock::now();

        while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - now).count() < 5) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        revokeEffect(effect);
    });

    effectThread.detach();
}

void Snake::revokeEffect(Consumables::Effect effect) {
    switch (effect) {
        case Consumables::Effect::FULL_GROWTH:
        case Consumables::Effect::PARTIAL_GROWTH:
        case Consumables::SPEED:
            timeToMove += 100;
            break;
        case Consumables::RAMPAGE:
            setOnSteroids(false);
            break;
        case Consumables::TIME_SLOW:
        case Consumables::NONE:
            break;
    }
}

void Snake::setOnSteroids(bool value) {
    onSteroids = value;
}

void Snake::grow() {
    body.push_front(body.front());
}

void Snake::setHeadPosition(Helper::Pixel newPos) {
    headPosition = newPos;
}

void Snake::clampTick() {
    timeToMove = std::clamp(timeToMove, 50, 400);
}

void Snake::setTimeToMove(int time) {
    timeToMove = time;
    clampTick();
}

void Snake::setLastUpdate(std::chrono::time_point<std::chrono::steady_clock> time) {
    lastUpdate = time;
}