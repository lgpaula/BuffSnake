#ifndef SNAKE_GENETICS_HPP
#define SNAKE_GENETICS_HPP

#include "IConsumable.hpp"

namespace Consumables {

    class Genetics : public IConsumable {
    public:
        Genetics();

    private:
        int displayDuration = 5000;
    };

}

#endif //SNAKE_GENETICS_HPP
