#ifndef SNAKE_CREATINE_HPP
#define SNAKE_CREATINE_HPP

#include "Consumable.hpp"

namespace Consumables {

    class Creatine : public Consumable {
    public:
        Creatine();

    private:
        int displayDuration = 5000;
    };

}

#endif //SNAKE_CREATINE_HPP
