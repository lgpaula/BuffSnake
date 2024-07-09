#ifndef SNAKE_PROTEIN_HPP
#define SNAKE_PROTEIN_HPP

#include "IConsumable.hpp"

namespace Consumables {

    class Protein : public IConsumable {
    public:
        Protein();

    private:
        int displayDuration = 5000;
    };

}

#endif //SNAKE_PROTEIN_HPP
