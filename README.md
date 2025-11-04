# 3D 命令行渲染引擎

一个用C++开发的轻量级3D渲染引擎，可以在命令行终端中渲染3D场景。

## 架构设计

### 核心组件

1. **Vector3D** (`vector3d.hpp`)
   - 3D向量类，支持基本的向量运算
   - 提供加法、减法、点积、归一化等操作

2. **Box** (`box.hpp`)
   - 长方体物体类
   - 实现了AABB（轴对齐包围盒）射线相交算法
   - 存储位置、大小和亮度信息

3. **ViewPoint** (`view_point.hpp`)
   - 视点类，固定在世界坐标原点 (0, 0, 0)
   - 等角度间隔发射指定数量的视线
   - 计算每条视线击中物体的高度和亮度

4. **Scene** (`scene.hpp`)
   - 场景管理类
   - 负责管理世界中的所有长方体对象

5. **Renderer** (`renderer.hpp`)
   - 命令行渲染器
   - 将3D渲染结果转换为ASCII字符显示在终端

## 工作原理

### 渲染流程

1. **视线生成**：从视点 (0,0,0) 出发，在指定的视场角内等角度间隔生成多条视线
2. **射线追踪**：每条视线与场景中的所有长方体进行相交检测
3. **深度测试**：如果一条视线击中多个物体，选择距离最近的物体
4. **结果收集**：记录击中点的高度（y坐标）和物体的亮度
5. **ASCII渲染**：根据高度和亮度将结果转换为ASCII字符显示

### 关键算法

- **AABB射线相交算法**：高效检测射线与长方体的碰撞
- **等角度采样**：在视场角范围内均匀分布视线
- **亮度映射**：将0-1的亮度值映射到不同的ASCII字符

## 编译和运行

### Windows (MSVC)
```powershell
cl /EHsc /std:c++17 /I src src/main.cpp /Fe:renderer3d.exe
renderer3d.exe
```

### Windows (MinGW/g++)
```powershell
g++ -std=c++17 -I src src/main.cpp -o renderer3d.exe
.\renderer3d.exe
```

### Linux/Mac
```bash
g++ -std=c++17 -I src src/main.cpp -o renderer3d
./renderer3d
```

## 使用示例

```cpp
#include "scene.hpp"
#include "view_point.hpp"
#include "renderer.hpp"

// 创建场景
Scene scene;

// 添加长方体（最小点、最大点、亮度）
scene.addBox(Vector3D(-5, 0, 5), Vector3D(-3, 3, 7), 0.8);
scene.addBox(Vector3D(3, 0, 6), Vector3D(5, 4, 8), 1.0);

// 创建视点（视场角90度，80条视线）
ViewPoint viewpoint(90.0, 80);

// 渲染场景（俯仰角0度）
auto results = viewpoint.render(scene.getBoxes(), 0.0);

// 显示到终端
Renderer renderer(80, 24);
renderer.draw(results);
```

## 参数说明

### ViewPoint 参数
- **viewAngle**: 水平视场角（度），默认90度
- **viewLineCount**: 视线数量，对应终端的列数，默认80

### Renderer 参数
- **width**: 终端显示宽度（字符数），默认80
- **height**: 终端显示高度（行数），默认24

### Box 参数
- **min_point**: 长方体的最小顶点坐标
- **max_point**: 长方体的最大顶点坐标
- **brightness**: 物体亮度，范围 0.0（暗）到 1.0（亮）

## 功能特性

- ✅ 固定视点在原点 (0,0,0)
- ✅ 支持多个长方体物体
- ✅ 等角度间隔视线采样
- ✅ 基于射线追踪的渲染
- ✅ 计算物体高度和亮度
- ✅ ASCII字符显示
- ✅ 支持俯仰角调节
- ✅ 简单的动画演示

## 扩展建议

1. **添加更多形状**：球体、圆柱体、三角形等
2. **光照系统**：实现简单的光照模型，如Phong光照
3. **交互控制**：支持键盘输入来移动视点或旋转视角
4. **颜色支持**：使用ANSI颜色码为物体添加颜色
5. **阴影效果**：实现简单的阴影投射
6. **性能优化**：使用空间分割结构（如八叉树）加速渲染

## 许可证

MIT License
