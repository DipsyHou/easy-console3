#ifndef VIEW_POINT_HPP
#define VIEW_POINT_HPP

#include "vector3d.hpp"
#include "box.hpp"
#include <vector>
#include <cmath>

// 视点类 - 负责从固定位置(0,0,0)渲染3D场景
class ViewPoint
{
public:
    // 构造函数: angle为视场角(度), lineCount为视线数量
    ViewPoint(double angle = 90.0, int lineCount = 80);
    ~ViewPoint();

    // 渲染结果结构
    struct RayResult {
        double height;      // 物体高度（y坐标）
        double brightness;  // 亮度 (0.0 - 1.0)
        double distance;    // 距离视点的距离
        bool hit;           // 是否击中物体
        
        RayResult() : height(0), brightness(0), distance(0), hit(false) {}
    };

    // 渲染场景 - 返回每条视线的结果
    std::vector<RayResult> render(const std::vector<Box>& boxes, double pitch = 0.0) const;

    // 获取视点位置（固定在原点）
    Vector3D getPosition() const { return position; }
    
    // 设置视场角
    void setViewAngle(double angle) { viewAngle = angle; }
    
    // 设置视线数量
    void setViewLineCount(int count) { viewLineCount = count; }

private:
    Vector3D position;      // 视点位置（固定为0,0,0）
    double viewAngle;       // 水平视场角（度）
    int viewLineCount;      // 视线数量
};

// 构造函数实现
inline ViewPoint::ViewPoint(double angle, int lineCount) 
    : position(0, 0, 0), viewAngle(angle), viewLineCount(lineCount) {
}

inline ViewPoint::~ViewPoint() {
}

// 渲染函数实现
inline std::vector<ViewPoint::RayResult> ViewPoint::render(
    const std::vector<Box>& boxes, double pitch) const {
    
    std::vector<RayResult> results(viewLineCount);
    
    // pitch: 俯仰角（度），0表示水平向前
    double half_fov = viewAngle * 0.5 * M_PI / 180.0;  // 转换为弧度
    double pitch_rad = pitch * M_PI / 180.0;
    
    for (int i = 0; i < viewLineCount; ++i) {
        // 计算当前视线的水平角度（等角度间隔）
        double horizontal_angle = -half_fov + (2.0 * half_fov * i) / (viewLineCount - 1);
        
        // 计算视线方向（在XZ平面上旋转，然后应用俯仰角）
        Vector3D ray_direction(
            std::cos(pitch_rad) * std::sin(horizontal_angle),
            std::sin(pitch_rad),
            std::cos(pitch_rad) * std::cos(horizontal_angle)
        );
        ray_direction = ray_direction.normalize();
        
        // 对所有物体进行射线检测，找到最近的物体
        RayResult& result = results[i];
        double closest_distance = 1e9;
        
        for (const Box& box : boxes) {
            Box::IntersectionResult intersection = box.intersect(position, ray_direction);
            
            if (intersection.hit && intersection.distance < closest_distance) {
                closest_distance = intersection.distance;
                result.hit = true;
                result.height = intersection.height;
                result.brightness = intersection.brightness;
                result.distance = intersection.distance;
            }
        }
    }
    
    return results;
}

#endif // VIEW_POINT_HPP