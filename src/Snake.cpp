#include "../include/Snake.hpp"
#include <algorithm>

Snake::Snake(CoordinateStructures::Pixel coords) : headPosition(coords) {
    body.emplace_front(headPosition.x - 1, headPosition.y);
    body.emplace_front(body.front().x - 1, headPosition.y);
}

bool Snake::changeDirection(CoordinateStructures::Direction dir) {
    const auto prevHead = headPosition;
    switch (dir) {
        case CoordinateStructures::Direction::UP:
            if (direction == CoordinateStructures::Direction::DOWN || direction == CoordinateStructures::Direction::UP) return false;
            --headPosition.y;
            break;
        case CoordinateStructures::DOWN:
            if (direction == CoordinateStructures::Direction::UP || direction == CoordinateStructures::Direction::DOWN) return false;
            ++headPosition.y;
            break;
        case CoordinateStructures::LEFT:
            if (direction == CoordinateStructures::Direction::RIGHT || direction == CoordinateStructures::Direction::LEFT) return false;
            --headPosition.x;
            break;
        case CoordinateStructures::RIGHT:
            if (direction == CoordinateStructures::Direction::LEFT || direction == CoordinateStructures::Direction::RIGHT) return false;
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
        case CoordinateStructures::Direction::UP:
            --headPosition.y;
            break;
        case CoordinateStructures::DOWN:
            ++headPosition.y;
            break;
        case CoordinateStructures::LEFT:
            --headPosition.x;
            break;
        case CoordinateStructures::RIGHT:
            ++headPosition.x;
            break;
    }

    body.push_back(prevHead);
    body.pop_front();
}

void Snake::applyEffect(Food::Effect effect) {
    switch (effect) {
        case Food::Effect::FULL_GROWTH:
            grow();
            break;
        case Food::Effect::PARTIAL_GROWTH: {
            std::uniform_int_distribution<> chance(1, 4);
            if (chance(engine) != 1) grow();
            break;
        }
        case Food::Effect::RAMPAGE:
            setOnSteroids(true);
            break;
        case Food::Effect::TIME_SLOW:
            break;
    }
}

void Snake::setOnSteroids(bool value) {
    onSteroids = value;
}

void Snake::grow() {
    body.push_front(body.front());
}

void Snake::setHeadPosition(CoordinateStructures::Pixel newPos) {
    headPosition = newPos;
}
