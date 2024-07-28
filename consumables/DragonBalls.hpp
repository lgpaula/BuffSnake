#ifndef SNAKE_DRAGONBALLS_HPP
#define SNAKE_DRAGONBALLS_HPP

#include "Consumable.hpp"
#include "Time.hpp"

namespace Consumables {

    class DragonBalls : public Consumable {
    public:
        explicit DragonBalls(cv::Size size);

        [[nodiscard]] ConsumableType getType() const override { return type; }

        [[nodiscard]] int getPoints() const override { return points; }

        [[nodiscard]] const cv::Mat &getIcon() const override { return icon; }

        [[nodiscard]] Effect getEffect() const override { return effect; }

        [[nodiscard]] Helper::Pixel getPosition() const override { return position; }

        [[nodiscard]] int getId() const override { return id; }

        const std::shared_ptr<ITime>& getDisplayDuration() const override { return displayDuration; }

        void setPosition(Helper::Pixel pixel) override;

        bool operator==(const DragonBalls& other) const {
            return type == other.type;
        }

    private:
        void setId();

        void setIcon();

        void resizeIcon();

    private:
        cv::Size size{};
        int duration = 10000;
        std::shared_ptr<ITime> displayDuration{};
        ConsumableType type{};
        int points{};
        cv::Mat icon{};
        Effect effect{};
        Helper::Pixel position{};
        int id{};
    };
}

template <>
struct std::hash<Consumables::DragonBalls> {
    std::size_t operator()(const Consumables::DragonBalls& db) const noexcept {
        std::size_t h1 = std::hash<int>{}(db.getId());
        std::size_t h2 = std::hash<int>{}(db.getId());
        return h1 ^ (h2 << 1);
    }
};


#endif //SNAKE_DRAGONBALLS_HPP
