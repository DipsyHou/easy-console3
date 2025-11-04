#include "space.hpp"
#include <chrono>
#include <thread>
#include <iostream>

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>

    char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
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

// Check if a position would collide with any wall
bool checkCollision(const Space& space, double newX, double newY, double collisionRadius = 0.1)
{
    // Cast rays in multiple directions around the player
    const int numRays = 8;  // Check 8 directions
    for (int i = 0; i < numRays; ++i) {
        double angle = (360.0 / numRays) * i;
        
        // Calculate ray endpoint
        double rayEndX = newX + collisionRadius * std::cos(angle * M_PI / 180.0);
        double rayEndY = newY + collisionRadius * std::sin(angle * M_PI / 180.0);
        
        // Check if ray from center to edge point hits any wall
        Viewpoint tempViewpoint(0, 0, 0, newX, newY, 0.0);
        Space tempSpace(&tempViewpoint);
        
        // We need to check the distance to walls
        // Use the space's castRay method by temporarily updating viewpoint
        Viewpoint* originalVp = space.getViewpoint();
        double origX = originalVp->getPosX();
        double origY = originalVp->getPosY();
        
        originalVp->setPosX(newX);
        originalVp->setPosY(newY);
        
        Space::RayResult result = space.castRay(angle);
        
        // Restore original position
        originalVp->setPosX(origX);
        originalVp->setPosY(origY);
        
        if (result.hit && result.distance < collisionRadius) {
            return true;  // Collision detected
        }
    }
    return false;  // No collision
}

int main()
{
    Viewpoint viewpoint(60, 300, 100, 0.0, 0.0, 0.0);
    Space space(&viewpoint);

    const double deg2rad = M_PI / 180.0;
    space.addWall(Wall(4.0, -2, 4.0, 2));
    space.addWall(Wall(4.0, -2, -4.0, -2));
    space.addWall(Wall(4.0, 2, -8.0, 2));
    space.addWall(Wall(-8, 2, -8, -6));
    space.addWall(Wall(-8, -6, 4, -6));

    space.render();

    char command;
    double stepLength = 0.1;
    double rotateAngle = 2.0;
    
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
                double newX = viewpoint.getPosX() - stepLength * std::sin(viewpoint.getTowards() * deg2rad);
                double newY = viewpoint.getPosY() + stepLength * std::cos(viewpoint.getTowards() * deg2rad);
                if (!checkCollision(space, newX, newY)) {
                    viewpoint.setPosX(newX);
                    viewpoint.setPosY(newY);
                }
            }
            break;
        case 'd':
            {
                double newX = viewpoint.getPosX() + stepLength * std::sin(viewpoint.getTowards() * deg2rad);
                double newY = viewpoint.getPosY() - stepLength * std::cos(viewpoint.getTowards() * deg2rad);
                if (!checkCollision(space, newX, newY)) {
                    viewpoint.setPosX(newX);
                    viewpoint.setPosY(newY);
                }
            break;
            }
        case 'w':
            {
                double newX = viewpoint.getPosX() + stepLength * std::cos(viewpoint.getTowards() * deg2rad);
                double newY = viewpoint.getPosY() + stepLength * std::sin(viewpoint.getTowards() * deg2rad);
                if (!checkCollision(space, newX, newY)) {
                    viewpoint.setPosX(newX);
                    viewpoint.setPosY(newY);
                }
            }
            break;
        case 's':
            {
                double newX = viewpoint.getPosX() - stepLength * std::cos(viewpoint.getTowards() * deg2rad);
                double newY = viewpoint.getPosY() - stepLength * std::sin(viewpoint.getTowards() * deg2rad);
                if (!checkCollision(space, newX, newY)) {
                    viewpoint.setPosX(newX);
                    viewpoint.setPosY(newY);
                }
            }
            break;
        case 'q':
            viewpoint.setTowards(viewpoint.getTowards() + rotateAngle);
            break;
        case 'e':
            viewpoint.setTowards(viewpoint.getTowards() - rotateAngle);
            break;
        case 27:  // ESC
            std::cout << "Exiting..." << std::endl;
            return 0;
        default:
            continue;
        }
        std::cout << "\033[2J\033[H" << std::flush;
        space.render();
        std::cout << "Position: (" << viewpoint.getPosX() << ", " << viewpoint.getPosY() << ") Towards: " << int(viewpoint.getTowards()) % 360 << std::endl;
    }

}