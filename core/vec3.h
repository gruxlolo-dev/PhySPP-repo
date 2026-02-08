#pragma once
#include <cmath>
#include <ostream>

struct Vec3 {
  double x, y, z;

  Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

  Vec3 operator+(const Vec3 &v) const { return {x + v.x, y + v.y, z + v.z}; }
  Vec3 operator-(const Vec3 &v) const { return {x - v.x, y - v.y, z - v.z}; }
  Vec3 operator*(double s) const { return {x * s, y * s, z * s}; }
  Vec3 operator/(double s) const { return {x / s, y / s, z / s}; }

  Vec3 &operator+=(const Vec3 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
  Vec3 &operator-=(const Vec3 &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }
  Vec3 &operator*=(double s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
  }

  bool operator==(const Vec3 &v) const {
    return x == v.x && y == v.y && z == v.z;
  }

  double dot(const Vec3 &v) const { return x * v.x + y * v.y + z * v.z; }
  Vec3 cross(const Vec3 &v) const {
    return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
  }

  double length() const { return std::sqrt(x * x + y * y + z * z); }
  double lengthSq() const { return x * x + y * y + z * z; }
  Vec3 normalized() const {
    double l = length();
    return l > 0 ? *this / l : Vec3();
  }

  friend std::ostream &operator<<(std::ostream &os, const Vec3 &v) {
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  }
};

inline Vec3 operator*(double s, const Vec3 &v) { return v * s; }
