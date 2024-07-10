#include "../include/Map.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <utility>
#include "../include/Snake.hpp"
#include <chrono>
#include "../consumables/Chicken.hpp"
#include "../consumables/Protein.hpp"
#include "../consumables/Creatine.hpp"
#include "../consumables/Steroids.hpp"
#include "../consumables/Genetics.hpp"

Map::Map(std::shared_ptr<Snake>& snake, CoordinateStructures::Size dimension,
         Map::OnConsumableEaten consumableEaten, Map::OnGameOver onGameOver) :
         snake(std::move(snake)), onConsumableEaten(std::move(consumableEaten)), onGameOver(std::move(onGameOver)) {

    map = cv::Mat::zeros(dimension.height * pixelPerSquare, dimension.width * pixelPerSquare, CV_8UC3);

    wall = resizeIcon(wall);

    createBorder();
    updateBackground();

    updateMap();
    updateSnake();
    auto consumable = Consumables::Chicken();
    spawnConsumable(consumable);
}

void Map::updateMap() {
    int key = cv::waitKey(1);
    onKeyPressed(key);
    if (paused) return;
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
            if (paused) return;
            input = CoordinateStructures::Direction::UP;
            break;
        case 84:
            if (paused) return;
            input = CoordinateStructures::Direction::DOWN;
            break;
        case 81:
            if (paused) return;
            input = CoordinateStructures::Direction::LEFT;
            break;
        case 83:
            if (paused) return;
            input = CoordinateStructures::Direction::RIGHT;
            break;
        case 32:
            paused = !paused;
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
    for (int i = 0; i < map.cols; i += pixelPerSquare) {
        border.emplace_back(i, 0);
        border.emplace_back(i, map.rows - pixelPerSquare);
    }
    for (int i = 0; i < map.rows; i += pixelPerSquare) {
        border.emplace_back(0, i);
        border.emplace_back(map.cols - pixelPerSquare, i);
    }

    updateBorder();
}

void Map::updateBorder() {
    for (const auto &b: border) {
        cv::Point point = cv::Point{b.x, b.y};
        cv::Mat roi = map(cv::Rect(point.x + 2, point.y + 2, wall.cols, wall.rows));
        removeAlpha(roi, wall);
    }
}

void Map::updateBackground() {
    int iCounter = 0;
    int jCounter = 0;

    for (int i = 0; i < map.cols; i += pixelPerSquare) {
        for (int j = 0; j < map.rows; j += pixelPerSquare) {
            cv::Point p1 = cv::Point(i, j);
            cv::Point p2 = cv::Point(i + pixelPerSquare - 1, j + pixelPerSquare - 1);
            cv::Scalar color = (iCounter + jCounter) % 2 == 0 ? cv::Scalar(81, 215, 170) : cv::Scalar(73, 209, 162);
            cv::rectangle(map, p1, p2, color, -1);
            ++jCounter;
        }
        ++iCounter;
        jCounter = 0;
    }

    updateBorder();
    updateConsumables();
}

cv::Scalar Map::randomize() {
    std::uniform_int_distribution<> r(0, 255);
    std::uniform_int_distribution<> g(0, 255);
    std::uniform_int_distribution<> b(0, 255);
    cv::Scalar out = cv::Scalar(b(engine), g(engine), r(engine));
    return out;
}

void Map::updateSnake() { //NOLINT
    updateBackground();

    auto head = snake->getHeadPosition() * pixelPerSquare;

    cv::Point h1 = cv::Point{(head.x) + 2, (head.y) + 2};
    cv::Point h2 = cv::Point{(head.x + pixelPerSquare) - 3, (head.y + pixelPerSquare) - 3};
    cv::Scalar headColor = snake->isOnSteroids() ? randomize() : cv::Scalar(255, 0, 0);
    cv::rectangle(map, h1, h2, headColor, -1);

    for (auto b : snake->getBody()) {
        b *= pixelPerSquare;
        cv::Point b1 = cv::Point{(b.x) + 5, (b.y) + 5};
        cv::Point b2 = cv::Point{(b.x + pixelPerSquare) - 6, (b.y + pixelPerSquare) - 6};
        cv::Scalar bodyColor = snake->isOnSteroids() ? randomize() : cv::Scalar(255, 0, 0);
        cv::rectangle(map, b1, b2, bodyColor, -1);
    }

    checkCollisionWithBody();
    borderCollision();
    checkOutOfBounds();
    checkCollisionWithConsumable(head);
    updateOccupiedSpaces();
}

void Map::removeAlpha(cv::Mat& roi, const cv::Mat& icon) {
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
    for (const Consumables::Consumable& c : consumables) {
        cv::Point point = cv::Point{c.getPosition().x, c.getPosition().y};
        cv::Mat roi = map(cv::Rect(point.x + 2, point.y + 2, c.getIcon().cols, c.getIcon().rows));
        removeAlpha(roi, c.getIcon());
    }
}

void Map::updateOccupiedSpaces() {
    occupiedSpaces.clear();
    occupiedSpaces.insert(snake->getHeadPosition());
    for (const auto &b : snake->getBody()) occupiedSpaces.insert(b);
    for (const auto &c : consumables) occupiedSpaces.insert(c.getPosition());
    for (const auto &b : border) occupiedSpaces.insert(b);
}

void clampTick(int &timeToMove) {
    timeToMove -= 3;
    timeToMove = std::clamp(timeToMove, 20, 400);
}

void Map::updateGameTick() {
    timeToMove -= 3;
    clampTick(timeToMove);
}

