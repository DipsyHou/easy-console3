#ifndef BOX_HPP
#define BOX_HPP

#include "vector3d.hpp"
#include <algorithm>

// 长方体类
class Box {
public:
    Vector3D min_point;  // 最小顶点坐标
    Vector3D max_point;  // 最大顶点坐标
    double brightness;   // 物体亮度 (0.0 - 1.0)
    
    Box(const Vector3D& min_pt, const Vector3D& max_pt, double bright = 1.0)
        : min_point(min_pt), max_point(max_pt), brightness(bright) {}
    
    // 射线与长方体相交检测
    // 返回值: 是否相交, 如果相交返回交点距离和交点高度
    struct IntersectionResult {
        bool hit;
        double distance;
        double height;  // 交点的y坐标（高度）
        double brightness;
        
        IntersectionResult() : hit(false), distance(0), height(0), brightness(0) {}
    };
    
    IntersectionResult intersect(const Vector3D& ray_origin, const Vector3D& ray_direction) const {
        IntersectionResult result;
        
        // 使用AABB（轴对齐包围盒）射线相交算法
        double tmin = -1e9, tmax = 1e9;
        
        // 检查X轴
        if (std::abs(ray_direction.x) > 1e-10) {
            double tx1 = (min_point.x - ray_origin.x) / ray_direction.x;
            double tx2 = (max_point.x - ray_origin.x) / ray_direction.x;
            tmin = std::max(tmin, std::min(tx1, tx2));
            tmax = std::min(tmax, std::max(tx1, tx2));
        } else {
            if (ray_origin.x < min_point.x || ray_origin.x > max_point.x) {
                return result;  // 不相交
            }
        }
        
        // 检查Y轴
        if (std::abs(ray_direction.y) > 1e-10) {
            double ty1 = (min_point.y - ray_origin.y) / ray_direction.y;
            double ty2 = (max_point.y - ray_origin.y) / ray_direction.y;
            tmin = std::max(tmin, std::min(ty1, ty2));
            tmax = std::min(tmax, std::max(ty1, ty2));
        } else {
            if (ray_origin.y < min_point.y || ray_origin.y > max_point.y) {
                return result;  // 不相交
            }
        }
        
        // 检查Z轴
        if (std::abs(ray_direction.z) > 1e-10) {
            double tz1 = (min_point.z - ray_origin.z) / ray_direction.z;
            double tz2 = (max_point.z - ray_origin.z) / ray_direction.z;
            tmin = std::max(tmin, std::min(tz1, tz2));
            tmax = std::min(tmax, std::max(tz1, tz2));
        } else {
            if (ray_origin.z < min_point.z || ray_origin.z > max_point.z) {
                return result;  // 不相交
            }
        }
        
        // 检查是否有有效相交
        if (tmax >= tmin && tmax > 0) {
            result.hit = true;
            result.distance = tmin > 0 ? tmin : tmax;
            
            // 计算交点
            Vector3D intersection = ray_origin + ray_direction * result.distance;
            result.height = intersection.y;
            result.brightness = this->brightness;
        }
        
        return result;
    }
};

#endif // BOX_HPP
