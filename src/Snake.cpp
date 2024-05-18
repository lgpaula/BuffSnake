#include <iostream>
#include "../include/Snake.hpp"
#include <algorithm>

Snake::Snake(CoordinateStructures::Pixel coords, const CoordinateStructures::Steps &steps) :
        headPosition({coords.x, coords.y}), steps(steps) {
    body.push_front({headPosition.x - steps.cols, headPosition.y});
    body.push_front({body.front().x - steps.cols, headPosition.y});
    body.push_front({body.front().x - steps.cols, headPosition.y});
    body.push_front({body.front().x - steps.cols, headPosition.y});
}

void Snake::move(CoordinateStructures::Direction dir) {
    const auto prevHead = headPosition;
    switch (dir) {
        case CoordinateStructures::Direction::UP:
            if (direction == CoordinateStructures::Direction::DOWN) return;
            headPosition.y -= steps.rows;
            break;
        case CoordinateStructures::DOWN:
            if (direction == CoordinateStructures::Direction::UP) return;
            headPosition.y += steps.rows;
            break;
        case CoordinateStructures::LEFT:
            if (direction == CoordinateStructures::Direction::RIGHT) return;
            headPosition.x -= steps.cols;
            break;
        case CoordinateStructures::RIGHT:
            if (direction == CoordinateStructures::Direction::LEFT) return;
            headPosition.x += steps.cols;
            break;
    }

    body.push_back(prevHead);
    body.pop_front();
    direction = dir;

    checkCollision();
}

void Snake::checkCollision() {
    if (std::find(body.begin(), body.end(), headPosition) != body.end()) {
        std::cout << "Game Over!" << std::endl;
        //add callback for restart
    }
}
