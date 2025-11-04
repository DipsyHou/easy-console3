#include "space.hpp"
#include <chrono>
#include <thread>
#include <iostream>

int main()
{
    Viewpoint viewpoint(150, 601, 128, -4, 0.0, 0.0);
    Space space(&viewpoint);

    space.addWall(Wall(4, -3, 2, 3));
    space.addWall(Wall(5, -3, -10, -3));
    space.addWall(Wall(5, 3, -5, 3));

    space.render();

    double angle = 0.0;
    int rotateFrameCount = 0;
    auto lastRotateFpsTime = std::chrono::high_resolution_clock::now();
    
    while(true)
    {
        space.getViewpoint()->setTowards(angle);
        space.render();
        angle += 0.5;
        
        rotateFrameCount++;
        
        // 每0.5秒更新一次帧率显示
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - lastRotateFpsTime).count();
        
        if (elapsed >= 500) {
            double fps = rotateFrameCount * 1000.0 / elapsed;
            std::cout << "\033[62;1H";
            std::cout << "FPS: " << fps << " angel: " << angle;
            
            rotateFrameCount = 0;
            lastRotateFpsTime = currentTime;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}