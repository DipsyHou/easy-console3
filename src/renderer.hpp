#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "view_point.hpp"
#include "box.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

// 命令行渲染器 - 将3D场景渲染到终端
class Renderer {
public:
    Renderer(int width = 80, int height = 24) 
        : screen_width(width), screen_height(height) {}
    
    // 将渲染结果转换为ASCII字符显示
    void draw(const std::vector<ViewPoint::RayResult>& ray_results) {
        // 清屏
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
        
        // 找到最大和最小高度，用于归一化
        double min_height = 1e9, max_height = -1e9;
        for (const auto& result : ray_results) {
            if (result.hit) {
                min_height = std::min(min_height, result.height);
                max_height = std::max(max_height, result.height);
            }
        }
        
        // 如果没有击中任何物体，显示空场景
        if (min_height > max_height) {
            std::cout << "Empty scene - no objects visible" << std::endl;
            return;
        }
        
        // 创建屏幕缓冲区
        std::vector<std::string> screen(screen_height, std::string(screen_width, ' '));
        
        // 渲染每条视线
        for (size_t i = 0; i < ray_results.size() && i < (size_t)screen_width; ++i) {
            const auto& result = ray_results[i];
            
            if (result.hit) {
                // 归一化高度到屏幕高度
                double normalized_height = (result.height - min_height) / (max_height - min_height);
                int column_height = static_cast<int>(normalized_height * screen_height);
                column_height = std::max(1, std::min(column_height, screen_height));
                
                // 根据亮度选择字符
                char display_char = getBrightnessChar(result.brightness);
                
                // 绘制列（从下往上）
                for (int row = 0; row < column_height; ++row) {
                    int screen_row = screen_height - 1 - row;
                    if (screen_row >= 0 && screen_row < screen_height) {
                        screen[screen_row][i] = display_char;
                    }
                }
            }
        }
        
        // 输出屏幕
        for (const auto& line : screen) {
            std::cout << line << std::endl;
        }
        
        // 显示统计信息
        std::cout << "\nHeight range: [" << min_height << ", " << max_height << "]" << std::endl;
    }
    
    // 设置屏幕大小
    void setScreenSize(int width, int height) {
        screen_width = width;
        screen_height = height;
    }

private:
    int screen_width;
    int screen_height;
    
    // 根据亮度返回对应的ASCII字符
    char getBrightnessChar(double brightness) const {
        // 亮度字符集（从暗到亮）
        const char brightness_chars[] = " .:-=+*#%@";
        int num_chars = sizeof(brightness_chars) - 1;
        
        int index = static_cast<int>(brightness * (num_chars - 1));
        index = std::max(0, std::min(index, num_chars - 1));
        
        return brightness_chars[index];
    }
};

#endif // RENDERER_HPP
