#include "../include/Game.hpp"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

Game::Game(int screenHeight, int screenWidth) : screenHeight(screenHeight), screenWidth(screenWidth) {
        fullscreenDisplay = cv::Mat(screenHeight, screenWidth, CV_8UC3, backgroundColor);

    setMainMenuText();
    resizeIcons();

    mainDisplayThread = std::thread([this]() {
        while (true) {
            if (gameRunning) {
                gameMode->updateMap();
                overlayMap();
                updateScore();
                updateConsumables();
                cv::imshow("Game", fullscreenDisplay);
                continue;
            }
            if (onGameOver && resetElementsFlag) {
                resetElementsFlag = false;
                setGameOverScreen();
                cv::imshow("Game", fullscreenDisplay);
                continue;
            }
            cv::imshow("Game", fullscreenDisplay);
            int key = cv::waitKey(1);
            onKeyPressed(key);
        }
    });
}

void Game::resizeIcons() {
    cv::Size iconSize(23, 23);
    cv::resize(steroidIcon, steroidIcon, iconSize);
    cv::resize(preworkoutIcon, preworkoutIcon, iconSize);
    for (auto& icon : dbIcons) cv::resize(icon, icon, iconSize);
}

void Game::updateScore() {
    if (mode == Mode::SINGLE_PLAYER) {
        cv::Point point = {mapPosition.x + 5, mapPosition.y - 5};
        cv::putText(fullscreenDisplay, "SCORE: " + std::to_string(gamePoints), point, cv::FONT_HERSHEY_SIMPLEX, 0.75, black, 2);
    }
}

void Game::overlayMap() {
    cv::rectangle(fullscreenDisplay, cv::Point(0, 0), cv::Point(fullscreenDisplay.cols, fullscreenDisplay.rows), backgroundColor, cv::FILLED);
    cv::Rect roi(mapPosition, gameMode->getMap().size());
    gameMode->getMap().copyTo(fullscreenDisplay(roi));
}

