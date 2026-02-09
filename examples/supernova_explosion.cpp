#include "PhySPP.h"

int main() {
  Simulation sim(1280, 720, "Supernova Explosion");

  double centerMass = 1e31;
  sim.addBody(Vec3(0, 0, 0), Vec3(0, 0, 0), centerMass, 1e9);

  for(int i = 0; i < 150; i++) {
    double theta = acos(2.0 * (rand() / (double)RAND_MAX) - 1.0);
    double phi = 2 * M_PI * (rand() / (double)RAND_MAX);
    
    double vx = sin(theta) * cos(phi) * 5e5;
    double vy = sin(theta) * sin(phi) * 5e5;
    double vz = cos(theta) * 5e5;
    
    double px = sin(theta) * cos(phi) * 1e8;
    double py = sin(theta) * sin(phi) * 1e8;
    double pz = cos(theta) * 1e8;
    
    sim.addBodyWithVelocityColor(Vec3(px, py, pz), Vec3(vx, vy, vz), 1e27, 1e7);
  }

  sim.useRK4();
  sim.enableRelativity();
  sim.enableCollisions();
  sim.enableGlow();
  sim.enableBloom();
  sim.setTimeStep(100);
  sim.run();

  return 0;
}
