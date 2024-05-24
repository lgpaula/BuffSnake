#include <iostream>
#include "../include/Snake.hpp"
#include <algorithm>

Snake::Snake(CoordinateStructures::Pixel coords, const CoordinateStructures::Steps &steps, OnGameOver onGameOver) :
        headPosition(coords), steps(steps), onGameOver(std::move(onGameOver)) {
    body.emplace_front(headPosition.x - steps.cols, headPosition.y);
    body.emplace_front(body.front().x - steps.cols, headPosition.y);
}

bool Snake::changeDirection(CoordinateStructures::Direction dir) {
    const auto prevHead = headPosition;
    switch (dir) {
        case CoordinateStructures::Direction::UP:
            if (direction == CoordinateStructures::Direction::DOWN || direction == CoordinateStructures::Direction::UP) return false;
            headPosition.y -= steps.rows;
            break;
        case CoordinateStructures::DOWN:
            if (direction == CoordinateStructures::Direction::UP || direction == CoordinateStructures::Direction::DOWN) return false;
            headPosition.y += steps.rows;
            break;
        case CoordinateStructures::LEFT:
            if (direction == CoordinateStructures::Direction::RIGHT || direction == CoordinateStructures::Direction::LEFT) return false;
            headPosition.x -= steps.cols;
            break;
        case CoordinateStructures::RIGHT:
            if (direction == CoordinateStructures::Direction::LEFT || direction == CoordinateStructures::Direction::RIGHT) return false;
            headPosition.x += steps.cols;
            break;
    }

    body.push_back(prevHead);
    body.pop_front();
    direction = dir;

    checkCollision();

    return true;
}

void Snake::move() {
    const auto prevHead = headPosition;
    switch (direction) {
        case CoordinateStructures::Direction::UP:
            headPosition.y -= steps.rows;
            break;
        case CoordinateStructures::DOWN:
            headPosition.y += steps.rows;
            break;
        case CoordinateStructures::LEFT:
            headPosition.x -= steps.cols;
            break;
        case CoordinateStructures::RIGHT:
            headPosition.x += steps.cols;
            break;
    }

    body.push_back(prevHead);
    body.pop_front();

    checkCollision();
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
            rampageMode();
            break;
        case Food::Effect::TIME_SLOW:
            break;
    }
}

void Snake::rampageMode() {
    onRampage = true;
}

void Snake::grow() {
    body.push_front(body.front());
}

void Snake::checkCollision() {
    if (std::find(body.begin(), body.end(), headPosition) != body.end()) {
        if (onRampage) {
            onRampage = false;
            return;
        }
        onGameOver();
    }
}
