#include "PhySPP.h"
#include <iostream>

int main() {
    std::cout << "Milky Way Galaxy Simulation\n";

    Simulation sim(1920, 1080, "Milky Way Galaxy");

    sim.addBlackHole(Vec3(0, 0, 0), 4.1e6 * Physics::SOLAR_MASS);

    for (int i = 0; i < 500; i++) {
        double angle = i * 2.0 * M_PI / 500 + (i / 100.0) * M_PI;
        double r = Physics::AU * 1000 * (1 + i * 0.01);
        double z = ((rand() % 1000) - 500) * Physics::AU * 50;
        Vec3 pos(r * cos(angle), r * sin(angle), z);

        double v = sqrt(Physics::G * 4.1e6 * Physics::SOLAR_MASS / r) *
                   (0.8 + (rand() % 40) / 100.0);
        Vec3 vel(-v * sin(angle), v * cos(angle), 0);

        sim.addBody(pos, vel, Physics::SOLAR_MASS * (0.5 + (rand() % 150) / 100.0),
                    Physics::SOLAR_RADIUS * (0.5 + (rand() % 100) / 100.0));
    }

    sim.useVerlet();
    sim.enableGlow();
    sim.setTimeStep(3600 * 24 * 365);

    sim.run();
    return 0;
}
