#include "PhySPP.h"

int main() {
    Simulation sim(1280, 720, "Asteroid Belt");

    sim.addSun();

    for (int i = 0; i < 200; i++) {
        double angle = 2 * M_PI * i / 200;
        double r = 3e11 + (rand() % 100000000) * 1e3;
        double v = sqrt(6.67e-11 * 2e30 / r);

        sim.addBodyWithVelocityColor(Vec3(r * cos(angle), (rand() % 2000000 - 1000000) * 1e4, r * sin(angle)),
                                     Vec3(-v * sin(angle), 0, v * cos(angle)),
                                     1e18 + rand() % (int) 1e19, 1e5 + rand() % (int) 5e5);
    }

    sim.useRK4();
    sim.enableCollisions();
    sim.enableGlow();
    sim.setTimeStep(3600 * 24);
    sim.run();

    return 0;
}
