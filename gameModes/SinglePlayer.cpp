#include "SinglePlayer.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <utility>
#include "../include/Snake.hpp"
#include <chrono>
#include "../consumables/Chicken.hpp"
#include "../consumables/Creatine.hpp"
#include "../consumables/Steroids.hpp"
#include "../consumables/Genetics.hpp"

SinglePlayer::SinglePlayer(Helper::Size dimension, SinglePlayer::OnConsumableEaten consumableEaten,
                           SinglePlayer::OnGameOverSP onGameOver, SinglePlayer::OnSteroidConsumed onSteroidConsumed) :
        onConsumableEaten(std::move(consumableEaten)), onGameOver(std::move(onGameOver)),
        onSteroidConsumed(std::move(onSteroidConsumed)) {

    snake = std::make_shared<Snake>(Helper::Pixel{dimension.width / 2, dimension.height / 2});
    map = cv::Mat::zeros(dimension.height * pixelPerSquare, dimension.width * pixelPerSquare, CV_8UC3);
    cv::resize(wall, wall, cv::Size(iconSize.width, iconSize.height));
    initialize();
}

void SinglePlayer::initialize() {
    createBorder();
    updateBackground();

    updateMap();
    updateSnake();
    std::shared_ptr<Consumables::Consumable> consumable = std::make_shared<Consumables::Chicken>(iconSize);
    spawnConsumable(consumable);
}

void SinglePlayer::updateMap() {
    int key = cv::waitKey(1);
    onKeyPressed(key);
    if (paused) return;
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - snake->getLastUpdate()).count();
    if (elapsed >= snake->getTimeToMove()) {
        onSnakeMove();
        snake->setLastUpdate(now);
    }
}

void SinglePlayer::onSnakeMove() {
    snake->move();
    updateSnake();
}

void SinglePlayer::onKeyPressed(int key) {
    Helper::Direction input = snake->getDirection();
    if (key == -1) return;
    switch (key) {
        case 27:
            cv::destroyAllWindows();
            exit(0);
        case 82:
            if (paused) return;
            input = Helper::Direction::UP;
            break;
        case 84:
            if (paused) return;
            input = Helper::Direction::DOWN;
            break;
        case 81:
            if (paused) return;
            input = Helper::Direction::LEFT;
            break;
        case 83:
            if (paused) return;
            input = Helper::Direction::RIGHT;
            break;
        case 32:
            paused = !paused;
            break;
        case 115:
            if (snake->isOnSteroids() || !steroidsStored || paused) return;
            onSteroidConsumed(Consumables::ConsumableType::STEROIDS, Consumables::PowerUpConsumed::USED);
            --steroidsStored;
            snake->applyEffect(Consumables::Effect::RAMPAGE);
            break;
        default:
            break;
    }
    if (snake->changeDirection(input)) {
        updateSnake();
        snake->setLastUpdate(std::chrono::steady_clock::now());
    }
}

void SinglePlayer::createBorder() {
    for (int i = 0; i < map.cols; i += pixelPerSquare) {
        border.emplace_back(i, 0);
        border.emplace_back(i, map.rows - pixelPerSquare);
    }
    for (int i = 0; i < map.rows; i += pixelPerSquare) {
        border.emplace_back(0, i);
        border.emplace_back(map.cols - pixelPerSquare, i);
    }

    corners = {
            Helper::Pixel{0, 0},
            Helper::Pixel{map.cols - pixelPerSquare, 0},
            Helper::Pixel{0, map.rows - pixelPerSquare},
            Helper::Pixel{map.cols - pixelPerSquare, map.rows - pixelPerSquare}
    };

    updateBorder();
}

void SinglePlayer::updateBorder() {
    for (const auto &b: border) {
        cv::Point point = cv::Point{b.x, b.y};
        cv::Mat roi = map(cv::Rect(point.x + 2, point.y + 2, wall.cols, wall.rows));
        removeAlpha(roi, wall);
    }
}

