#include "viewpoint.hpp"
#include "wall.hpp"
#include "utils.cpp"
#include <vector>
#include <cmath>
#include <iostream>

class Space
{
public:
    Space() : viewpoint(nullptr) {}
    Space(Viewpoint* vp) : viewpoint(vp) {}
    ~Space() {}

    struct RayResult {
        double distance;
        bool hit;
        const Wall* hitWall;
        RayResult() : distance(1e9), hit(false), hitWall(nullptr) {}
    };
    
    Viewpoint* getViewpoint() const { return viewpoint; }

    // calculate intersection of a ray with all walls
    RayResult castRay(double rayAngle) 
    {
        RayResult result;
        
        double rayOriginX = viewpoint->getPosX();
        double rayOriginY = viewpoint->getPosY();
        
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
                    result.hitWall = &wall;
                }
            }
        }
        
        return result;
    }

    void render()
    {
        int lineCount = viewpoint->getViewLineCount();
        double halfFov = viewpoint->getViewAngle() / 2.0;
        
        for (int i = 0; i < lineCount; ++i) {
            
            // angle relative to the center of view
            double relativeAngle = -halfFov + (viewpoint->getViewAngle() * i) / (lineCount - 1);
            // absolute angle in the world
            double absoluteAngle = viewpoint->getTowards() + relativeAngle;
            
            RayResult rayResult = castRay(absoluteAngle);
            
            const int screenHeight = 60;
            
            double correctedDistance = rayResult.distance * std::cos(relativeAngle * M_PI / 180.0);
            double viewHeight = rayResult.hit ? atan(5.0 / correctedDistance) / 1.57 * screenHeight : 0.0;
            
            // todo: brightness according to wall direction
            char wallChar = ' ';
            if (rayResult.hit && rayResult.hitWall != nullptr) {
                double dx = rayResult.hitWall->getX2() - rayResult.hitWall->getX1();
                double dy = rayResult.hitWall->getY2() - rayResult.hitWall->getY1();
                
                if (std::abs(dx) > std::abs(dy)) {
                    wallChar = '@';
                } else {
                    wallChar = '+';
                }
            }
            
            int wallHeight = static_cast<int>(viewHeight);
            int wallTop = (screenHeight - wallHeight) / 2;
            int wallBottom = wallTop + wallHeight;

            for(int y = 0; y < screenHeight; ++y) {
                char displayChar;
                
                if (y >= wallTop && y < wallBottom && rayResult.hit) {
                    // wall
                    displayChar = wallChar;
                } else if (y >= screenHeight / 2) {
                    // ground
                    displayChar = '.';
                } else {
                    // ceiling
                    displayChar = ' ';
                }
                
                printChar(displayChar, lineCount - i, y + 1);
            }
        }
    }

    void addWall(const Wall& wall) {
        walls.push_back(wall);
    }

private:
    Viewpoint* viewpoint;
    std::vector<Wall> walls;

};