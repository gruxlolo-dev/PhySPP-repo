#include "PhySPP.h"

int main() {
    Simulation sim(1280, 720, "Binary Star System");

    double starMass = 2e30;
    double separation = 1.5e11;
    double orbitalVel = 3e4;

    sim.addBody(Vec3(-separation / 2, 0, 0), Vec3(0, orbitalVel, 0), starMass, 7e8);
    sim.addBody(Vec3(separation / 2, 0, 0), Vec3(0, -orbitalVel, 0), starMass, 7e8);

    for (int i = 0; i < 50; i++) {
        double angle = 2 * M_PI * i / 50;
        double r = separation * 3;
        sim.addBodyWithVelocityColor(Vec3(r * cos(angle), 0, r * sin(angle)),
                                     Vec3(-2e4 * sin(angle), 0, 2e4 * cos(angle)),
                                     1e24, 5e6);
    }

    sim.useRK4();
    sim.enableRelativity();
    sim.enableCollisions();
    sim.enableGlow();
    sim.setTimeStep(3600 * 12);
    sim.run();

    return 0;
}
