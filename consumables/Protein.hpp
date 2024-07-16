#ifndef SNAKE_PROTEIN_HPP
#define SNAKE_PROTEIN_HPP

#include "Consumable.hpp"
#include "Time.hpp"

namespace Consumables {

    class Protein : public Consumable {
    public:
        explicit Protein(cv::Size size);

        [[nodiscard]] ConsumableType getType() const override { return type; }

        [[nodiscard]] int getPoints() const override { return points; }

        [[nodiscard]] const cv::Mat &getIcon() const override { return icon; }

        [[nodiscard]] Effect getEffect() const override { return effect; }

        [[nodiscard]] CoordinateStructures::Pixel getPosition() const override { return position; }

        const std::shared_ptr<ITime>& getDisplayDuration() const override { return displayDuration; }

        void setPosition(CoordinateStructures::Pixel pixel) override;

        bool operator==(const Protein& other) const {
            return type == other.type;
        }

    private:
        void resizeIcon();

    private:
        cv::Size size{};
        int duration = 8000;
        std::shared_ptr<ITime> displayDuration{};
        ConsumableType type{};
        int points{};
        cv::Mat icon{};
        Effect effect{};
        CoordinateStructures::Pixel position{};
    };
}

#endif //SNAKE_PROTEIN_HPP
