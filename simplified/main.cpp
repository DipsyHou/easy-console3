#include "space.hpp"

int main()
{
    Space space;

    // 添加墙体（x1, y1, x2, y2, brightness）
    space.addWall(Wall(5, -2, 5, 20));   // 垂直墙
    space.addWall(Wall(5, -2, -5, -2));
    space.addWall(Wall(5, 2, -5, 2));   // 水平墙

    space.render();

}