void SinglePlayer::updateBackground() {
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

cv::Scalar SinglePlayer::randomize() {
    std::uniform_int_distribution<> r(0, 255);
    std::uniform_int_distribution<> g(0, 255);
    std::uniform_int_distribution<> b(0, 255);
    cv::Scalar out = cv::Scalar(b(engine), g(engine), r(engine));
    return out;
}

void SinglePlayer::updateSnake() { //NOLINT
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
    checkCollisionWithConsumable();
    updateOccupiedSpaces();
}

void SinglePlayer::removeAlpha(cv::Mat& roi, const cv::Mat& icon) {
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

void SinglePlayer::updateConsumables() {
    for (const auto& c : consumables) {
        cv::Point point = cv::Point{c->getPosition().x, c->getPosition().y};
        cv::Mat roi = map(cv::Rect(point.x + 2, point.y + 2, c->getIcon().cols, c->getIcon().rows));
        removeAlpha(roi, c->getIcon());
    }
}

void SinglePlayer::updateOccupiedSpaces() {
    occupiedSpaces.clear();
    occupiedSpaces.insert(snake->getHeadPosition() * pixelPerSquare);
    for (const auto &b : snake->getBody()) occupiedSpaces.insert(b * pixelPerSquare);
    for (const auto &c : consumables) occupiedSpaces.insert(c->getPosition());
    for (const auto &b : border) occupiedSpaces.insert(b);
}

void SinglePlayer::updateGameTick() {
    snake->setTimeToMove(snake->getTimeToMove() - 3);
}

void SinglePlayer::showPointsOnConsumable(const std::shared_ptr<Consumables::Consumable>& consumable) {
    if (consumable->getType() == Consumables::ConsumableType::STEROIDS ||
        consumable->getType() == Consumables::ConsumableType::GENETICS) return;
    cv::Point position = cv::Point{consumable->getPosition().x, consumable->getPosition().y};
    cv::putText(map, "+" + std::to_string(consumable->getPoints()), position, cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 2);
}

void SinglePlayer::onConsumableCollision(const std::shared_ptr<Consumables::Consumable>& consumable) {
    showPointsOnConsumable(consumable);

    if (consumable->getType() == Consumables::ConsumableType::GENETICS) {
        std::uniform_int_distribution<> chance(1, 3);
        if (chance(engine) == 1) {
            snake->setTimeToMove(snake->getTimeToMove() * 2);
        }
    }
    else if (consumable->getType() == Consumables::ConsumableType::STEROIDS) {
        steroidsStored = std::clamp(++steroidsStored, 0, 3);
        onSteroidConsumed(Consumables::ConsumableType::STEROIDS, Consumables::PowerUpConsumed::STORED);
    }

    onConsumableEaten(consumable->getPoints());
    snake->applyEffect(consumable->getEffect());
    consumables.erase(consumable);

    ++consumablesEaten;
    spawnConsumableOverTime();
    updateGameTick();
}

void SinglePlayer::checkCollisionWithConsumable() {
    for (const auto &c : consumables) {
        auto head = snake->getHeadPosition() * pixelPerSquare;
        if (head.x == c->getPosition().x && head.y == c->getPosition().y) {
            onConsumableCollision(c);
            break;
        }
    }
}

void SinglePlayer::checkCollisionWithBody() {
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

void SinglePlayer::removeBorderInX(const Helper::Pixel &px) {
    for (auto it = border.begin(); it != border.end();)
        (it->y == px.y) ? it = border.erase(it) : ++it;
}

void SinglePlayer::removeBorderInY(const Helper::Pixel &px) {
    for (auto it = border.begin(); it != border.end();)
        (it->x == px.x) ? it = border.erase(it) : ++it;
}

void SinglePlayer::removeCorners() {
    for (auto it = border.begin(); it != border.end();)
        (std::any_of(corners.begin(), corners.end(), [it](const Helper::Pixel &c) { return *it == c; }))
        ? it = border.erase(it) : ++it;

    updateBorder();
}

void SinglePlayer::borderCollision() {
    auto head = snake->getHeadPosition() * pixelPerSquare;

    for (const auto &b : border) {
        if (head == b) {
            if (!snake->isOnSteroids()) {
                onGameOver();
                return;
            }

            if (std::any_of(corners.begin(), corners.end(),
                            [head](const Helper::Pixel &c) { return head == c; })) {
                snake->setOnSteroids(false);
                updateOccupiedSpaces();
                removeCorners();
                corners.clear();
                return;
            }

            if (head.x == 0 || head.x == map.cols - pixelPerSquare) removeBorderInX(head);
            if (head.y == 0 || head.y == map.rows - pixelPerSquare) removeBorderInY(head);
            updateBorder();
            snake->setOnSteroids(false);
            updateOccupiedSpaces();
            return;
        }
    }
}

void SinglePlayer::checkOutOfBounds() { //NOLINT
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

bool SinglePlayer::consumableAlreadyExists(Consumables::ConsumableType type) {
    return std::any_of(consumables.begin(), consumables.end(), [type](const std::shared_ptr<Consumables::Consumable>& c) {
        return c->getType() == type;
    });
}

void SinglePlayer::spawnConsumableOverTime() {
    if (consumablesEaten % 1 == 0 && !consumableAlreadyExists(Consumables::ConsumableType::CHICKEN)) {
        std::shared_ptr<Consumables::Consumable> newConsumable = std::make_shared<Consumables::Chicken>(iconSize);
        spawnConsumable(newConsumable);
    }

    if (consumablesEaten % 8 == 0 && !consumableAlreadyExists(Consumables::ConsumableType::CREATINE)) {
        std::shared_ptr<Consumables::Consumable> newConsumable = std::make_shared<Consumables::Creatine>(iconSize);
        spawnConsumable(newConsumable);
    }

    if (consumablesEaten % 15 == 0 && !consumableAlreadyExists(Consumables::ConsumableType::STEROIDS)) {
        std::shared_ptr<Consumables::Consumable> newConsumable = std::make_shared<Consumables::Steroids>(iconSize);
        spawnConsumable(newConsumable);
    }

    if (consumablesEaten % 20 == 0 && !consumableAlreadyExists(Consumables::ConsumableType::GENETICS)) {
        std::shared_ptr<Consumables::Consumable> newConsumable = std::make_shared<Consumables::Genetics>(iconSize);
        spawnConsumable(newConsumable);
    }
}

void SinglePlayer::spawnConsumable(const std::shared_ptr<Consumables::Consumable>& consumable) {
    setConsumablePosition(*consumable);

    occupiedSpaces.insert(consumable->getPosition());
    consumables.insert(consumable);
    updateConsumables();

    if (consumable->getDisplayDuration() && consumable->getDisplayDuration()->hasDuration()) {
        std::thread timer([this, consumable]() {

            int duration = consumable->getDisplayDuration()->getDuration();
            while (duration > 0) {
                if (!paused) duration -= 10;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            if (consumables.find(consumable) != consumables.end()) {
                consumables.erase(consumable);
                updateConsumables();
            }
        });

        timer.detach();
    }

}

void SinglePlayer::setConsumablePosition(Consumables::Consumable& consumable) {
    Helper::Pixel pos = generatePosition();

    while (std::find(occupiedSpaces.begin(), occupiedSpaces.end(), pos) != occupiedSpaces.end()) {
        pos = generatePosition();
    }

    consumable.setPosition(pos);
}

void SinglePlayer::fitToGrid(Helper::Pixel &pixel) {
    pixel.x -= (pixel.x % pixelPerSquare);
    pixel.y -= (pixel.y % pixelPerSquare);
}

Helper::Pixel SinglePlayer::generatePosition() {
    std::uniform_int_distribution<> x(0, (map.cols - 1));
    std::uniform_int_distribution<> y(0, (map.rows - 1));

    Helper::Pixel pos = {x(engine), y(engine)};

    fitToGrid(pos);
    return pos;
}

SinglePlayer::~SinglePlayer() {
    if (snake != nullptr) snake.reset();
};