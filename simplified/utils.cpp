#include <iostream>

// print char at given position in console
void printChar(char c, int posX, int posY)
{
    std::cout << "\033[" << posY << ";" << posX << "H" << c << std::flush;
}