void Map::showPointsOnConsumable(const Consumables::Consumable &consumable) {
    if (consumable.getType() == Consumables::ConsumableType::STEROIDS ||
        consumable.getType() == Consumables::ConsumableType::GENETICS) return;
    cv::Point position = cv::Point{consumable.getPosition().x, consumable.getPosition().y};
    cv::putText(map, "+" + std::to_string(consumable.getPoints()), position, cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 2);
}

void Map::onConsumableCollision(const Consumables::Consumable& consumable) {
    showPointsOnConsumable(consumable);

    if (consumable.getType() == Consumables::Genetics().getType()) {
        std::uniform_int_distribution<> chance(1, 3);
        if (chance(engine) == 1) {
            timeToMove *= 2;
            clampTick(timeToMove);
        }
    }

    onConsumableEaten(consumable);
    snake->applyEffect(consumable.getEffect());
    consumables.erase(consumable);

    ++consumablesEaten;
    spawnConsumableOverTime();
    updateGameTick();
}

void Map::checkCollisionWithConsumable(CoordinateStructures::Pixel &head) {
    for (const auto &c : consumables) {
        if (head.x == c.getPosition().x && head.y == c.getPosition().y) {
            onConsumableCollision(c);
            break;
        }
    }
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

void Map::removeBorderInX(const CoordinateStructures::Pixel &head) {
    for (auto it = border.begin(); it != border.end();)
        (it->y == head.y) ? it = border.erase(it) : ++it;

    updateBorder();
}

void Map::removeBorderInY(const CoordinateStructures::Pixel &head) {
    for (auto it = border.begin(); it != border.end();)
        (it->x == head.x) ? it = border.erase(it) : ++it;

    updateBorder();
}

void Map::borderCollision() {
    auto head = snake->getHeadPosition() * pixelPerSquare;

    for (const auto &b : border) {
        if (head == b) {
            if (!snake->isOnSteroids()) {
                onGameOver();
                return;
            }
            if (head.x == 0 || head.x == map.cols - pixelPerSquare) removeBorderInX(head);
            if (head.y == 0 || head.y == map.rows - pixelPerSquare) removeBorderInY(head);
            snake->setOnSteroids(false);
            updateOccupiedSpaces();
            return;
        }
    }
}

void Map::checkOutOfBounds() { //NOLINT
    auto head = snake->getHeadPosition() * pixelPerSquare;

    if (head.x < 0) {
        snake->setHeadPosition({(map.cols - pixelPerSquare) / pixelPerSquare, head.y / pixelPerSquare});
        updateSnake();
    }
    if (head.x > map.cols - pixelPerSquare) {
        snake->setHeadPosition({0, head.y / pixelPerSquare});
        updateSnake();
    }
    if (head.y < 0) {
        snake->setHeadPosition({head.x / pixelPerSquare, (map.rows - pixelPerSquare) / pixelPerSquare});
        updateSnake();
    }
    if (head.y > map.rows - pixelPerSquare) {
        snake->setHeadPosition({head.x / pixelPerSquare, 0});
        updateSnake();
    }
}

void Map::spawnConsumableOverTime() {
    if (consumablesEaten % 1 == 0) {
        auto newConsumable = Consumables::Chicken();
        spawnConsumable(newConsumable);
    }

    if (consumablesEaten % 8 == 0) {
        std::uniform_int_distribution<> chance(0, 1);
        if (chance(engine)) {
            auto newConsumable = Consumables::Protein();
            spawnConsumable(newConsumable);
        } else {
            auto newConsumable = Consumables::Creatine();
            spawnConsumable(newConsumable);
        }
    }

    if (consumablesEaten % 2 == 0) {
        auto newConsumable = Consumables::Steroids();
        spawnConsumableWithDuration(newConsumable);
    }

    if (consumablesEaten % 20 == 0) {
        auto newConsumable = Consumables::Genetics();
        spawnConsumable(newConsumable);
    }
}

void Map::spawnConsumableWithDuration(Consumables::Consumable& consumable) {
    spawnConsumable(consumable);

    auto timeLeft = consumable.getDisplayDuration();
    std::thread timer([timeLeft, this, consumable]() mutable {

        while (timeLeft > 0) {
            timeLeft -= 1000;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        consumables.erase(consumable);
        updateConsumables();

    });

    timer.detach();
}

void Map::spawnConsumable(Consumables::Consumable& consumable) {
    consumable.setIcon(resizeIcon(consumable.getIcon()));
    setConsumablePosition(consumable);

    occupiedSpaces.insert(consumable.getPosition());
    consumables.insert(consumable);
    updateConsumables();
}

void Map::setConsumablePosition(Consumables::Consumable& consumable) {
    CoordinateStructures::Pixel pos = generatePosition();

    while (std::find(occupiedSpaces.begin(), occupiedSpaces.end(), pos) != occupiedSpaces.end()) {
        pos = generatePosition();
    }

    consumable.setPosition(pos);
}

void Map::fitToGrid(CoordinateStructures::Pixel &pixel) {
    pixel.x -= (pixel.x % pixelPerSquare);
    pixel.y -= (pixel.y % pixelPerSquare);
}

CoordinateStructures::Pixel Map::generatePosition() {
    std::uniform_int_distribution<> x(0, (map.cols - 1));
    std::uniform_int_distribution<> y(0, (map.rows - 1));

    CoordinateStructures::Pixel pos = {x(engine), y(engine)};

    fitToGrid(pos);
    return pos;
}

cv::Mat Map::resizeIcon(const cv::Mat& icon) {
    cv::Mat resizedIcon;
    cv::resize(icon, resizedIcon, cv::Size(pixelPerSquare - 4, pixelPerSquare - 4));
    return resizedIcon;
}