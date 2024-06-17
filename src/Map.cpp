#include "../include/Map.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <utility>
#include "../include/Snake.hpp"
#include <chrono>

Map::Map(std::shared_ptr<Snake>& snake, CoordinateStructures::Size dimension,
         Map::OnConsumableEaten consumableEaten, Map::OnGameOver onGameOver) :
         snake(std::move(snake)), map(cv::Mat::zeros(dimension.height, dimension.width, CV_8UC3)),
                  onConsumableEaten(std::move(consumableEaten)), onGameOver(std::move(onGameOver)) {

    steps.cols = map.cols / 20;
    steps.rows = map.rows / 20;

    updateBackground();
    createBorder();

    updateMap();
    updateSnake();
    Food::Consumable consumable = Food::Consumable{Food::ConsumableType::CHICKEN};
    spawnConsumable(consumable);
}

void Map::updateMap() {
    int key = cv::waitKey(1);
    onKeyPressed(key);
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate).count();
    if (elapsed >= timeToMove) {
        onSnakeMove();
        lastUpdate = now;
    }
}

void Map::onSnakeMove() {
    snake->move();
    updateSnake();
}

void Map::updateTimer() {
    lastUpdate = std::chrono::steady_clock::now();
}

void Map::onKeyPressed(int key) {
    CoordinateStructures::Direction input = snake->getDirection();
    switch (key) {
        case 27:
            cv::destroyAllWindows();
            exit(0);
        case 82:
            input = CoordinateStructures::Direction::UP;
            break;
        case 84:
            input = CoordinateStructures::Direction::DOWN;
            break;
        case 81:
            input = CoordinateStructures::Direction::LEFT;
            break;
        case 83:
            input = CoordinateStructures::Direction::RIGHT;
            break;
        default:
            break;
    }
    if (snake->changeDirection(input)) {
        updateSnake();
        updateTimer();
    }
}

void Map::createBorder() {
    for (int i = 0; i < map.cols; i += steps.cols) {
        CoordinateStructures::Pixel p1 = {i, 0};
        CoordinateStructures::Pixel p2 = {i + steps.cols - 1, 0};
        border.emplace_back(p1, p2);
    }

    for (int i = 0; i < map.cols; i += steps.cols) {
        CoordinateStructures::Pixel p1 = {i, map.rows - 1};
        CoordinateStructures::Pixel p2 = {i + steps.cols - 1, map.rows - 1};
        border.emplace_back(p1, p2);
    }

    for (int i = 0; i < map.rows; i += steps.rows) {
        CoordinateStructures::Pixel p1 = {0, i};
        CoordinateStructures::Pixel p2 = {0, i + steps.rows - 1};
        border.emplace_back(p1, p2);
    }

    for (int i = 0; i < map.rows; i += steps.rows) {
        CoordinateStructures::Pixel p1 = {map.cols - 1, i};
        CoordinateStructures::Pixel p2 = {map.cols - 1, i + steps.rows - 1};
        border.emplace_back(p1, p2);
    }

    updateBorder();
}

void Map::updateBorder() {
    for (const auto &b: border)
        cv::line(map, cv::Point(b.first.x, b.first.y), cv::Point(b.second.x, b.second.y), cv::Scalar(0, 0, 255), 2);
}

void Map::updateBackground() {
    int iCounter = 0;
    int jCounter = 0;

    for (int i = 0; i < map.cols; i += steps.cols) {
        for (int j = 0; j < map.rows; j += steps.rows) {
            cv::Point p1 = cv::Point(i, j);
            cv::Point p2 = cv::Point(i + steps.cols, j + steps.rows);
            if ((iCounter + jCounter) % 2 == 0)
                cv::rectangle(map, p1, p2, cv::Scalar(81, 215, 170), -1);
            else
                cv::rectangle(map, p1, p2, cv::Scalar(73, 209, 162), -1);
            ++jCounter;
        }
        ++iCounter;
    }
}

void Map::fitToGrid(CoordinateStructures::Pixel &pixel) const {
    pixel.x = pixel.x - (pixel.x % steps.cols);
    pixel.y = pixel.y - (pixel.y % steps.rows);
}

