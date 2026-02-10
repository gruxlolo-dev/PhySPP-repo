#include "PhySPP.h"
#include <iostream>

int main() {
    std::cout << "Complete Solar System Simulation\n";
    std::cout << "================================\n\n";

    Simulation sim(1920, 1080, "Complete Solar System");

    std::cout << "Adding Sun...\n";
    sim.addSun();

    std::cout << "Adding inner planets...\n";

    sim.addBody(Vec3(0.387 * Physics::AU, 0, 0), Vec3(0, 47870, 0), 3.285e23,
                2.4397e6);

    sim.addBody(Vec3(0.723 * Physics::AU, 0, 0), Vec3(0, 35020, 0), 4.867e24,
                6.0518e6);

    sim.addEarth();

    sim.addMars();

    std::cout << "Adding outer planets...\n";

    sim.addJupiter();

    sim.addBody(Vec3(9.537 * Physics::AU, 0, 0), Vec3(0, 9690, 0), 5.683e26,
                5.8232e7);

    sim.addBody(Vec3(19.191 * Physics::AU, 0, 0), Vec3(0, 6810, 0), 8.681e25,
                2.5362e7);

    sim.addBody(Vec3(30.069 * Physics::AU, 0, 0), Vec3(0, 5430, 0), 1.024e26,
                2.4622e7);

    std::cout << "Adding asteroid belt...\n";

    for (int i = 0; i < 50; i++) {
        double angle = i * 2.0 * M_PI / 50;
        double r = Physics::AU * (2.2 + (rand() % 100) / 100.0);
        Vec3 pos(r * cos(angle), 0, r * sin(angle));
        double v = sqrt(Physics::G * Physics::SOLAR_MASS / r);
        Vec3 vel(-v * sin(angle), 0, v * cos(angle));
        sim.addAsteroid(pos, vel);
    }

    std::cout << "\nTotal bodies: " << sim.getBodyCount() << "\n";
    std::cout << "\nConfiguring simulation...\n";

    sim.useRK4();
    std::cout << "  - RK4 integrator\n";

    sim.enableRelativity();
    std::cout << "  - Relativistic effects\n";

    sim.enableCollisions();
    std::cout << "  - Collision detection\n";

    sim.enableGlow();
    std::cout << "  - Glow effects\n";

    sim.setTimeStep(3600 * 24);
    std::cout << "  - Time step: 1 day\n";

    std::cout << "\n=== Controls ===\n";
    std::cout << "Mouse  - Rotate camera\n";
    std::cout << "W/S    - Zoom\n";
    std::cout << "SPACE  - Pause\n";
    std::cout << "0-9    - Focus on body\n";
    std::cout << "Q      - Quit\n\n";

    std::cout << "Starting simulation...\n\n";

    sim.run();

    return 0;
}
