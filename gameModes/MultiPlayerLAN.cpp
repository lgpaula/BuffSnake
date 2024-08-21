#include "MultiPlayerLAN.hpp"

#include <utility>
#include "../include/Snake.hpp"
#include "../consumables/Chicken.hpp"
#include "../consumables/Steroids.hpp"
#include "../consumables/Preworkout.hpp"

MultiPlayerLAN::MultiPlayerLAN(int playersOnLan, Helper::Size dimension, OnGameOverMP onGameOver,
                               OnConsumableConsumedMP onConsumableConsumed, OnLifeIncrease onLifeIncrease, OnLifeDecrease onLifeDecrease) :
        amountOfPlayers(playersOnLan), onGameOver(std::move(onGameOver)), onConsumableConsumed(std::move(onConsumableConsumed)),
        onLifeIncrease(std::move(onLifeIncrease)), onLifeDecrease(std::move(onLifeDecrease)) {

    instantiateStartingPositions(dimension);
    createSnakes();
    map = cv::Mat::zeros(dimension.height * pixelPerSquare, dimension.width * pixelPerSquare, CV_8UC3);
    cv::resize(wall, wall, cv::Size(iconSize.width, iconSize.height));
    initialize();
}

void MultiPlayerLAN::instantiateStartingPositions(Helper::Size dimension) {
    startingPositions = {
            Helper::Pixel{3, 3},
            Helper::Pixel{dimension.width - 4, 3},
            Helper::Pixel{3, dimension.height - 4},
            Helper::Pixel{dimension.width - 4, dimension.height - 4}
    };
}

void MultiPlayerLAN::createSnakes() {
    for (size_t i = 0; i < amountOfPlayers; ++i) {
        auto color = Helper::COLORS[i];
        auto newSnake = std::make_shared<Snake>(startingPositions[i], color, static_cast<Helper::Direction>(i), i);
        snakes.insert(newSnake);
        updateOccupiedSpaces();
    }

    generateKeyMappings();
}

void MultiPlayerLAN::generateKeyMappings() {
    for (const auto& snake : snakes) {
        if (snake->getId() == 0) {
            movementKeyMappings[snake] = {
                    {82, Helper::Direction::UP},
                    {84, Helper::Direction::DOWN},
                    {81, Helper::Direction::LEFT},
                    {83, Helper::Direction::RIGHT}
            };
            effectKeyMappings[snake] = {
                    {255, Consumables::Effect::RAMPAGE},  // DEL
                    {86, Consumables::Effect::SPEED}      // PAGEDOWN
            };
        } else if (snake->getId() == 1) {
            movementKeyMappings[snake] = {
                    {119, Helper::Direction::UP},    // W
                    {115, Helper::Direction::DOWN},  // S
                    {97,  Helper::Direction::LEFT},   // A
                    {100, Helper::Direction::RIGHT}  // D
            };
            effectKeyMappings[snake] = {
                    {113, Consumables::Effect::RAMPAGE},  // Q
                    {101, Consumables::Effect::SPEED}     // E
            };
        } else if (snake->getId() == 2) {
            movementKeyMappings[snake] = {
                    {105, Helper::Direction::UP},    // I
                    {107, Helper::Direction::DOWN},  // K
                    {106, Helper::Direction::LEFT},  // J
                    {108, Helper::Direction::RIGHT}  // L
            };
            effectKeyMappings[snake] = {
                    {117, Consumables::Effect::RAMPAGE},  // U
                    {111, Consumables::Effect::SPEED}     // O
            };
        } else if (snake->getId() == 3) {
            movementKeyMappings[snake] = {
                    {56, Helper::Direction::UP},    // 8
                    {53, Helper::Direction::DOWN},  // 5
                    {52, Helper::Direction::LEFT},  // 4
                    {54, Helper::Direction::RIGHT}  // 6
            };
            effectKeyMappings[snake] = {
                    {55, Consumables::Effect::RAMPAGE},  // 7
                    {57, Consumables::Effect::SPEED}     // 9
            };
        }
    }
}

void MultiPlayerLAN::initialize() {
    createBorder();
    updateBackground();

    updateMap();
    updateSnake();
    for (size_t i = 0; i < amountOfPlayers; ++i) {
        std::shared_ptr<Consumables::Consumable> consumable = std::make_shared<Consumables::Chicken>(iconSize);
        spawnConsumable(consumable);
    }
}

