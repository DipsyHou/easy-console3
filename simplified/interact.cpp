#include "space.hpp"
#include "utils.hpp"
#include <chrono>
#include <thread>
#include <iostream>



int main()
{

    Space space(60, 300, 100, 0, 2, 0.0);

    addArcWall(space, 0, 0, 10, 0, 180, 360);
    space.addWall(Wall(10, 0, -10, 0));

    runInteractionLoop(space, 0.1, 2.0, true);
    
    space.saveToFile("saves/save.space");

    Space space2("saves/save.space");

    runInteractionLoop(space2, 0.1, 2.0, true);

    return 0;
}