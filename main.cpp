#include <iostream>
#include "gameModes/SinglePlayer.hpp"
#include "include/Game.hpp"
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <X11/Xlib.h>
#endif

int main() {
    int screenWidth = 0;
    int screenHeight = 0;

#ifdef _WIN32
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
#elif __linux__
    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cerr << "Cannot open display" << std::endl;
        return 1;
    }

    Screen* screen = DefaultScreenOfDisplay(display);
    screenWidth = screen->width;
    screenHeight = screen->height;
    XCloseDisplay(display);
#endif

    std::cout << "screenHeight: " << screenHeight << " screenWidth: " << screenWidth << std::endl;

//    std::unique_ptr<Game> game = std::make_unique<Game>(screenHeight, screenWidth);
    std::unique_ptr<Game> game = std::make_unique<Game>(1080, 1920);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    return 0;
}