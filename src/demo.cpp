#include "vector3d.hpp"
#include "box.hpp"
#include "view_point.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

// 创建一个简单的城市场景
void createCityScene(Scene& scene) {
    // 地面
    scene.addBox(Vector3D(-20, -0.5, 0), Vector3D(20, 0, 30), 0.2);
    
    // 左侧建筑群
    scene.addBox(Vector3D(-15, 0, 5), Vector3D(-12, 5, 8), 0.7);
    scene.addBox(Vector3D(-15, 0, 10), Vector3D(-12, 8, 13), 0.8);
    scene.addBox(Vector3D(-15, 0, 15), Vector3D(-12, 4, 18), 0.6);
    
    // 右侧建筑群
    scene.addBox(Vector3D(12, 0, 5), Vector3D(15, 6, 8), 0.9);
    scene.addBox(Vector3D(12, 0, 10), Vector3D(15, 7, 13), 0.85);
    scene.addBox(Vector3D(12, 0, 15), Vector3D(15, 5, 18), 0.75);
    
    // 中央高塔
    scene.addBox(Vector3D(-2, 0, 12), Vector3D(2, 12, 16), 1.0);
    
    // 前景的小建筑
    scene.addBox(Vector3D(-8, 0, 3), Vector3D(-6, 3, 5), 0.8);
    scene.addBox(Vector3D(6, 0, 3), Vector3D(8, 3, 5), 0.8);
}

// 创建一个简单的房间场景
void createRoomScene(Scene& scene) {
    // 地板
    scene.addBox(Vector3D(-10, -0.5, 0), Vector3D(10, 0, 20), 0.3);
    
    // 左墙
    scene.addBox(Vector3D(-10, 0, 0), Vector3D(-9.5, 8, 20), 0.4);
    
    // 右墙
    scene.addBox(Vector3D(9.5, 0, 0), Vector3D(10, 8, 20), 0.4);
    
    // 后墙
    scene.addBox(Vector3D(-10, 0, 19.5), Vector3D(10, 8, 20), 0.5);
    
    // 桌子
    scene.addBox(Vector3D(-3, 0, 8), Vector3D(3, 2, 12), 0.8);
    
    // 椅子
    scene.addBox(Vector3D(-1.5, 0, 5), Vector3D(1.5, 1.5, 7), 0.7);
    scene.addBox(Vector3D(-1.5, 1.5, 5), Vector3D(1.5, 3, 5.5), 0.7);
}

// 创建金字塔场景
void createPyramidScene(Scene& scene) {
    // 地面
    scene.addBox(Vector3D(-20, -0.5, 0), Vector3D(20, 0, 30), 0.2);
    
    // 金字塔（用多个盒子堆叠模拟）
    double base_size = 6.0;
    double height_per_level = 1.5;
    int levels = 8;
    
    for (int i = 0; i < levels; ++i) {
        double size = base_size * (1.0 - i * 0.12);
        double y_start = i * height_per_level;
        double y_end = y_start + height_per_level;
        double brightness = 0.4 + 0.6 * (levels - i) / levels;
        
        scene.addBox(
            Vector3D(-size/2, y_start, 10 - size/2),
            Vector3D(size/2, y_end, 10 + size/2),
            brightness
        );
    }
}

int main() {
    std::cout << "=== 3D 命令行渲染引擎 ===" << std::endl;
    std::cout << "视点位置: (0, 0, 0)" << std::endl;
    std::cout << "\n请选择场景:" << std::endl;
    std::cout << "1. 简单场景（默认）" << std::endl;
    std::cout << "2. 城市场景" << std::endl;
    std::cout << "3. 房间场景" << std::endl;
    std::cout << "4. 金字塔场景" << std::endl;
    std::cout << "\n输入场景编号 (1-4): ";
    
    int choice = 1;
    std::cin >> choice;
    
    // 创建场景
    Scene scene;
    
    switch (choice) {
        case 2:
            std::cout << "\n正在加载城市场景..." << std::endl;
            createCityScene(scene);
            break;
        case 3:
            std::cout << "\n正在加载房间场景..." << std::endl;
            createRoomScene(scene);
            break;
        case 4:
            std::cout << "\n正在加载金字塔场景..." << std::endl;
            createPyramidScene(scene);
            break;
        default:
            std::cout << "\n正在加载简单场景..." << std::endl;
            // 默认场景
            scene.addBox(Vector3D(-10, -1, 2), Vector3D(10, 0, 20), 0.3);
            scene.addBox(Vector3D(-5, 0, 5), Vector3D(-3, 3, 7), 0.8);
            scene.addBox(Vector3D(3, 0, 6), Vector3D(5, 4, 8), 1.0);
            scene.addBox(Vector3D(-1, 0, 10), Vector3D(1, 6, 12), 0.9);
            scene.addBox(Vector3D(-8, 0, 15), Vector3D(8, 2, 16), 0.5);
            break;
    }
    
    std::cout << "场景中有 " << scene.getBoxCount() << " 个长方体" << std::endl;
    
    // 询问渲染模式
    std::cout << "\n请选择渲染模式:" << std::endl;
    std::cout << "1. 静态渲染（单帧）" << std::endl;
    std::cout << "2. 动画渲染（俯仰角变化）" << std::endl;
    std::cout << "3. 动画渲染（旋转视角）" << std::endl;
    std::cout << "\n输入模式编号 (1-3): ";
    
    int mode = 1;
    std::cin >> mode;
    
    // 创建视点和渲染器
    ViewPoint viewpoint(90.0, 80);
    Renderer renderer(80, 24);
    
    if (mode == 1) {
        // 静态渲染
        std::cout << "\n渲染静态场景..." << std::endl;
        auto results = viewpoint.render(scene.getBoxes(), 0.0);
        renderer.draw(results);
        std::cout << "\n渲染完成! 按Enter键退出..." << std::endl;
        std::cin.ignore();
        std::cin.get();
    }
    else if (mode == 2) {
        // 俯仰角动画
        std::cout << "\n开始动画渲染（俯仰角变化）..." << std::endl;
        std::cout << "按Ctrl+C退出...\n" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        for (int frame = 0; frame < 60; ++frame) {
            double pitch = 20.0 * std::sin(frame * 0.15);
            auto results = viewpoint.render(scene.getBoxes(), pitch);
            renderer.draw(results);
            std::cout << "帧: " << frame << " | 俯仰角: " << pitch << "°" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    else if (mode == 3) {
        // 旋转视角（通过改变视线角度模拟）
        std::cout << "\n开始动画渲染（视角旋转）..." << std::endl;
        std::cout << "按Ctrl+C退出...\n" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        for (int frame = 0; frame < 60; ++frame) {
            double pitch = 10.0 * std::sin(frame * 0.1);
            auto results = viewpoint.render(scene.getBoxes(), pitch);
            renderer.draw(results);
            std::cout << "帧: " << frame << " | 视角: " << pitch << "°" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    std::cout << "\n渲染完成!" << std::endl;
    
    return 0;
}
