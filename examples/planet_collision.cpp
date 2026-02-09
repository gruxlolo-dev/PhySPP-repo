#include "PhySPP.h"
#include <iostream>

int main() {
  std::cout << "Planet Collision Simulation\n";

  Simulation sim(1920, 1080, "Planet Collision");

  sim.addBodyWithVelocityColor(Vec3(-Physics::AU * 0.5, 0, 0), Vec3(15000, 0, 0),
              Physics::EARTH_MASS, Physics::EARTH_RADIUS);

  sim.addBodyWithVelocityColor(Vec3(Physics::AU * 0.5, 0, 0), Vec3(-15000, 0, 0),
              Physics::EARTH_MASS * 1.5, Physics::EARTH_RADIUS * 1.2);

  sim.addBodyWithVelocityColor(Vec3(Physics::AU * 0.5 + 5e7, 0, 0), Vec3(-15000, 1000, 0),
              Physics::EARTH_MASS * 0.01, Physics::EARTH_RADIUS * 0.27);

  sim.useRK4();
  sim.enableCollisions();
  sim.enableGlow();
  sim.setTimeStep(3600);

  sim.run();
  return 0;
}
