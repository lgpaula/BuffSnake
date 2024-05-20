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
        Consumable() = default;

        explicit Consumable(ConsumableType type) : type(type) {
            setPoints();
        }

        ConsumableType type{};
        CoordinateStructures::Pixel position{};
        int points{};
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

        bool operator==(const Consumable& other) const {
            return type == other.type;
        }

    };

}

template <>
struct std::hash<Food::Consumable> {
    std::size_t operator()(const Food::Consumable& c) const {
        using std::hash;
        using std::size_t;
        using std::string;

        size_t res = 17;
        res = res * 31 + hash<int>()(static_cast<int>(c.type));
        return res;
    }
};

#endif //SNAKE_CONSUMABLES_HPP
