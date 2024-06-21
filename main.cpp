#include <iostream>
#include "include/Map.hpp"
#include "include/Game.hpp"
#include "include/Snake.hpp"
#include <X11/Xlib.h>

int main() {

    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cerr << "Cannot open display" << std::endl;
        return 1;
    }

    Screen* screen = DefaultScreenOfDisplay(display);
    int screenWidth = screen->width;
    int screenHeight = screen->height;
    std::cout << "screenHeight: " << screenHeight << " screenWidth: " << screenWidth << std::endl;

    std::unique_ptr<Game> game = std::make_unique<Game>(1080, 1920);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    XCloseDisplay(display);

    return 0;

    /*
     * add bricks which tilt on steroids.
     * steroids go super saiyan?
     * head and tail are different sprites
     * icons split into folders
     * timers
     * score at all times
     * screen size
     * levels
     */
}