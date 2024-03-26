#ifndef SNAKE_SNAKE_HPP
#define SNAKE_SNAKE_HPP

struct HeadPosition {
    int x;
    int y;
};

class Snake {

public:
    Snake(int x, int y);

    void move();


private:
    HeadPosition headPosition;

};


#endif //SNAKE_SNAKE_HPP