cv::Scalar MultiPlayerLAN::randomize() {
    std::uniform_int_distribution<> r(0, 255);
    std::uniform_int_distribution<> g(0, 255);
    std::uniform_int_distribution<> b(0, 255);
    cv::Scalar out = cv::Scalar(b(engine), g(engine), r(engine));
    return out;
}

void MultiPlayerLAN::updateSnake() { //NOLINT
    updateBackground();

    for (auto& snake : snakes) {
        auto headColor = snake->isOnSteroids() ? randomize() : snake->getColor().toScalar();
        auto head = snake->getHeadPosition() * pixelPerSquare;
        cv::Point h1 = cv::Point{(head.x) + 2, (head.y) + 2};
        cv::Point h2 = cv::Point{(head.x + pixelPerSquare) - 3, (head.y + pixelPerSquare) - 3};
        cv::rectangle(map, h1, h2, headColor, -1);

        for (auto b : snake->getBody()) {
            auto bodyColor = snake->isOnSteroids() ? randomize() : snake->getColor().toScalar();
            b *= pixelPerSquare;
            cv::Point b1 = cv::Point{(b.x) + 5, (b.y) + 5};
            cv::Point b2 = cv::Point{(b.x + pixelPerSquare) - 6, (b.y + pixelPerSquare) - 6};
            cv::rectangle(map, b1, b2, bodyColor, -1);
        }
        checkCollisionWithConsumable(snake);
    }

    checkCollisionWithOtherSnake();
    borderCollision();
    checkOutOfBounds();
    updateOccupiedSpaces();
}

void MultiPlayerLAN::updateMap() {
    if (snakes.empty()) {
        onGameOver(-1);
        return;
    }
    if (snakes.size() == 1) {
        onGameOver(snakes.begin()->get()->getId());
        return;
    }

    checkDeadSnakes();

    int key = cv::waitKey(1);
    onKeyPressed(key);

    if (paused) return;

    auto now = std::chrono::steady_clock::now();
    for (auto& snake : snakes) {
        if (deadSnakes.find(snake->getId()) != deadSnakes.end()) continue;
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - snake->getLastUpdate()).count();
        if (elapsed > snake->getTimeToMove()) {
            snake->setLastUpdate(now);
            onSnakeMove(snake);
        }
    }
}

void MultiPlayerLAN::checkDeadSnakes() {
    for (const auto id : deadSnakes) {
        auto it = std::find_if(snakes.begin(), snakes.end(), [id](const std::shared_ptr<Snake>& s) { return s->getId() == id; });
        if (it != snakes.end()) {
            snakes.erase(it);
        }
    }
    deadSnakes.clear();
}

void MultiPlayerLAN::onSnakeMove(const std::shared_ptr<Snake>& snake) {
    snake->move();
    updateSnake();
}

