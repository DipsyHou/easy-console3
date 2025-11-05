#include "space.hpp"
#include "utils.hpp"
#include <chrono>
#include <thread>
#include <iostream>

int main()
{
    Viewpoint viewpoint(60, 300, 100, 0.0, 0.0, 0.0);
    Space space(&viewpoint);

    const double deg2rad = M_PI / 180.0;

    space.addWall(Wall(-20, -20, 20, -20));    // 下
    space.addWall(Wall(20, -20, 20, 16));      // 右（留出口）
    space.addWall(Wall(20, 20, -20, 20));      // 上
    space.addWall(Wall(-20, 20, -20, -20));    // 左
    space.addWall(Wall(-18, -18, -12, -12));   // 45° 斜墙
    space.addWall(Wall(-12, -12, -12, -6));    // 竖墙
    space.addWall(Wall(-12, -6, -18, 0));      // 斜墙
    space.addWall(Wall(-18, 0, -15, 3));       // 斜墙
    space.addWall(Wall(-18, 5, -8, 15));       // 左上斜墙
    space.addWall(Wall(-18, 15, -8, 5));       // 左下斜墙（交叉）
    space.addWall(Wall(-15, 8, -15, 12));      // 中间竖墙
    space.addWall(Wall(-8, -18, 0, -10));      // 右上斜
    space.addWall(Wall(0, -10, -5, -5));       // 左上斜
    space.addWall(Wall(3, 3, -2, 8));          // 左上斜
    space.addWall(Wall(8, -18, 15, -8));       // 斜墙1
    space.addWall(Wall(10, -16, 17, -6));      // 斜墙2（平行）
    space.addWall(Wall(12, -14, 12, -8));      // 连接墙
    space.addWall(Wall(12, -8, 18, -2));       // 延伸斜墙
    space.addWall(Wall(8, 5, 12, -3));         // 左斜墙
    space.addWall(Wall(16, 5, 12, -3));        // 右斜墙（V字）
    space.addWall(Wall(12, -3, 12, -8));       // 底部竖墙
    space.addWall(Wall(-15, 18, -10, 13));     // 左斜1
    space.addWall(Wall(-10, 13, -5, 18));      // 左斜2
    space.addWall(Wall(-5, 18, 0, 13));        // 中斜1
    space.addWall(Wall(0, 13, 5, 18));         // 中斜2
    space.addWall(Wall(5, 18, 5, 13));         // 竖墙
    space.addWall(Wall(8, 8, 14, 14));         // 斜墙1
    space.addWall(Wall(10, 10, 10, 16));       // 竖墙
    space.addWall(Wall(10, 16, 15, 16));       // 横墙
    space.addWall(Wall(15, 10, 18, 13));       // 最后斜墙
    space.addWall(Wall(-6, -12, 2, -8));       // 下方斜墙
    space.addWall(Wall(2, -8, 6, -12));        // 下方斜墙2
    space.addWall(Wall(-10, 5, -5, 0));        // 左侧斜墙
    space.addWall(Wall(6, 2, 10, 6));          // 右侧斜墙       

    space.render();

    runInteractionLoop(space, 0.1, 2.0, true);
    
    return 0;
}