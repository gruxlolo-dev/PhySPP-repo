#include "PhySPP.h"
#include <iostream>

int main() {
  std::cout << "Custom Physics Demo\n\n";

  PhysicsEngine engine;

  engine.setIntegrator(IntegratorType::RK4);
  engine.enableRelativistic(true);
  engine.enableCollisions(true);

  auto sun =
      std::make_shared<Body>(Vec3(0, 0, 0), Vec3(0, 0, 0), Physics::SOLAR_MASS,
                             Physics::SOLAR_RADIUS, BodyType::STAR);
  sun->setColor(1.0f, 0.9f, 0.2f);
  sun->setEmissive(1.0f);
  engine.add(sun);

  double r = Physics::AU;
  double v = sqrt(Physics::G * Physics::SOLAR_MASS / r);
  auto planet =
      std::make_shared<Body>(Vec3(r, 0, 0), Vec3(0, v, 0), Physics::EARTH_MASS,
                             Physics::EARTH_RADIUS, BodyType::PLANET);
  planet->setColor(0.2f, 0.8f, 0.3f);
  planet->elasticity = 0.9;
  planet->friction = 0.1;
  engine.add(planet);

  std::cout << "Simulating 1 year...\n";
  double dt = 3600 * 24;

  for (int day = 0; day < 365; day++) {
    engine.step(dt);

    if (day % 30 == 0) {
      std::cout << "Day " << day << ": ";
      std::cout << "Distance = " << planet->pos.length() / Physics::AU
                << " AU, ";
      std::cout << "Energy = " << engine.totalEnergy() << " J\n";
    }
  }

  std::cout << "\nFinal position: " << planet->pos << "\n";
  std::cout << "Final velocity: " << planet->vel << "\n";
  std::cout << "Total energy: " << engine.totalEnergy() << " J\n";

  return 0;
}
