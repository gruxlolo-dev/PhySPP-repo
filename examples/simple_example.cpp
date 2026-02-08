#include "PhySPP.h"

int main() {

  Simulation sim(1280, 720, "PhySPP - Solar System");

  sim.addSun();
  sim.addEarth();
  sim.addMars();
  sim.addJupiter();

  sim.useRK4();
  sim.enableRelativity();
  sim.enableCollisions();

  sim.enableGlow();
  sim.enableBloom();
  sim.enableLensFlare();

  sim.setTimeStep(3600 * 24);

  sim.run();

  return 0;
}
