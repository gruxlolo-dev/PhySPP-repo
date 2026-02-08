#include "PhySPP.h"
#include <iostream>

int main() {
  std::cout << "PhySPP v2.0 - Full Demo\n";
  std::cout << "=======================\n\n";

  Simulation sim(1920, 1080, "PhySPP v2.0 - Full Demo");

  std::cout << "Adding celestial bodies...\n";

  sim.addSun();
  sim.addEarth();
  sim.addMars();
  sim.addJupiter();

  for (int i = 0; i < 20; i++) {
    double angle = i * 2.0 * M_PI / 20;
    double r = Physics::AU * 2.5;
    Vec3 pos(r * cos(angle), 0, r * sin(angle));
    double v = sqrt(Physics::G * Physics::SOLAR_MASS / r);
    Vec3 vel(-v * sin(angle), 0, v * cos(angle));
    sim.addAsteroid(pos, vel);
  }

  sim.addComet(Vec3(Physics::AU * 5, Physics::AU * 2, 0),
               Vec3(-20000, -10000, 0));

  std::cout << "Bodies added: " << sim.getBodyCount() << "\n\n";

  std::cout << "Configuring physics...\n";

  sim.useRK4();
  std::cout << "  - RK4 integrator (4th order accuracy)\n";

  sim.enableRelativity();
  std::cout << "  - Relativistic corrections enabled\n";

  sim.enableCollisions();
  std::cout << "  - Collision detection enabled\n";

  std::cout << "\nConfiguring graphics...\n";

  sim.enableGlow();
  std::cout << "  - Glow effect enabled\n";

  sim.enableBloom();
  std::cout << "  - Bloom effect enabled\n";

  sim.enableLensFlare();
  std::cout << "  - Lens flare enabled\n";

  sim.setTimeStep(3600 * 24);
  std::cout << "\nTime step: 1 day per frame\n";

  std::cout << "\n=== Controls ===\n";
  std::cout << "SPACE - Pause/Resume\n";
  std::cout << "Q     - Quit\n";
  std::cout << "O     - Toggle orbits\n";
  std::cout << "G     - Toggle grid\n";
  std::cout << "+/-   - Time scale\n";
  std::cout << "W/S   - Zoom\n";
  std::cout << "Arrows - Rotate camera\n";
  std::cout << "0-9   - Focus on body\n";
  std::cout << "\nStarting simulation...\n\n";

  sim.run();

  std::cout << "\nSimulation ended.\n";
  std::cout << "Final body count: " << sim.getBodyCount() << "\n";
  std::cout << "Total energy: " << sim.getTotalEnergy() << " J\n";

  return 0;
}
