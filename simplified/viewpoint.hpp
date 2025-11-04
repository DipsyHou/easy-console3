
class Viewpoint
{
public:
    Viewpoint(double angle, int lineCount, double posX = 0.0, double posY = 0.0, double towards = 0.0)
        : viewAngle(angle), viewLineCount(lineCount), posX(posX), posY(posY), towards(towards){}

    double getViewAngle() const { return viewAngle; }
    int getViewLineCount() const { return viewLineCount; }
    double getPosX() const { return posX; }
    double getPosY() const { return posY; }
    double getTowards() const { return towards; }

    void setPosX(double x) { posX = x; }
    void setPosY(double y) { posY = y; }
    void setTowards(double angle) { towards = angle; }

private:
    double viewAngle;   // 0-360
    int viewLineCount;
    double posX;
    double posY;
    double towards; // 0-360
};