cv::Scalar Map::randomize() {
    std::uniform_int_distribution<> r(0, 255);
    std::uniform_int_distribution<> g(0, 255);
    std::uniform_int_distribution<> b(0, 255);
    cv::Scalar out = cv::Scalar(b(engine), g(engine), r(engine));
    return out;
}

void Map::updateSnake() {
    updateBackground();
    updateBorder();
    updateConsumables();

    auto head = snake->getHeadPosition();
    fitToGrid(head);
    cv::Point h1 = cv::Point{head.x + 2, head.y + 2};
    cv::Point h2 = cv::Point{head.x + steps.cols - 3, head.y + steps.rows - 3};
    cv::Scalar headColor = snake->isOnSteroids() ? randomize() : cv::Scalar(255, 0, 0);
    cv::rectangle(map, h1, h2, headColor, -1);

    for (auto& b : snake->getBody()) {
        fitToGrid(b);
        cv::Point b1 = cv::Point{b.x + 5, b.y + 5};
        cv::Point b2 = cv::Point{b.x + steps.cols - 6, b.y + steps.rows - 6};
        cv::Scalar bodyColor = snake->isOnSteroids() ? randomize() : cv::Scalar(255, 0, 0);
        cv::rectangle(map, b1, b2, bodyColor, -1);
    }

    checkCollisionWithBody();
    checkCollisionWithBorder();
    checkCollisionWithConsumable(head);
    updateOccupiedSpaces();
}

void removeAlpha(cv::Mat& roi, const cv::Mat& icon) {
    std::vector<cv::Mat> channels;
    cv::split(icon, channels);
    cv::Mat alpha = channels[3];
    channels.pop_back();
    cv::Mat bgr;
    cv::merge(channels, bgr);
    cv::Mat mask;
    cv::cvtColor(alpha, mask, cv::COLOR_GRAY2BGR);
    cv::Mat blended;
    cv::multiply(bgr, mask, blended, 1.0 / 255);
    cv::multiply(roi, cv::Scalar::all(255) - mask, roi, 1.0 / 255);
    cv::add(roi, blended, roi);
}

void Map::updateConsumables() {
    for (const Food::Consumable& c : consumables) {
        cv::Point point = cv::Point{c.position.x, c.position.y};
        cv::Mat roi = map(cv::Rect(point.x + 2, point.y + 2, c.icon.cols, c.icon.rows));
        removeAlpha(roi, c.icon);
    }
}

void Map::updateOccupiedSpaces() {
    occupiedSpaces.clear();
    occupiedSpaces.insert(snake->getHeadPosition());
    for (const auto &b : snake->getBody()) occupiedSpaces.insert(b);
    for (const auto &c : consumables) occupiedSpaces.insert(c.position);
}

void clampTick(int &timeToMove) {
    timeToMove -= 3;
    timeToMove = std::clamp(timeToMove, 20, 400);
}

void Map::updateGameTick() {
    timeToMove -= 3;
    clampTick(timeToMove);
}

void Map::showPointsOnConsumable(const Food::Consumable& consumable) {
    cv::Point point = cv::Point{consumable.position.x, consumable.position.y};
    cv::putText(map, "+" + std::to_string(consumable.points), point, cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 2);
}

void Map::onConsumableCollision(const Food::Consumable& consumable) {
    showPointsOnConsumable(consumable);

    if (consumable.type == Food::ConsumableType::GENETICS) {
        std::uniform_int_distribution<> chance(1, 3);
        if (chance(engine) == 1) {
            timeToMove *= 2;
            clampTick(timeToMove);
        }
    }

    onConsumableEaten(consumable);
    snake->applyEffect(consumable.effect);
    consumables.erase(consumable);

    ++consumablesEaten;
    spawnConsumableOverTime();
    updateGameTick();
}

void Map::checkCollisionWithConsumable(CoordinateStructures::Pixel &head) {
    for (const auto &c : consumables) {
        if (head.x == c.position.x && head.y == c.position.y) {
            onConsumableCollision(c);
            break;
        }
    }
}

void Map::removeBorderInX(const CoordinateStructures::Pixel &head) {
    for (auto it = border.begin(); it != border.end(); )
        (it->first.y == head.y && it->second.y != head.y) ? it = border.erase(it) : ++it;
}

