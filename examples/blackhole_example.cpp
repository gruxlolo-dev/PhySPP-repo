#include "PhySPP.h"

int main() {
    Simulation sim(1920, 1080, "Black Hole Simulation");

    sim.addBlackHole(Vec3(0, 0, 0), 100 * Physics::SOLAR_MASS);

    for (int i = 0; i < 10; i++) {
        double angle = i * 2.0 * M_PI / 10;
        double r = Physics::AU * 50;
        Vec3 pos(r * cos(angle), 0, r * sin(angle));
        double v = sqrt(Physics::G * 100 * Physics::SOLAR_MASS / r);
        Vec3 vel(-v * sin(angle), 0, v * cos(angle));
        sim.addBody(pos, vel, Physics::SOLAR_MASS, Physics::SOLAR_RADIUS);
    }

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
