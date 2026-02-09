#include "PhySPP.h"
#include <iostream>

int main() {
  std::cout << "Bouncing Ball Simulation\n";

  Simulation sim(1280, 720, "Bouncing Ball");

  auto ground = std::make_shared<Body>(Vec3(0, -1e7, 0), Vec3(0, 0, 0), 1e30,
                                       5e6, BodyType::PLANET);
  ground->setColor(0.3f, 0.3f, 0.3f);
  ground->elasticity = 0.9;

  auto ball = std::make_shared<Body>(Vec3(0, 5e7, 0), Vec3(0, 0, 0), 1e20, 1e6,
                                     BodyType::ASTEROID);
  ball->setColor(1.0f, 0.2f, 0.2f);
  ball->elasticity = 0.9;

  sim.addBodyWithVelocityColor(Vec3(0, -1e7, 0), Vec3(0, 0, 0), 1e30, 5e6);
  sim.addBodyWithVelocityColor(Vec3(0, 5e7, 0), Vec3(0, 0, 0), 1e20, 1e6);
  sim.addBodyWithVelocityColor(Vec3(2e7, 6e7, 0), Vec3(0, 0, 0), 1e20, 1e6);
  sim.addBodyWithVelocityColor(Vec3(-2e7, 7e7, 0), Vec3(0, 0, 0), 1e20, 1e6);

  sim.useVerlet();
  sim.enableCollisions();
  sim.setTimeStep(10);

  sim.run();
  return 0;
}
