#include "space.hpp"
#include <chrono>
#include <thread>
#include <iostream>

int main()
{
    Viewpoint viewpoint(120, 601, 128, -4, 0.0, 0.0);
    Space space(&viewpoint);

    // create multiple walls with different inclinations (angles in degrees)
    const double deg2rad = M_PI / 180.0;
    double centerX = 4.0; // distance in front of camera
    double centerY = 0.0;
    double wallLength = 20.0;
    // angles: -75, -45, -15, 0, 15, 45, 75
    for (double ang = -75.0; ang <= 75.0; ang += 30.0) {
        double a = ang * deg2rad;
        double dx = std::cos(a) * (wallLength * 0.5);
        double dy = std::sin(a) * (wallLength * 0.5);
        double x1 = centerX - dx;
        double y1 = centerY - dy;
        double x2 = centerX + dx;
        double y2 = centerY + dy;
        space.addWall(Wall(x1, y1, x2, y2));
    }

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
            std::cout << "\033[129;1H";
            std::cout << "FPS: " << fps << " angel: " << angle;
            
            rotateFrameCount = 0;
            lastRotateFpsTime = currentTime;
        }
        
        // std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}