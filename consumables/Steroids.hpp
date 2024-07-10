#ifndef SNAKE_STEROIDS_HPP
#define SNAKE_STEROIDS_HPP

#include "Consumable.hpp"

namespace Consumables {

    class Steroids : public Consumable {
    public:
        Steroids();

        int getDisplayDuration() override;

    };

}

#endif //SNAKE_STEROIDS_HPP
