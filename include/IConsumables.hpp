#ifndef SNAKE_CONSUMABLES_HPP
#define SNAKE_CONSUMABLES_HPP

#include "CoordinateStructures.hpp"

namespace Food {

    enum ConsumableType{
        CHICKEN,
        PROTEIN,
        CREATINE,
        STEROIDS
    };

    struct Consumable {
        Consumable(ConsumableType type) : type(type) {
            setPoints();
        }

        ConsumableType type;
        CoordinateStructures::Pixel position;
        int points;
        bool isCurrentlySpawned = false;
//        Icon icon;

        void setPoints() {
            switch (type) {
                case CHICKEN:
                    points = 5;
                    break;
                case PROTEIN:
                    points = 20;
                    break;
                case CREATINE:
                    points = 20;
                    break;
                case STEROIDS:
                    points = -10;
                    break;
            }
        }



    };

}

#endif //SNAKE_CONSUMABLES_HPP
