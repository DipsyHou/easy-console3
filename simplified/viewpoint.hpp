
class Viewpoint
{
public:
    Viewpoint(double angle, int lineCount)
        : viewAngle(angle), viewLineCount(lineCount) {}

    double getViewAngle() const { return viewAngle; }

    int getViewLineCount() const { return viewLineCount; }

    double getPosX() const { return posX; }
    double getPosY() const { return posY; }
    double getTowards() const { return towards; }
    
private:
    double viewAngle;   // 视场角（度)
    int viewLineCount; // 视线数量
    double posX = 0.0; // 视点X坐标
    double posY = 0.0; // 视点Y坐标
    double towards = 0.0; // 视点朝向
};