void MultiPlayerLAN::updateBackground() {
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

void MultiPlayerLAN::createBorder() {
    for (int i = 0; i < map.cols; i += pixelPerSquare) {
        border.emplace_back(i, 0);
        border.emplace_back(i, map.rows - pixelPerSquare);
    }
    for (int i = pixelPerSquare; i < map.rows - pixelPerSquare; i += pixelPerSquare) {
        border.emplace_back(0, i);
        border.emplace_back(map.cols - pixelPerSquare, i);
    }

    updateOccupiedSpaces();
    for (size_t i = 0; i < amountOfRandomWalls; ++i) {
        Helper::Pixel pos = generatePosition();
        while (std::find(occupiedSpaces.begin(), occupiedSpaces.end(), pos) != occupiedSpaces.end()) {
            pos = generatePosition();
        }
        border.push_back(pos);
    }

    updateOccupiedSpaces();
    updateBorder();
}

void MultiPlayerLAN::updateBorder() {
    for (const auto& b: border) {
        cv::Point point = cv::Point{b.x, b.y};
        cv::Mat roi = map(cv::Rect(point.x + 2, point.y + 2, wall.cols, wall.rows));
        removeAlpha(roi, wall);
    }
}

void MultiPlayerLAN::fitToGrid(Helper::Pixel &pixel) {
    pixel.x -= (pixel.x % pixelPerSquare);
    pixel.y -= (pixel.y % pixelPerSquare);
}

void MultiPlayerLAN::onKeyPressed(int key) {
    if (key == 32) paused = !paused;

    if (paused) return;

    for (const auto& snake : snakes) {
        auto& movementKeys = movementKeyMappings[snake];
        auto& effectKeys = effectKeyMappings[snake];

        if (movementKeys.find(key) != movementKeys.end()) {
            Helper::Direction input = movementKeys[key];
            if (snake->changeDirection(input)) {
                snake->setLastUpdate(std::chrono::steady_clock::now());
                updateSnake();
            }
        } else if (effectKeys.find(key) != effectKeys.end()) {
            Consumables::Effect effect = effectKeys[key];
            if (onConsumableConsumed(effect, Consumables::PowerUpConsumed::USED, snake->getId()))
                snake->applyEffect(effect);
        } else if (key == 27) {
            cv::destroyAllWindows();
            exit(0);
        }
    }
}

void MultiPlayerLAN::checkCollisionWithOtherSnake() {
    for (const auto& attacker : snakes) {
        for (const auto& attacked : snakes) {
            if (attacker == attacked) continue;

            if (attacker->getHeadPosition() == attacked->getHeadPosition()) {
                attacker->setOnSteroids(false);
                attacked->setOnSteroids(false);

                attacker->getBody().erase(attacker->getBody().begin(), attacker->getBody().end());
                attacked->getBody().erase(attacked->getBody().begin(), attacked->getBody().end());

                if (!onLifeDecrease(attacker->getId())) deadSnakes.insert(attacker->getId());
                if (!onLifeDecrease(attacked->getId())) deadSnakes.insert(attacked->getId());
                continue;
            }

            auto it = std::find(attacked->getBody().begin(), attacked->getBody().end(), attacker->getHeadPosition());
            if (it != attacked->getBody().end()) {
                if (attacker->isOnSteroids()) {
                    attacked->getBody().erase(attacked->getBody().begin(), ++it);
                    if (!onLifeDecrease(attacked->getId())) deadSnakes.insert(attacked->getId());
                } else {
                    attacker->getBody().erase(attacker->getBody().begin(), attacker->getBody().end());
                    if (!onLifeDecrease(attacker->getId())) deadSnakes.insert(attacker->getId());
                }
            }
        }
    }
}

void MultiPlayerLAN::checkOutOfBounds() { //NOLINT
    for (auto &snake : snakes) {
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
}

void MultiPlayerLAN::checkCollisionWithConsumable(const std::shared_ptr<Snake>& snake) {
    for (const auto &c : consumables) {
        auto head = snake->getHeadPosition() * pixelPerSquare;
        if (head.x == c->getPosition().x && head.y == c->getPosition().y) {
            onConsumableCollision(c, snake);
            break;
        }
    }
}

void MultiPlayerLAN::setConsumablePosition(Consumables::Consumable &consumable) {
    Helper::Pixel pos = generatePosition();

    while (std::find(occupiedSpaces.begin(), occupiedSpaces.end(), pos) != occupiedSpaces.end()) {
        pos = generatePosition();
    }

    consumable.setPosition(pos);
}

void MultiPlayerLAN::updateOccupiedSpaces() {
    occupiedSpaces.clear();
    for (const auto &snake : snakes) {
        occupiedSpaces.insert(snake->getHeadPosition() * pixelPerSquare);
        for (const auto &b : snake->getBody()) occupiedSpaces.insert(b * pixelPerSquare);
    }
    for (const auto &c : consumables) occupiedSpaces.insert(c->getPosition());
    for (const auto &b : border) occupiedSpaces.insert(b);
}

void MultiPlayerLAN::updateConsumables() {
    for (const auto& c : consumables) {
        cv::Point point = cv::Point{c->getPosition().x, c->getPosition().y};
        cv::Mat roi = map(cv::Rect(point.x + 2, point.y + 2, c->getIcon().cols, c->getIcon().rows));
        removeAlpha(roi, c->getIcon());
    }
}

Helper::Pixel MultiPlayerLAN::generatePosition() {
    std::uniform_int_distribution<> x(0, (map.cols - 1));
    std::uniform_int_distribution<> y(0, (map.rows - 1));

    Helper::Pixel pos = {x(engine), y(engine)};

    fitToGrid(pos);
    return pos;
}

void MultiPlayerLAN::spawnConsumableOverTime() {
    if (consumablesEaten % 1 == 0 && !consumableAlreadyExists(Consumables::ConsumableType::CHICKEN)) {
        std::shared_ptr<Consumables::Consumable> newConsumable = std::make_shared<Consumables::Chicken>(iconSize);
        spawnConsumable(newConsumable);
    }

    if (consumablesEaten % 1 == 0 && !consumableAlreadyExists(Consumables::ConsumableType::DRAGONBALLS)) {
        std::shared_ptr<Consumables::Consumable> newConsumable = std::make_shared<Consumables::DragonBalls>(iconSize);
        spawnConsumable(newConsumable);
    }

    if (consumablesEaten % 1 == 0 && !consumableAlreadyExists(Consumables::ConsumableType::PREWORKOUT)) {
        std::shared_ptr<Consumables::Consumable> newConsumable = std::make_shared<Consumables::Preworkout>(iconSize);
        spawnConsumable(newConsumable);
    }

    if (consumablesEaten % 1 == 0 && !consumableAlreadyExists(Consumables::ConsumableType::STEROIDS)) {
        std::shared_ptr<Consumables::Consumable> newConsumable = std::make_shared<Consumables::Steroids>(iconSize);
        spawnConsumable(newConsumable);
    }
}

void MultiPlayerLAN::updateGameTick() {
    for (auto& snake : snakes) {
        snake->setTimeToMove(snake->getTimeToMove() - 3);
    }
}

void MultiPlayerLAN::borderCollision() {
    for (auto& snake : snakes) {
        auto head = snake->getHeadPosition() * pixelPerSquare;
        for (auto it = border.begin(); it != border.end();) {
            if (head == *it) {
                if (!snake->isOnSteroids()) if (!onLifeDecrease(snake->getId())) deadSnakes.insert(snake->getId());

                border.erase(it);
                updateBorder();
                updateOccupiedSpaces();
                return;
            } else { ++it; }
        }
    }
}

void MultiPlayerLAN::onConsumableCollision(const std::shared_ptr<Consumables::Consumable> &consumable, const std::shared_ptr<Snake>& snake) {
    onConsumableConsumed(consumable->getEffect(), Consumables::PowerUpConsumed::STORED, snake->getId());

    if (consumable->getType() == Consumables::ConsumableType::DRAGONBALLS) {
        auto life = onLifeIncrease(snake->getId(), consumable->getId());
        if (life == 7) {
            onGameOver(snake->getId());
            return;
        }
    }

    if (consumable->getType() == Consumables::ConsumableType::CHICKEN) snake->applyEffect(consumable->getEffect());
    if (consumable->getType() == Consumables::ConsumableType::CREATINE) snake->applyEffect(consumable->getEffect());

    consumables.erase(consumable);

    ++consumablesEaten;
    spawnConsumableOverTime();
    updateGameTick();
}

void MultiPlayerLAN::checkCollisionWithBody() {}

void MultiPlayerLAN::spawnConsumable(const std::shared_ptr<Consumables::Consumable> &consumable) {
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

void MultiPlayerLAN::removeAlpha(cv::Mat &roi, const cv::Mat &icon) {
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

bool MultiPlayerLAN::consumableAlreadyExists(Consumables::ConsumableType type) {
    int cap = 0;
    switch (type) {
        case Consumables::ConsumableType::CHICKEN:
            cap = 8;
            break;
        case Consumables::ConsumableType::DRAGONBALLS:
            cap = 7;
            break;
        case Consumables::ConsumableType::PREWORKOUT:
            cap = 3;
            break;
        case Consumables::ConsumableType::STEROIDS:
            cap = 2;
            break;
        default:
            break;
    }

    int current = 0;
    for (const auto& consumable : consumables) {
        if (consumable->getType() == type) ++current;
        if (current >= cap) return true;
    }

    return false;
}

MultiPlayerLAN::~MultiPlayerLAN() = default;