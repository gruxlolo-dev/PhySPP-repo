#pragma once
#include "body.h"
#include <vector>

namespace Forces {

inline Vec3 gravity(const Body &a, const Body &b) {
  Vec3 r = b.pos - a.pos;
  double dist = r.length();
  if (dist < 1e-10)
    return Vec3();
  return r * (Physics::G * a.mass * b.mass / (dist * dist * dist));
}

inline Vec3 relativisticGravity(const Body &a, const Body &b) {
  Vec3 r = b.pos - a.pos;
  double dist = r.length();
  if (dist < 1e-10)
    return Vec3();

  Vec3 v = a.vel - b.vel;
  double v2 = v.lengthSq();
  double c2 = Physics::C * Physics::C;

  Vec3 F = r * (Physics::G * a.mass * b.mass / (dist * dist * dist));

  double correction =
      1.0 + v2 / c2 - 2.0 * Physics::G * (a.mass + b.mass) / (dist * c2);
  return F * correction;
}

inline Vec3 tidalForce(const Body &a, const Body &b) {
  Vec3 r = b.pos - a.pos;
  double dist = r.length();
  if (dist < 1e-10)
    return Vec3();

  double tidal =
      2.0 * Physics::G * b.mass * a.radius / (dist * dist * dist * dist);
  return r * tidal;
}

inline Vec3 drag(const Body &body, double dragCoeff = 0.1) {
  return body.vel * (-dragCoeff * body.vel.length());
}

inline Vec3 radiationPressure(const Body &source, const Body &target) {
  if (source.luminosity <= 0)
    return Vec3();
  Vec3 r = target.pos - source.pos;
  double dist = r.length();
  if (dist < 1e-10)
    return Vec3();

  double pressure = source.luminosity / (4.0 * M_PI * dist * dist * Physics::C);
  double area = M_PI * target.radius * target.radius;
  return r.normalized() * (pressure * area);
}
} // namespace Forces
