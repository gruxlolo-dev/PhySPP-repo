#include "PhySPP.h"
#include <iostream>

int main() {
  std::cout << "Planet Collision with Camera Tracking\n";
  std::cout << "=====================================\n\n";

  Simulation sim(1920, 1080, "Planet Collision - Camera Tracking");

  std::cout << "Setting up collision scenario...\n";

  auto planet1 = std::make_shared<Body>(
      Vec3(-Physics::AU * 0.3, 0, 0), Vec3(20000, 0, 0), Physics::EARTH_MASS,
      Physics::EARTH_RADIUS, BodyType::PLANET);
  planet1->setColor(0.2f, 0.5f, 1.0f);
  planet1->name = "Planet A";

  auto planet2 = std::make_shared<Body>(
      Vec3(Physics::AU * 0.3, 0, 0), Vec3(-18000, 0, 0),
      Physics::EARTH_MASS * 0.8, Physics::EARTH_RADIUS * 0.9, BodyType::PLANET);
  planet2->setColor(0.9f, 0.3f, 0.1f);
  planet2->name = "Planet B";

  sim.addBody(Vec3(-Physics::AU * 0.3, 0, 0), Vec3(20000, 0, 0),
              Physics::EARTH_MASS, Physics::EARTH_RADIUS);
  sim.addBody(Vec3(Physics::AU * 0.3, 0, 0), Vec3(-18000, 0, 0),
              Physics::EARTH_MASS * 0.8, Physics::EARTH_RADIUS * 0.9);

  sim.addBody(Vec3(Physics::AU * 0.3 + 5e7, 0, 0), Vec3(-18000, 1500, 0),
              Physics::EARTH_MASS * 0.01, Physics::EARTH_RADIUS * 0.27);

  std::cout << "Planets configured:\n";
  std::cout << "  - Planet A (Blue): Earth-like\n";
  std::cout << "  - Planet B (Red): Mars-like\n";
  std::cout << "  - Small Moon\n\n";

  std::cout << "Physics settings:\n";
  sim.useRK4();
  std::cout << "  - RK4 integrator (high accuracy)\n";

  sim.enableCollisions();
  std::cout << "  - Collision detection ON\n";

  sim.enableGlow();
  std::cout << "  - Glow effects ON\n";

  sim.setTimeStep(600);
  std::cout << "  - Time step: 10 minutes\n\n";

  std::cout << "=== Camera Controls ===\n";
  std::cout << "Mouse  - Rotate view\n";
  std::cout << "W/S    - Zoom in/out\n";
  std::cout << "0      - Follow Planet A\n";
  std::cout << "1      - Follow Planet B\n";
  std::cout << "2      - Follow Moon\n";
  std::cout << "SPACE  - Pause\n";
  std::cout << "Q      - Quit\n\n";

  std::cout << "Starting collision simulation...\n";
  std::cout << "Watch as planets collide!\n\n";

  sim.run();

  return 0;
}
