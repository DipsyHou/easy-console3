#include "viewpoint.hpp"
#include "wall.hpp"

#include <vector>
#include <cmath>
#include <iostream>

class Space
{
public:
    Space() {}
    ~Space() {}

    struct RayResult {
        double distance;    // 距离视点的距离
        bool hit;           // 是否击中物体
        const Wall* hitWall; // 击中的墙体指针
        RayResult() : distance(1e9), hit(false), hitWall(nullptr) {}
    };
    

    void printChar(char c, int posX, int posY)
    {
        // 移动光标到指定位置并打印字符
        std::cout << "\033[" << posY << ";" << posX << "H" << c << std::flush;
    }

    // 计算指定角度的射线与所有墙体的相交
    // rayAngle: 射线的绝对角度（度）
    RayResult castRay(double rayAngle) 
    {
        RayResult result;
        
        double rayOriginX = viewpoint.getPosX();
        double rayOriginY = viewpoint.getPosY();
        
        double angleRad = rayAngle * M_PI / 180.0;
        
        double rayDirX = std::cos(angleRad);
        double rayDirY = std::sin(angleRad);
        
        double closestDistance = 1e9;
        
        for (const auto& wall : walls) {
            double wallX1 = wall.getX1();
            double wallY1 = wall.getY1();
            double wallX2 = wall.getX2();
            double wallY2 = wall.getY2();
            
            
            double wallDirX = wallX2 - wallX1;
            double wallDirY = wallY2 - wallY1;
            
            double denominator = rayDirX * wallDirY - rayDirY * wallDirX;
            
            if (std::abs(denominator) < 1e-10) {
                continue;
            }
            
            double dx = wallX1 - rayOriginX;
            double dy = wallY1 - rayOriginY;
            
            double t = (dx * wallDirY - dy * wallDirX) / denominator;
            double s = (dx * rayDirY - dy * rayDirX) / denominator;
            
            if (t >= 0 && s >= 0 && s <= 1) {
                double distance = t;
                
                if (distance < closestDistance) {
                    closestDistance = distance;
                    result.hit = true;
                    result.distance = distance;
                    result.hitWall = &wall;  // 记录击中的墙体
                }
            }
        }
        
        return result;
    }

    void render()
    {
        int lineCount = viewpoint.getViewLineCount();
        double halfFov = viewpoint.getViewAngle() / 2.0;
        
        for (int i = 0; i < lineCount; ++i) {
            double rayAngle = -halfFov + (viewpoint.getViewAngle() * i) / (lineCount - 1);
            RayResult rayResult = castRay(rayAngle);
            
            const int screenHeight = 24;  // 屏幕高度
            
            // 计算墙体在屏幕上的投影高度
            double viewHeight = rayResult.hit ? atan(5.0 / rayResult.distance) / 1.57 * screenHeight : 0.0;
            
            // 判断墙体方向（水平或垂直）
            char wallChar = ' ';
            if (rayResult.hit && rayResult.hitWall != nullptr) {
                double dx = rayResult.hitWall->getX2() - rayResult.hitWall->getX1();
                double dy = rayResult.hitWall->getY2() - rayResult.hitWall->getY1();
                
                // 判断墙体是水平还是垂直
                if (std::abs(dx) > std::abs(dy)) {
                    wallChar = '@';  // 水平墙
                } else {
                    wallChar = '+';  // 垂直墙
                }
            }
            
            // 计算墙体的垂直居中位置
            int wallHeight = static_cast<int>(viewHeight);
            int wallTop = (screenHeight - wallHeight) / 2;  // 墙体顶部位置（居中）
            int wallBottom = wallTop + wallHeight;          // 墙体底部位置

            for(int y = 0; y < screenHeight; ++y) {
                char displayChar;
                
                if (y >= wallTop && y < wallBottom && rayResult.hit) {
                    // 墙体区域
                    displayChar = wallChar;
                } else if (y >= screenHeight / 2) {
                    // 下半部分 - 地面
                    displayChar = '.';
                } else {
                    // 上半部分 - 天空
                    displayChar = ' ';
                }
                
                printChar(displayChar, lineCount - i, y + 1);
            }
            // if (rayResult.hit) {
            //     std::cout << "Ray " << i << " Angle: " << rayAngle 
            //               << "° Hit at distance: " << rayResult.distance << std::endl;
            // } else {
            //     std::cout << "Ray " << i << " Angle: " << rayAngle 
            //               << "° No hit" << std::endl;
            // }
        }
    }

    void addWall(const Wall& wall) {
        walls.push_back(wall);
    }

private:
    Viewpoint viewpoint{180, 91};
    std::vector<Wall> walls;

};