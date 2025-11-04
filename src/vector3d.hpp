#ifndef VECTOR3D_HPP
#define VECTOR3D_HPP

#include <cmath>

// 3D向量类
struct Vector3D {
    double x, y, z;
    
    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
    
    // 向量加法
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }
    
    // 向量减法
    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }
    
    // 标量乘法
    Vector3D operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }
    
    // 点积
    double dot(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    // 向量长度
    double length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    // 归一化
    Vector3D normalize() const {
        double len = length();
        if (len < 1e-10) return Vector3D(0, 0, 0);
        return Vector3D(x / len, y / len, z / len);
    }
};

#endif // VECTOR3D_HPP
