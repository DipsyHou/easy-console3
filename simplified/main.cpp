#include "space.hpp"
#include <chrono>
#include <thread>
#include <iostream>

int main()
{
    Viewpoint viewpoint(150, 241, -4, 0.0, 0.0);
    Space space(&viewpoint);

    space.addWall(Wall(4, -3, 2, 3));
    space.addWall(Wall(5, -3, -5, -3));
    space.addWall(Wall(5, 3, -5, 3));

    space.render();

    auto startTime = std::chrono::high_resolution_clock::now();
    
    // move foward
    double move = 0;
    while(move < 5)
    {

        
        space.getViewpoint()->setPosX(-4.0 + move);
        space.render();
        move += 0.05;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    
    std::cout << "\n\n=== 性能统计 ===" << std::endl;
    std::cout << "总运行时间: " << totalTime / 1000.0 << " 秒" << std::endl;
    std::cout << "平均帧率: " << (move / 0.05) * 1000.0 / totalTime << " FPS" << std::endl;

    // turn right
    double angle = 0.0;
    while(angle < 360.0)
    {
        space.getViewpoint()->setTowards(angle);
        space.render();
        angle += 1.0;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}