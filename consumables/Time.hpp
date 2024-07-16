#ifndef SNAKE_ITIME_HPP
#define SNAKE_ITIME_HPP


class ITime {
public:
    ITime() = default;
    virtual ~ITime() = default;

    [[nodiscard]] virtual bool hasDuration() const = 0;

    [[nodiscard]] virtual int getDuration() const = 0;

    virtual void setDuration(int time) = 0;
};

class DurationYes : public ITime {
    [[nodiscard]] bool hasDuration() const override { return true; };

    [[nodiscard]] int getDuration() const override { return duration; };

    void setDuration(int time) override { this->duration = time; };

public:
    int duration{};
};

class DurationNo : public ITime {
    [[nodiscard]] bool hasDuration() const override { return false; };

    [[nodiscard]] int getDuration() const override { return 0; };

    void setDuration(int time) override { this->duration = time; };

public:
    [[maybe_unused]] int duration{};
};

#endif //SNAKE_ITIME_HPP
