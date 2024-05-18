#include "../include/Map.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "../include/Snake.hpp"

Map::Map(CoordinateStructures::Size dimension, const Map::OnDirectionChange& onDirectionChange) : map(cv::Mat::zeros(
        dimension.height, dimension.width, CV_8UC3)), onDirectionChange(onDirectionChange) {
    steps.cols = map.cols / 20;
    steps.rows = map.rows / 20;

    addBackground();
    createBorder();

    displayThread = std::thread([this, onDirectionChange]() {
        while (true) {
            cv::imshow("Map", map);
            int key = cv::waitKey(1);
            onKeyPressed(key);
        }
    });
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

CoordinateStructures::Pixel Map::getCenter() const {
    return CoordinateStructures::Pixel{map.rows / 2, map.cols / 2};
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
    //add callback for border update;

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

    checkCollision(head);
}

void Map::checkCollision(CoordinateStructures::Pixel &head) const {
    if (head.x < 0 || head.x >= map.cols || head.y < 0 || head.y >= map.rows) {
        std::cout << "Game Over!" << std::endl;
        cv::destroyAllWindows();
        //add callback for restart;
    }
}

Map::~Map() noexcept {
    if (displayThread.joinable()) {
        displayThread.join();
    }
}
