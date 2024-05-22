#include <iostream>
#include "../include/Snake.hpp"
#include <algorithm>

Snake::Snake(CoordinateStructures::Pixel coords, const CoordinateStructures::Steps &steps, OnGameOver onGameOver) :
        headPosition(coords), steps(steps), onGameOver(std::move(onGameOver)) {
    body.emplace_front(headPosition.x - steps.cols, headPosition.y);
    body.emplace_front(body.front().x - steps.cols, headPosition.y);
}

void Snake::changeDirection(CoordinateStructures::Direction dir) {
    const auto prevHead = headPosition;
    switch (dir) {
        case CoordinateStructures::Direction::UP:
            if (direction == CoordinateStructures::Direction::DOWN || direction == CoordinateStructures::Direction::UP) return;
            headPosition.y -= steps.rows;
            break;
        case CoordinateStructures::DOWN:
            if (direction == CoordinateStructures::Direction::UP || direction == CoordinateStructures::Direction::DOWN) return;
            headPosition.y += steps.rows;
            break;
        case CoordinateStructures::LEFT:
            if (direction == CoordinateStructures::Direction::RIGHT || direction == CoordinateStructures::Direction::LEFT) return;
            headPosition.x -= steps.cols;
            break;
        case CoordinateStructures::RIGHT:
            if (direction == CoordinateStructures::Direction::LEFT || direction == CoordinateStructures::Direction::RIGHT) return;
            headPosition.x += steps.cols;
            break;
    }

    body.push_back(prevHead);
    body.pop_front();
    direction = dir;

    checkCollision();
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

void Snake::grow() {
    body.push_front(body.front());
}

void Snake::checkCollision() {
    if (std::find(body.begin(), body.end(), headPosition) != body.end()) {
        onGameOver();
    }
}
