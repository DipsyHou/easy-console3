#ifndef UTILS_HPP
#define UTILS_HPP

#include "space.hpp"
#include <iostream>
#include <cmath>

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

// Print char at given position in console
inline void printChar(char c, int posX, int posY)
{
    std::cout << "\033[" << posY << ";" << posX << "H" << c << std::flush;
}

// Get character without waiting for Enter (cross-platform)
#ifdef _WIN32
    inline char getch() { return _getch(); }
#else
    inline char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
            perror("tcgetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
            perror("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror("tcsetattr ~ICANON");
        return buf;
    }
#endif

// Check collision with any wall
inline bool checkCollision(const Space& space, double newX, double newY, double collisionRadius = 0.1)
{
    const int numRays = 8;
    for (int i = 0; i < numRays; ++i) {
        double angle = (360.0 / numRays) * i;
        
        Viewpoint* originalVp = space.getViewpoint();
        double origX = originalVp->getPosX();
        double origY = originalVp->getPosY();
        
        originalVp->setPosX(newX);
        originalVp->setPosY(newY);
        
        Space::RayResult result = space.castRay(angle);
        
        originalVp->setPosX(origX);
        originalVp->setPosY(origY);
        
        if (result.hit && result.distance < collisionRadius) {
            return true;
        }
    }
    return false;
}

// Main interaction loop
inline void runInteractionLoop(Space& space, double stepLength = 0.1, double rotateAngle = 2.0, bool enableCheckCollision = true)
{
    const double deg2rad = M_PI / 180.0;
    Viewpoint* viewpoint = space.getViewpoint();
    char command;
    
    std::cout << "Use WASD to move, QE to rotate. Press ESC to quit." << std::endl;
    
    while(true)
    {
        #ifdef _WIN32
            command = _getch();
        #else
            command = getch();
        #endif
        
        switch (command)
        {
        case 'a':
            {
                double newX = viewpoint->getPosX() - stepLength * std::sin(viewpoint->getTowards() * deg2rad);
                double newY = viewpoint->getPosY() + stepLength * std::cos(viewpoint->getTowards() * deg2rad);
                if (enableCheckCollision == false || !checkCollision(space, newX, newY)) {
                    viewpoint->setPosX(newX);
                    viewpoint->setPosY(newY);
                }
            }
            break;
        case 'd':
            {
                double newX = viewpoint->getPosX() + stepLength * std::sin(viewpoint->getTowards() * deg2rad);
                double newY = viewpoint->getPosY() - stepLength * std::cos(viewpoint->getTowards() * deg2rad);
                if (enableCheckCollision == false || !checkCollision(space, newX, newY)) {
                    viewpoint->setPosX(newX);
                    viewpoint->setPosY(newY);
                }
            }
            break;
        case 'w':
            {
                double newX = viewpoint->getPosX() + stepLength * std::cos(viewpoint->getTowards() * deg2rad);
                double newY = viewpoint->getPosY() + stepLength * std::sin(viewpoint->getTowards() * deg2rad);
                if (enableCheckCollision == false || !checkCollision(space, newX, newY)) {
                    viewpoint->setPosX(newX);
                    viewpoint->setPosY(newY);
                }
            }
            break;
        case 's':
            {
                double newX = viewpoint->getPosX() - stepLength * std::cos(viewpoint->getTowards() * deg2rad);
                double newY = viewpoint->getPosY() - stepLength * std::sin(viewpoint->getTowards() * deg2rad);
                if (enableCheckCollision == false || !checkCollision(space, newX, newY)) {
                    viewpoint->setPosX(newX);
                    viewpoint->setPosY(newY);
                }
            }
            break;
        case 'q':
            viewpoint->setTowards(viewpoint->getTowards() + rotateAngle);
            break;
        case 'e':
            viewpoint->setTowards(viewpoint->getTowards() - rotateAngle);
            break;
        case 27:  // ESC
            std::cout << "Exiting..." << std::endl;
            return;
        default:
            continue;
        }
        
        std::cout << "\033[2J\033[H" << std::flush;
        space.render();
        std::cout << "Position: (" << viewpoint->getPosX() << ", " << viewpoint->getPosY() 
                  << ") Towards: " << int(viewpoint->getTowards()) % 360 << std::endl;
    }
}

#endif
