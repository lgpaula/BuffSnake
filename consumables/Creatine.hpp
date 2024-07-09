#ifndef SNAKE_CREATINE_HPP
#define SNAKE_CREATINE_HPP

#include "IConsumable.hpp"

namespace Consumables {

    class Creatine : public IConsumable {
    public:
        Creatine();

    private:
        int displayDuration = 5000;
    };

}

#endif //SNAKE_CREATINE_HPP
