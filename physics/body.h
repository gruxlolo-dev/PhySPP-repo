#pragma once
#include "../core/constants.h"
#include "../core/vec3.h"
#include <memory>
#include <string>
#include <vector>

enum class BodyType {
  STAR,
  PLANET,
  MOON,
  ASTEROID,
  COMET,
  BLACK_HOLE,
  NEUTRON_STAR
};

class Body {
public:
  Vec3 pos, vel, acc;
  double mass, radius;
  BodyType type;
  std::string name;
  std::string label;

  float color[3] = {1, 1, 1};
  float emissive = 0.0f;
  double renderScale = 1.0;
  bool showTrail = true;
  std::vector<Vec3> trail;
  bool isFragment = false;
  bool colorByVelocity = false;
  double maxVelocityForColor = 1e5;
  int flags = 0;

  double temperature = 0;
  double luminosity = 0;
  double elasticity = 0.5;
  double friction = 0.1;
  bool destroyed = false;
  double kineticEnergyAtCollision = 0;
  
  enum class CollisionType {
    NONE,
    ELASTIC_BOUNCE,
    MERGE,
    FRAGMENTATION,
    ANNIHILATION,
    NUCLEAR_REACTION
  };
  CollisionType lastCollisionType = CollisionType::NONE;

  Body(Vec3 pos, Vec3 vel, double mass, double radius,
       BodyType type = BodyType::PLANET)
      : pos(pos), vel(vel), acc(0, 0, 0), mass(mass), radius(radius),
        type(type) {}

  double kineticEnergy() const { return 0.5 * mass * vel.lengthSq(); }
  Vec3 momentum() const { return vel * mass; }
  Vec3 angularMomentum(const Vec3 &origin) const {
    return (pos - origin).cross(momentum());
  }

  double schwarzschildRadius() const {
    return 2.0 * Physics::G * mass / (Physics::C * Physics::C);
  }

  double escapeVelocity() const {
    return std::sqrt(2.0 * Physics::G * mass / radius);
  }

  double surfaceGravity() const {
    return Physics::G * mass / (radius * radius);
  }

  void updateTrail(int maxPoints = 1000) {
    if (showTrail) {
      trail.push_back(pos);
      if (trail.size() > maxPoints)
        trail.erase(trail.begin());
    }
  }

  void updateColorByVelocity() {
    if (colorByVelocity && !destroyed) {
      double speed = vel.length();
      if (std::isfinite(speed) && speed >= 0) {
        float intensity = std::min(1.0, speed / maxVelocityForColor);
        color[0] = 0.2f + intensity * 0.8f;
        color[1] = 0.2f + (1.0f - intensity) * 0.6f;
        color[2] = 0.2f;
        emissive = intensity * 0.5f;
      }
    }
  }

  void setColor(float r, float g, float b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
  }
  void setEmissive(float e) { emissive = e; }
  void destroy() { destroyed = true; }
};

using BodyPtr = std::shared_ptr<Body>;