void removeAlpha(cv::Mat& roi, const cv::Mat& mat) {
    std::vector<cv::Mat> channels;
    cv::split(mat, channels);
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

void Game::updateConsumables() {
    if (mode == Mode::SINGLE_PLAYER) {
        for (int i = 0; i < steroidCounter; ++i) {
            cv::Mat roi = fullscreenDisplay(cv::Rect((steroidPosition.x - i*30), steroidPosition.y, steroidIcon.cols, steroidIcon.rows));
            removeAlpha(roi, steroidIcon);
        }
    } else if (mode == Mode::MULTI_PLAYER_LOCAL) {
        for (size_t i = 0; i < amountOfPlayers; ++i) {
            for (const auto& position : dbPositions[i]) cv::rectangle(fullscreenDisplay, position, position + cv::Point(25, 25), cv::Scalar(30, 130, 245), 1);
            for (const auto& position : consumablePositions[i]) cv::rectangle(fullscreenDisplay, position, position + cv::Point(25, 25), Helper::COLORS[i].toScalar(), 1);
        }

        for (size_t set = 0; set < amountOfPlayers; ++set) {
            for (auto it = dragonBalls[set].begin(); it != dragonBalls[set].end(); ++it) {
                auto icon = dbIcons[*it-1];
                cv::Mat roi = fullscreenDisplay(cv::Rect(dbPositions[set][*it-1].x + 2, dbPositions[set][*it-1].y + 2, icon.cols, icon.rows));
                removeAlpha(roi, icon);
            }
        }

        for (size_t vec = 0; vec < amountOfPlayers; ++vec) {
            for (size_t i = 0; i < powerUps[vec].size(); ++i) {
                auto icon = powerUps[vec][i] == Consumables::Effect::RAMPAGE ? steroidIcon : preworkoutIcon;
                cv::Mat roi = fullscreenDisplay(cv::Rect(consumablePositions[vec][i].x + 2, consumablePositions[vec][i].y + 2, icon.cols, icon.rows));
                removeAlpha(roi, icon);
            }
        }
    }
}

bool Game::updatePowerUps(Consumables::Effect effect, Consumables::PowerUpConsumed power, int snakeId) {
    if (power == Consumables::PowerUpConsumed::USED) {
        if (removePowerUp(effect, snakeId)) return true;
    }
    else if (power == Consumables::PowerUpConsumed::STORED) {
        addPowerUp(effect, snakeId);
    }
    return false;
}

void Game::addPowerUp(Consumables::Effect effect, int snakeId) {
    if (effect != Consumables::Effect::RAMPAGE && effect != Consumables::Effect::SPEED) return;
    if (powerUps[snakeId].size() >= 5) return;
    powerUps[snakeId].emplace_back(effect);
}

bool Game::removePowerUp(Consumables::Effect effect, int snakeId) {
    if (std::find(powerUps[snakeId].begin(), powerUps[snakeId].end(), effect) == powerUps[snakeId].end()) return false;
    for (auto it = powerUps[snakeId].begin(); it != powerUps[snakeId].end(); ++it) {
        if (*it == effect) {
            powerUps[snakeId].erase(it);
            return true;
        }
    }

    return false;
}

int Game::addLife(int snakeId, int consumableId) {
    dragonBalls[snakeId].insert(consumableId);
    updateConsumables();
    return static_cast<int>(dragonBalls[snakeId].size());
}

int Game::removeLife(int snakeId) {
    if (dragonBalls[snakeId].empty()) return 0;

    auto randomIndex = std::rand() % dragonBalls[snakeId].size(); //NOLINT(cert-msc50-cpp)
    auto it = dragonBalls[snakeId].begin();
    std::advance(it, randomIndex);
    dragonBalls[snakeId].erase(it);

    updateConsumables();

    return static_cast<int>(dragonBalls[snakeId].size());
}

void Game::addBackground(const std::string& path) {
    cv::Mat logo = cv::imread(path, cv::IMREAD_UNCHANGED);
    cv::Mat roi = fullscreenDisplay(cv::Rect(0, 0, logo.cols, logo.rows));
    removeAlpha(roi, logo);
}

void Game::setMainMenuText() {
    addBackground("icons/mainMenuSnake.png");
    onMainMenu = true;
    onGameOver = false;
    cv::putText(fullscreenDisplay, "Singleplayer", mainMenuFirstOption, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Multiplayer Local", mainMenuSecondOption, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Multiplayer LAN", mainMenuThirdOption, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Multiplayer Online", mainMenuFourthOption, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
//    cv::putText(fullscreenDisplay, "Instructions", mainMenuFifthOption, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    addSelector(mainMenuFirstOption.y);
}

void Game::addSelector(int height) {
    if (!onMainMenu && !onGameOver) return;
    selectorPosition.y = height;
    cv::rectangle(fullscreenDisplay, cv::Point(screenWidth / 2 - 500, screenHeight / 2 - 300),
                  cv::Point(screenWidth / 2 - 475, screenHeight / 2), backgroundColor, cv::FILLED);
    cv::putText(fullscreenDisplay, ">", selectorPosition, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
}

void Game::onKeyPressed(int key) { //NOLINT
    switch (key) {
        case KEY_BACKSPACE:
            handleBackspace();
            break;
        case KEY_UP:
            handleKeyUp();
            break;
        case KEY_DOWN:
            handleKeyDown();
            break;
        case KEY_ENTER:
            handleEnter();
            break;
        case 27:
            cv::destroyAllWindows();
            exit(0);
        default:
            break;
    }
}

void Game::handleBackspace() {
    if (!onMainMenu && !onGameOver) setMainMenuText();
}

void Game::handleKeyUp() {
    moveSelectorUp();
}

void Game::handleKeyDown() {
    moveSelectorDown();
}

void Game::handleEnter() { //NOLINT
    if (onMainMenu) {
        if (selectorPosition.y == mainMenuFirstOption.y) {
            mode = Mode::SINGLE_PLAYER;
            singlePlayerInstructions();
        } else if (selectorPosition.y == mainMenuSecondOption.y) {
            mode = Mode::MULTI_PLAYER_LOCAL;
            multiPlayerInstructions();
        } else if (selectorPosition.y == mainMenuThirdOption.y) {
            mode = Mode::MULTI_PLAYER_LAN;
            nopeScreen();
        } else if (selectorPosition.y == mainMenuFourthOption.y) {
            mode = Mode::MULTI_PLAYER_ONLINE;
            nopeScreen();
        }
    } else if (onGameOver) {
        if (selectorPosition.y == playAgainPosition.y) {
            startGame();
        } else if (selectorPosition.y == returnToMenuPosition.y) {
            setMainMenuText();
        }
    } else if (onInstructions) {
        startGame();
    }
}

void Game::moveSelectorUp() {
    selectorPosition.y -= 50;
    if (onMainMenu) {
        selectorPosition.y = std::clamp(selectorPosition.y, mainMenuFirstOption.y, mainMenuFourthOption.y);
    }
    if (onGameOver) {
        selectorPosition.y = std::clamp(selectorPosition.y, playAgainPosition.y, returnToMenuPosition.y);
    }
    addSelector(selectorPosition.y);
}

void Game::moveSelectorDown() {
    selectorPosition.y += 50;
    if (onMainMenu) {
        selectorPosition.y = std::clamp(selectorPosition.y, mainMenuFirstOption.y, mainMenuFourthOption.y);
    }
    if (onGameOver) {
        selectorPosition.y = std::clamp(selectorPosition.y, playAgainPosition.y, returnToMenuPosition.y);
    }
    addSelector(selectorPosition.y);
}

void Game::onPlayerSelection(int key) { //NOLINT
    switch (key) {
        case KEY_BACKSPACE:
            handleBackspace();
            break;
        case 50:
            amountOfPlayers = 2;
            startGame();
            break;
        case 51:
            amountOfPlayers = 3;
            startGame();
            break;
        case 52:
            amountOfPlayers = 4;
            startGame();
            break;
        case 27:
            cv::destroyAllWindows();
            exit(0);
        default:
            break;
    }
}

void Game::singlePlayerInstructions() { //NOLINT
    onMainMenu = false;
    onInstructions = true;
    addBackground("icons/spInstructions.png");
    cv::imshow("Game", fullscreenDisplay);

    int key = cv::waitKey(1);
    onKeyPressed(key);
}

void Game::multiPlayerInstructions() { //NOLINT
    onMainMenu = false;
    onInstructions = true;

    int index = 1;
    while (true) {
        addBackground("icons/mpInstructions" + std::to_string(index) + ".png");
        if (index == 1) index = 2;
        else index = 1;

        cv::imshow("Game", fullscreenDisplay);
        int key = cv::waitKey(600);
        if (key != -1) {
            onPlayerSelection(key);
            break;
        }
    }
}

void Game::nopeScreen() {
    addBackground("icons/nopeScreen.png");
    cv::imshow("Game", fullscreenDisplay);
    cv::waitKey(50);
    setMainMenuText();
}

void Game::updateSteroidCount(Consumables::PowerUpConsumed power) {
    switch (power) {
        case Consumables::PowerUpConsumed::STORED:
            ++steroidCounter;
            break;
        case Consumables::PowerUpConsumed::USED:
            --steroidCounter;
            if (gamePoints >= 50) gamePoints -= 50;
            else gamePoints = 0;
            break;
    }

    steroidCounter = std::clamp(steroidCounter, 0, 3);
}

void Game::startGame() {
    resetElements();
    auto size = Helper::Size{25, 25};

    switch (mode) {
        case Mode::SINGLE_PLAYER:
            mapPosition = {screenWidth / 2 - 250, screenHeight / 2 - 250};
            gameMode = std::make_unique<SinglePlayer>(size, [this](int points) {
                addPoints(points);
            }, [this]() {
                gameOver();
            }, [this](Consumables::ConsumableType type, Consumables::PowerUpConsumed power) {
                updateSteroidCount(power);
            });
            gameRunning = true;
            break;
        case Mode::MULTI_PLAYER_LOCAL: {
            mapPosition = {screenWidth / 2 - 600, screenHeight / 2 - 450};
            size = Helper::Size{50, 35};
            gameMode = std::make_unique<MultiPlayerLocal>(amountOfPlayers, size, [this](int snakeId) {
                gameOverMultiPlayer(snakeId);
            }, [this](Consumables::Effect effect, Consumables::PowerUpConsumed power, int snakeId) {
                return updatePowerUps(effect, power, snakeId);
            }, [this](int snakeId, int consumableId) {
                return addLife(snakeId, consumableId);
            }, [this](int snakeId) {
                return removeLife(snakeId);
            });
            gameRunning = true;
            break;
        }
        case Mode::MULTI_PLAYER_LAN:
        case Mode::MULTI_PLAYER_ONLINE:
        default:
            break;
    }
}

void Game::resetElements() {
    gamePoints = 0;
    steroidCounter = 0;
    dragonBalls = {std::set<int>{4}, std::set<int>{4}, std::set<int>{4}, std::set<int>{4}};
    powerUps = {std::vector<Consumables::Effect>{}, std::vector<Consumables::Effect>{},
            std::vector<Consumables::Effect>{}, std::vector<Consumables::Effect>{}};
    if (gameMode != nullptr) gameMode.reset();
}

void Game::addPoints(int points) {
    if (points > 0) gamePoints += points; //avoids steroids -> stupid, stupid way.

    gamePoints = std::clamp(gamePoints, 0, gamePoints);
}

void Game::setGameOverScreen() {
    if (mode == Mode::SINGLE_PLAYER) {
        setGameOverScreenSinglePlayer();
    } else if (mode == Mode::MULTI_PLAYER_LOCAL) {
        setGameOverScreenMultiPlayer();
    }
}

void Game::setGameOverScreenSinglePlayer() {
    addBackground("icons/gameOverSnake2.png");
    cv::putText(fullscreenDisplay, "Score: " + std::to_string(gamePoints), finalScorePosition, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Play again", playAgainPosition, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Return to main menu", returnToMenuPosition, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    addSelector(playAgainPosition.y);
}

void Game::setGameOverScreenMultiPlayer() {
    cv::rectangle(fullscreenDisplay, cv::Point(0, 0), cv::Point(fullscreenDisplay.cols, fullscreenDisplay.rows), backgroundColor, cv::FILLED);

    std::vector<cv::Mat> loserIcons;
    for (int i = 0; i < amountOfPlayers; ++i) {
        if (i == winner) continue;
        loserIcons.emplace_back(cv::imread("icons/sad_" + getColorName(Helper::COLORS[i]) + "_snake.png", cv::IMREAD_UNCHANGED));
    }
    if (winner == -1) {
        for (int i = 0; i < loserIcons.size(); ++i) {
            auto snakePositions = amountOfPlayers - 1;
            cv::Mat roi = fullscreenDisplay(cv::Rect(snakeDisplayPositions[snakePositions][i].x,
                                             snakeDisplayPositions[snakePositions][i].y, loserIcons[i].cols, loserIcons[i].rows));
            removeAlpha(roi, loserIcons[i]);
        }
    } else {
        auto colorName = getColorName(Helper::COLORS[winner]);
        auto winnerIcon = cv::imread("icons/big_" + colorName + "_snake.png", cv::IMREAD_UNCHANGED);
        cv::resize(winnerIcon, winnerIcon, cv::Size(982, 1016));

        auto snakePositions = amountOfPlayers - 2;
        cv::Mat roi = fullscreenDisplay(cv::Rect(snakeDisplayPositions[snakePositions][0].x,
                                                 snakeDisplayPositions[snakePositions][0].y,winnerIcon.cols, winnerIcon.rows));
        removeAlpha(roi, winnerIcon);
        for (int i = 0; i < loserIcons.size(); ++i) {
            roi = fullscreenDisplay(cv::Rect(snakeDisplayPositions[snakePositions][i+1].x,
                                             snakeDisplayPositions[snakePositions][i+1].y, loserIcons[i].cols, loserIcons[i].rows));
            removeAlpha(roi, loserIcons[i]);
        }
    }

    cv::putText(fullscreenDisplay, "Play again", playAgainPosition, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    cv::putText(fullscreenDisplay, "Return to main menu", returnToMenuPosition, cv::FONT_HERSHEY_SIMPLEX, 1, white, 2);
    addSelector(playAgainPosition.y);
}

void Game::gameOver() {
    std::cout << "Game over" << std::endl;
    onGameOver = true;
    onMainMenu = false;
    gameRunning = false;
    resetElementsFlag = true;
}

void Game::gameOverMultiPlayer(int snakeId) {
    winner = snakeId;
    gameOver();
}

Game::~Game() noexcept {
    cv::destroyAllWindows();
    if (mainDisplayThread.joinable()) mainDisplayThread.join();
}
