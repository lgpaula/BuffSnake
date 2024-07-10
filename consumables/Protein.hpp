#ifndef SNAKE_PROTEIN_HPP
#define SNAKE_PROTEIN_HPP

#include "Consumable.hpp"

namespace Consumables {

    class Protein : public Consumable {
    public:
        Protein();

    private:
        int displayDuration = 5000;
    };

}

#endif //SNAKE_PROTEIN_HPP
