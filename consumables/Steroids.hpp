#ifndef SNAKE_STEROIDS_HPP
#define SNAKE_STEROIDS_HPP

#include "IConsumable.hpp"

namespace Consumables {

    class Steroids : public IConsumable {
    public:
        Steroids();

    private:
        int displayDuration = 5000;
        int effectDuration = 3000;
    };

}

#endif //SNAKE_STEROIDS_HPP