void Map::removeBorderInY(const CoordinateStructures::Pixel &head) {
    for (auto it = border.begin(); it != border.end(); )
        (it->first.x == head.x && it->second.x != head.x) ? it = border.erase(it) : ++it;
}

bool Map::borderCollision() {
    for (const auto &b : border) {
        if (snake->getHeadPosition().x == b.first.x && snake->getHeadPosition().y == b.first.y) return true;
        if (snake->getHeadPosition().x == b.second.x && snake->getHeadPosition().y == b.second.y) return true;
    }

    return false;
}

void Map::checkCollisionWithBody() {
    auto it = std::find(snake->getBody().begin(), snake->getBody().end(), snake->getHeadPosition());
    if (it != snake->getBody().end()) {
        if (snake->isOnSteroids()) {
            snake->getBody().erase(snake->getBody().begin(), ++it);
            snake->setOnSteroids(false);
            return;
        }
        onGameOver();
    }
}

void Map::checkCollisionWithBorder() {
    auto head = snake->getHeadPosition();

    if (!snake->isOnSteroids() && borderCollision()) onGameOver();

    if (head.x < 0) {
        removeBorderInX(head);
        snake->setHeadPosition({map.cols - steps.cols, head.y});
        updateSnake();
        snake->setOnSteroids(false);
    }
    if (head.x > map.cols - steps.cols) {
        removeBorderInX(head);
        snake->setHeadPosition({0, head.y});
        updateSnake();
        snake->setOnSteroids(false);
    }
    if (head.y < 0) {
        removeBorderInY(head);
        snake->setHeadPosition({head.x, map.rows - steps.rows});
        updateSnake();
        snake->setOnSteroids(false);
    }
    if (head.y > map.rows - steps.rows) {
        removeBorderInY(head);
        snake->setHeadPosition({head.x, 0});
        updateSnake();
        snake->setOnSteroids(false);
    }
}

void Map::spawnConsumableOverTime() {
    if (consumablesEaten % 1 == 0) {
        auto newConsumable = Food::Consumable{Food::ConsumableType::CHICKEN};
        spawnConsumable(newConsumable);
    }

    if (consumablesEaten % 8 == 0) {
        std::uniform_int_distribution<> chance(0, 1);
        if (chance(engine)) {
            auto newConsumable = Food::Consumable{Food::ConsumableType::PROTEIN};
            spawnConsumable(newConsumable);
        } else {
            auto newConsumable = Food::Consumable{Food::ConsumableType::CREATINE};
            spawnConsumable(newConsumable);
        }
    }

    if (consumablesEaten % 15 == 0) {
        auto newConsumable = Food::Consumable{Food::ConsumableType::STEROIDS};
        spawnConsumable(newConsumable);
    }

    if (consumablesEaten % 20 == 0) {
        auto newConsumable = Food::Consumable{Food::ConsumableType::GENETICS};
        spawnConsumable(newConsumable);
    }
}

void Map::spawnConsumable(Food::Consumable& consumable) {
    resizeIcon(consumable);
    setConsumablePosition(consumable);

    occupiedSpaces.insert(consumable.position);
    fitToGrid(consumable.position);
    consumables.insert(consumable);
    updateConsumables();
}

void Map::setConsumablePosition(Food::Consumable &consumable) {
    CoordinateStructures::Pixel pos = generatePosition();

    while (std::find(occupiedSpaces.begin(), occupiedSpaces.end(), pos) != occupiedSpaces.end()) {
        pos = generatePosition();
    }

    consumable.position = pos;
}

CoordinateStructures::Pixel Map::generatePosition() {
    std::uniform_int_distribution<> x(0, (map.cols)); // -1
    std::uniform_int_distribution<> y(0, (map.rows)); // -1

    CoordinateStructures::Pixel pos = {x(engine), y(engine)};

    fitToGrid(pos);
    return pos;
}

void Map::resizeIcon(Food::Consumable& consumable) const {
    cv::resize(consumable.icon, consumable.icon, cv::Size(steps.cols - 4, steps.rows - 4));
}

Map::~Map() noexcept {
    cv::destroyAllWindows();
    if (displayThread.joinable()) displayThread.join();
}
