#include "space.hpp"
#include <thread>
#include <chrono>

int main()
{
    Viewpoint viewpoint(180.0, 181, -4, 0.0, 0.0);
    Space space(&viewpoint);

    space.addWall(Wall(4, -3, 2, 3));
    space.addWall(Wall(5, -3, -5, -3));
    space.addWall(Wall(5, 3, -5, 3));

    space.render();

    double move = 0;
    while(move < 8)
    {
        space.getViewpoint()->setPosX(-4.0 + move);
        space.render();
        move += 0.05;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

}