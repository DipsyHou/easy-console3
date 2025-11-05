#ifndef SPACE_HPP
#define SPACE_HPP

#include "viewpoint.hpp"
#include "wall.hpp"
#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

class Space
{
public:
    Space() : viewpoint(new Viewpoint()), ownsViewpoint(true) 
    {
        viewpoint->setViewAngle(60);
        viewpoint->setViewLineCount(300);
        viewpoint->setScreenHeight(100);
        viewpoint->setPosX(0.0);
        viewpoint->setPosY(0.0);
        viewpoint->setTowards(0.0);
    }

    Space(const std::string& filename) : viewpoint(new Viewpoint()), ownsViewpoint(true)
    {
        
        std::ifstream ifs(filename);
        if (!ifs) {
            std::cerr << "Error opening file for reading: " << filename << std::endl;
            return;
        }

        walls.clear();
        std::string line;
        while (std::getline(ifs, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;
            if (type == "Viewpoint") {
                double angle;
                int lineCount, screenHeight;
                double posX, posY, towards;
                iss >> angle >> lineCount >> screenHeight >> posX >> posY >> towards;
                viewpoint->setViewAngle(angle);
                viewpoint->setViewLineCount(lineCount);
                viewpoint->setScreenHeight(screenHeight);
                viewpoint->setPosX(posX);
                viewpoint->setPosY(posY);
                viewpoint->setTowards(towards);
            } else if (type == "Wall") {
                double x1, y1, x2, y2;
                iss >> x1 >> y1 >> x2 >> y2;
                walls.emplace_back(x1, y1, x2, y2);
            }
        }
        ifs.close();
    }

    Space(Viewpoint* vp) : viewpoint(vp), ownsViewpoint(false) {}

    ~Space() {
        if (ownsViewpoint && viewpoint) {
            delete viewpoint;
            viewpoint = nullptr;
        }
    }

    struct RayResult {
        double distance;
        bool hit;
        const Wall* hitWall;
        RayResult() : distance(1e9), hit(false), hitWall(nullptr) {}
    };
    
    Viewpoint* getViewpoint() const { return viewpoint; }

    // calculate intersection of a ray with all walls
    RayResult castRay(double rayAngle) const
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
            
            // parallel check
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

    void render() const
    {
        const int lineCount = viewpoint->getViewLineCount();
        const double halfFov = viewpoint->getViewAngle() / 2.0;
        const int screenHeight = viewpoint->getScreenHeight();
        const int screenHalfHeight = screenHeight / 2;
        
        // screen buffer
        static std::vector<std::string> screenBuffer(screenHeight);
        for (auto& line : screenBuffer) {
            line.assign(lineCount, ' ');
        }
        
        const double angleStep = viewpoint->getViewAngle() / (lineCount - 1);
        const double towardsAngle = viewpoint->getTowards();
        const double degToRad = M_PI / 180.0;
        
        for (int i = 0; i < lineCount; ++i) {
            // angle relative to the center of view
            double relativeAngle = -halfFov + angleStep * i;
            // absolute angle in the world
            double absoluteAngle = towardsAngle + relativeAngle;
            
            RayResult rayResult = castRay(absoluteAngle);
            
            if (!rayResult.hit) {
                int col = lineCount - i - 1;
                for(int y = 0; y < screenHalfHeight; ++y) {
                    screenBuffer[y][col] = ' ';  // ceiling
                }
                for(int y = screenHalfHeight; y < screenHeight; ++y) {
                    screenBuffer[y][col] = '.';  // ground
                }
                continue;
            }
            
            double correctedDistance = rayResult.distance * std::cos(relativeAngle * degToRad);
            double viewHeight = atan(5.0 / correctedDistance) / viewpoint->getViewAngle() / degToRad * screenHeight;
            // double viewHeight = atan(5.0 / rayResult.distance) / viewpoint->getViewAngle() / degToRad * screenHeight;
            
            double dx = rayResult.hitWall->getX2() - rayResult.hitWall->getX1();
            double dy = rayResult.hitWall->getY2() - rayResult.hitWall->getY1();
            double wallAngle = std::atan2(dy, dx); // -pi..pi
            double brightness = std::abs(std::cos(wallAngle)); // 0..1

            const std::string shades = ":-=+*#%@";
            int shadeIdx = std::min<int>(static_cast<int>(brightness * (shades.size() - 1) + 0.5), (int)shades.size() - 1);
            char wallChar = shades[shadeIdx];
            
            int wallHeight = static_cast<int>(viewHeight);
            int wallTop = (screenHeight - wallHeight) / 2;
            int wallBottom = wallTop + wallHeight;
            
            int col = lineCount - i - 1;
            for(int y = 0; y < screenHeight; ++y) {
                if (y >= wallTop && y < wallBottom) {
                    screenBuffer[y][col] = wallChar;
                } else if (y >= screenHalfHeight) {
                    screenBuffer[y][col] = '.';
                } else {
                    screenBuffer[y][col] = ' ';
                }
            }
        }
        
        std::cout << "\033[H";
        for (const auto& line : screenBuffer) {
            std::cout << line << '\n';
        }
        std::cout << std::flush;
    }

    void addWall(const Wall& wall) 
    {
        walls.push_back(wall);
    }

    void saveToFile(const std::string& filename) const 
    {
        std::ofstream ofs(filename);
        if (!ofs) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }
        
        ofs << "Viewpoint " << viewpoint->getViewAngle()
            << " " << viewpoint->getViewLineCount()
            << " " << viewpoint->getScreenHeight()
            << " " << viewpoint->getPosX() 
            << " " << viewpoint->getPosY()
            << " " << viewpoint->getTowards() << "\n";

        for (const auto& wall : walls) 
        {
            ofs << "Wall " << wall.getX1() << " " << wall.getY1()
                << " " << wall.getX2() << " " << wall.getY2() << "\n";
        }
        ofs.close();
    }

private:
    Viewpoint* viewpoint;
    std::vector<Wall> walls;
    bool ownsViewpoint;

};

#endif