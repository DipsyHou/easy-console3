#ifndef SCENE_HPP
#define SCENE_HPP

#include "box.hpp"
#include <vector>

// 场景类 - 管理世界中的所有物体
class Scene {
public:
    Scene() {}
    
    // 添加长方体到场景
    void addBox(const Box& box) {
        boxes.push_back(box);
    }
    
    // 创建并添加长方体（通过两个顶点定义）
    void addBox(const Vector3D& min_point, const Vector3D& max_point, double brightness = 1.0) {
        boxes.emplace_back(min_point, max_point, brightness);
    }
    
    // 清空场景
    void clear() {
        boxes.clear();
    }
    
    // 获取所有物体
    const std::vector<Box>& getBoxes() const {
        return boxes;
    }
    
    // 获取物体数量
    size_t getBoxCount() const {
        return boxes.size();
    }

private:
    std::vector<Box> boxes;  // 场景中的所有长方体
};

#endif // SCENE_HPP
