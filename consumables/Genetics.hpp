#ifndef SNAKE_GENETICS_HPP
#define SNAKE_GENETICS_HPP

#include "Consumable.hpp"

namespace Consumables {

    class Genetics : public Consumable {
    public:
        Genetics();

    private:
        int displayDuration = 5000;
    };

}

#endif //SNAKE_GENETICS_HPP
