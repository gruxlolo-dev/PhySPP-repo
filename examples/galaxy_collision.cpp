#include "PhySPP.h"

int main() {
    Simulation sim(1280, 720, "Galaxy Collision");

    double galMass = 1e31;
    double separation = 5e11;

    sim.addBody(Vec3(-separation, 0, 0), Vec3(1e4, 0, 0), galMass, 1e9);
    sim.addBody(Vec3(separation, 0, 0), Vec3(-1e4, 0, 0), galMass, 1e9);

    for (int i = 0; i < 100; i++) {
        double angle = 2 * M_PI * i / 100;
        double r = 2e11;
        double v = 2e4;

        sim.addBodyWithVelocityColor(Vec3(-separation + r * cos(angle), 0, r * sin(angle)),
                                     Vec3(1e4 - v * sin(angle), 0, v * cos(angle)),
                                     1e28, 5e7);

        sim.addBodyWithVelocityColor(Vec3(separation + r * cos(angle), 0, r * sin(angle)),
                                     Vec3(-1e4 - v * sin(angle), 0, v * cos(angle)),
                                     1e28, 5e7);
    }

    sim.useRK4();
    sim.enableRelativity();
    sim.enableCollisions();
    sim.enableGlow();
    sim.setTimeStep(3600 * 24 * 30);
    sim.run();

    return 0;
}
