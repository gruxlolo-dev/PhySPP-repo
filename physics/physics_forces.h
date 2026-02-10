#pragma once
#include "physics_body.h"
#include "../core/vec3.h"
#include "../core/constants.h"

namespace PhysicsForces {
  inline Vec3 gravity(const PhysicsBody &a, const PhysicsBody &b) {
    Vec3 r = b.position - a.position;
    double dist = r.length();
    if (dist < 1e-10) return Vec3(0, 0, 0);
    
    double forceMag = Physics::G * a.mass * b.mass / (dist * dist);
    return r.normalized() * forceMag;
  }
}
