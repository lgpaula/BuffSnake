#ifndef SNAKE_STEROIDS_HPP
#define SNAKE_STEROIDS_HPP

#include "Consumable.hpp"

namespace Consumables {

    class Steroids : public Consumable {
    public:
        Steroids();

    private:
        int displayDuration = 5000;
        int effectDuration = 3000;
    };

}

#endif //SNAKE_STEROIDS_HPP
