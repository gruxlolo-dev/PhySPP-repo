#include "PhySPP.h"

int main() {
  Simulation sim(1280, 720, "Earth-Moon System");

  double earthMass = 5.972e24;
  double moonMass = 7.342e22;
  double distance = 3.844e8;
  double moonVel = 1022;

  sim.addBody(Vec3(0, 0, 0), Vec3(0, 0, 0), earthMass, 6.371e6);
  sim.addBody(Vec3(distance, 0, 0), Vec3(0, moonVel, 0), moonMass, 1.737e6);

  for(int i = 0; i < 10; i++) {
    double angle = 2 * M_PI * i / 10;
    double r = distance * 0.3;
    double v = sqrt(6.67e-11 * earthMass / r);
    
    sim.addBodyWithVelocityColor(Vec3(r * cos(angle), 0, r * sin(angle)),
                Vec3(-v * sin(angle), 0, v * cos(angle)),
                1e15, 1e4);
  }

  sim.useRK4();
  sim.enableRelativity();
  sim.enableCollisions();
  sim.enableGlow();
  sim.enableBloom();
  sim.setTimeStep(60);
  sim.run();

  return 0;
}
