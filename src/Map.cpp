#include "../include/Map.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <utility>
#include "../include/Snake.hpp"
#include <chrono>

Map::Map(CoordinateStructures::Size dimension, Map::OnDirectionChange onDirectionChange,
         Map::OnConsumableEaten consumableEaten, Map::OnGameOver onGameOver, Map::OnSnakeMove onSnakeMove) :
         map(cv::Mat::zeros(dimension.height, dimension.width, CV_8UC3)), onDirectionChange(std::move(onDirectionChange)),
                  onConsumableEaten(std::move(consumableEaten)), onGameOver(std::move(onGameOver)), onSnakeMove(onSnakeMove) {
    steps.cols = map.cols / 20;
    steps.rows = map.rows / 20;

    addBackground();
    createBorder();

    lastUpdate = std::chrono::steady_clock::now();

    displayThread = std::thread([this, &onSnakeMove]() {
        while (true) {
            cv::imshow("Map", map);
            int key = cv::waitKey(1);
            onKeyPressed(key);

            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate).count();
            if (elapsed >= timeToMove) {
                onSnakeMove();
                lastUpdate = now;
            }
        }
    });
}

void Map::updateTimer() {
    lastUpdate = std::chrono::steady_clock::now();
}

void Map::onKeyPressed(int key) {
    switch (key) {
        case 27:
            cv::destroyAllWindows();
            break;
        case 82:
            onDirectionChange(CoordinateStructures::Direction::UP);
            break;
        case 84:
            onDirectionChange(CoordinateStructures::Direction::DOWN);
            break;
        case 81:
            onDirectionChange(CoordinateStructures::Direction::LEFT);
            break;
        case 83:
            onDirectionChange(CoordinateStructures::Direction::RIGHT);
            break;
        default:
            break;
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
    //add callback for border update (when broken);
    for (const auto &b: border)
        cv::line(map, cv::Point(b.first.x, b.first.y), cv::Point(b.second.x, b.second.y), cv::Scalar(0, 0, 255), 2);
}

void Map::addBackground() {
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

void Map::updateSnake(Snake &snake) {
    addBackground();
    updateBorder();
    updateConsumables();

    auto head = snake.getHeadPosition();
    fitToGrid(head);
    cv::Point h1 = cv::Point{head.x + 2, head.y + 2};
    cv::Point h2 = cv::Point{head.x + steps.cols - 3, head.y + steps.rows - 3};
    cv::rectangle(map, h1, h2, cv::Scalar(255, 0, 0), -1);

    for (auto& b : snake.getBody()) {
        fitToGrid(b);
        cv::Point b1 = cv::Point{b.x + 5, b.y + 5};
        cv::Point b2 = cv::Point{b.x + steps.cols - 6, b.y + steps.rows - 6};
        cv::rectangle(map, b1, b2, cv::Scalar(255, 0, 0), -1);
    }

    checkCollisionWithBorder(head);
    checkCollisionWithConsumable(head);
    updateOccupiedSpaces(snake);
}

void Map::updateConsumables() {
    for (Food::Consumable c : consumables) {
        cv::Point point = cv::Point{c.position.x, c.position.y};
        cv::Mat roi = map(cv::Rect(point.x, point.y, c.icon.cols, c.icon.rows));
        cv::Mat mask;

        if (c.icon.channels() == 4) {
            cv::Mat channels[4];
            cv::split(c.icon, channels);
            cv::Mat transparentAlpha = cv::Mat::zeros(channels[3].size(), channels[3].type());
            cv::Mat rgb[3] = {channels[0], channels[1], channels[2]};
            cv::merge(rgb, 3, c.icon);
            mask = transparentAlpha;
        }

        mask.empty() ? c.icon.copyTo(roi) : c.icon.copyTo(roi, mask);
    }
}

void Map::updateOccupiedSpaces(Snake &snake) {
    occupiedSpaces.clear();
    occupiedSpaces.insert(snake.getHeadPosition());
    for (const auto &b : snake.getBody()) occupiedSpaces.insert(b);
    for (const auto &c : consumables) occupiedSpaces.insert(c.position);
}

void Map::checkCollisionWithConsumable(CoordinateStructures::Pixel &head) {
    for (const auto &c : consumables) {
        if (head.x == c.position.x && head.y == c.position.y) {
            Food::Consumable newConsumable {c.type};
            if (c.type == Food::ConsumableType::GENETICS) timeToMove *= 2;
            consumables.erase(c);
            onConsumableEaten(newConsumable);
            break;
        }
    }
}

void Map::checkCollisionWithBorder(CoordinateStructures::Pixel &head) const {
    if (head.x < 0 || head.x >= map.cols || head.y < 0 || head.y >= map.rows) {
        onGameOver();
    }
}

void Map::spawnConsumableOverTime() {
    if (consumablesSpawned % 10 == 0) {
        std::uniform_int_distribution<> chance(0, 1);
        chance(engine) ? spawnConsumable(Food::Consumable{Food::ConsumableType::PROTEIN}) :
                        spawnConsumable(Food::Consumable{Food::ConsumableType::CREATINE});
    }

    if (consumablesSpawned % 25 == 0) {
        spawnConsumable(Food::Consumable{Food::ConsumableType::STEROIDS});
    }

    if (consumablesSpawned % 30 == 0) {
        std::uniform_int_distribution<> chance(1, 3);
        if (chance(engine) == 1) spawnConsumable(Food::Consumable{Food::ConsumableType::GENETICS});
    }
}

void Map::spawnConsumable(Food::Consumable consumable) {
    ++consumablesSpawned;
    spawnConsumableOverTime();

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
    std::uniform_int_distribution<> x(0, (map.cols));
    std::uniform_int_distribution<> y(0, (map.rows));

    CoordinateStructures::Pixel pos = {x(engine), y(engine)};

    fitToGrid(pos);
    return pos;
}

void Map::resizeIcon(Food::Consumable& consumable) const {
    cv::resize(consumable.icon, consumable.icon, cv::Size(steps.cols, steps.rows));
}

Map::~Map() noexcept {
    if (displayThread.joinable()) displayThread.join();
}
