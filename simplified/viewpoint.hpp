#ifndef VIEWPOINT_HPP
#define VIEWPOINT_HPP

class Viewpoint
{
public:
    Viewpoint(double angle = 150.0, int lineCount = 301, int screenHeight = 64, double posX = 0.0, double posY = 0.0, double towards = 0.0)
        : viewAngle(angle), viewLineCount(lineCount), screenHeight(screenHeight), posX(posX), posY(posY), towards(towards){}

    double getViewAngle() const { return viewAngle; }
    int getViewLineCount() const { return viewLineCount; }
    int getScreenHeight() const { return screenHeight; }
    double getPosX() const { return posX; }
    double getPosY() const { return posY; }
    double getTowards() const { return towards; }

    void setViewAngle(double angle) { viewAngle = angle; }
    void setViewLineCount(int count) { viewLineCount = count; }
    void setScreenHeight(int height) { screenHeight = height; }
    void setPosX(double x) { posX = x; }
    void setPosY(double y) { posY = y; }
    void setTowards(double angle) { towards = angle; }

private:
    double viewAngle;   // 0-360
    int viewLineCount;
    int screenHeight;
    double posX;
    double posY;
    double towards; // 0-360
};

#endif