#pragma once
#include "body.h"
#include <vector>

namespace Forces {

inline Vec3 gravity(const Body &a, const Body &b) {
  Vec3 r = b.pos - a.pos;
  double dist = r.length();
  if (dist < 1e-10)
    return Vec3();
  
  double softening = (a.radius + b.radius) * 0.01;
  double dist_soft = sqrt(dist*dist + softening*softening);
  
  return r * (Physics::G * a.mass * b.mass / (dist_soft * dist_soft * dist_soft));
}

inline Vec3 relativisticGravity(const Body &a, const Body &b) {
  Vec3 r = b.pos - a.pos;
  double dist = r.length();
  if (dist < 1e-10)
    return Vec3();

  const double c = Physics::C;
  double rs = 2.0 * Physics::G * b.mass / (c * c);
  
  if(dist < rs * 1.5) {
    return Vec3();
  }
  
  double v = a.vel.length();
  double beta = v / c;
  double gamma = 1.0 / sqrt(1.0 - beta * beta);
  
  Vec3 n = r.normalized();
  double v_radial = a.vel.dot(n);
  
  double pn_correction = 1.0 + (3.0 * v * v / (c * c)) + (4.0 * Physics::G * b.mass / (dist * c * c));
  double force = Physics::G * a.mass * b.mass / (dist * dist) * pn_correction;
  
  Vec3 gravForce = n * force;
  Vec3 frameDrag = Vec3(-r.y, r.x, 0) * (2.0 * Physics::G * b.mass / (c * c * dist * dist));
  
  return (gravForce + frameDrag) * gamma;
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

inline double rocheLimit(const Body& primary, const Body& satellite) {
  return 2.46 * primary.radius * pow(primary.mass / satellite.mass, 1.0/3.0);
}
}
