#include "vector3d.hpp"
#include "box.hpp"
#include "view_point.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "=== 3D 命令行渲染引擎 ===" << std::endl;
    std::cout << "视点位置: (0, 0, 0)" << std::endl << std::endl;
    
    // 创建场景
    Scene scene;
    
    // 添加一些测试用的长方体
    // 第一个盒子：地面
    // scene.addBox(
    //     Vector3D(-1, -1, -1),   // 最小点
    //     Vector3D(1, 1, 1),    // 最大点
    //     1.0                    // 亮度
    // );
    
    // // 第二个盒子：左侧立方体
    // scene.addBox(
    //     Vector3D(-5, 0, 5),
    //     Vector3D(-3, 3, 7),
    //     0.8
    // );
    
    // // 第三个盒子：右侧立方体
    scene.addBox(
        Vector3D(3, 0, 6),
        Vector3D(5, 4, 8),
        1.0
    );
    
    // // 第四个盒子：中间高塔
    // scene.addBox(
    //     Vector3D(-1, 0, 10),
    //     Vector3D(1, 6, 12),
    //     0.9
    // );
    
    // // 第五个盒子：远处的墙
    // scene.addBox(
    //     Vector3D(-8, 0, 15),
    //     Vector3D(8, 2, 16),
    //     0.5
    // );
    
    std::cout << "场景中有 " << scene.getBoxCount() << " 个长方体" << std::endl;
    
    // 创建视点（视场角90度，80条视线）
    ViewPoint viewpoint(90.0, 80);
    
    // 创建渲染器（80列x24行）
    Renderer renderer(80, 24);
    
    // 静态渲染场景（俯仰角为0度，水平向前看）
    std::cout << "\n正在渲染场景...\n" << std::endl;
    
    double pitch = 0;  // 俯仰角：0度表示水平向前
    
    // 渲染场景
    auto results = viewpoint.render(scene.getBoxes(), pitch);
    
    // 显示到终端
    renderer.draw(results);
    
    std::cout << "俯仰角: " << pitch << "°" << std::endl;
    std::cout << "\n渲染完成!" << std::endl;
    
    return 0;
}
