#include "../include/Game.hpp"
#include <iostream>

Game::Game(Display &display) : display(display) {
    std::cout << "Game created" << std::endl;
}

void Game::addPoints(int points) {
    gamePoints += points;
}

int Game::getPoints() const {
    return gamePoints;
}

void Game::gameOver() {

}
