#ifndef SNAKE_CREATINE_HPP
#define SNAKE_CREATINE_HPP

#include "Consumable.hpp"
#include "Time.hpp"

namespace Consumables {

    class Creatine : public Consumable {
    public:
        explicit Creatine(cv::Size size);

        [[nodiscard]] ConsumableType getType() const override { return type; }

        [[nodiscard]] int getPoints() const override { return points; }

        [[nodiscard]] const cv::Mat &getIcon() const override { return icon; }

        [[nodiscard]] Effect getEffect() const override { return effect; }

        [[nodiscard]] Helper::Pixel getPosition() const override { return position; }

        [[nodiscard]] int getId() const override { return id; }

        const std::shared_ptr<ITime>& getDisplayDuration() const override { return displayDuration; }

        void setPosition(Helper::Pixel pixel) override;

        bool operator==(const Creatine& other) const {
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
        Helper::Pixel position{};
        int id{};
    };

}

#endif //SNAKE_CREATINE_HPP
