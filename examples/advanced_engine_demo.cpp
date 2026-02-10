#include "PhySPP.h"

int main() {
    Simulation sim(1920, 1080, "Advanced Engine Demo - All Features");

    // Centralna gwiazda (statyczna)
    auto sun = sim.physics->createBody();
    sun->pos = {0, 0, 0};
    sun->vel = {0, 0, 0};
    sun->mass = Physics::SOLAR_MASS * 2;
    sun->radius = 1e9;
    sun->flags = 1 | 2; // STATIC | NO_INTEGRATION
    sun->setColor(1.0f, 1.0f, 0.0f);
    sun->setEmissive(1.0f);

    // Planety z velocity coloring
    for (int i = 0; i < 5; i++) {
        double angle = 2 * M_PI * i / 5;
        double r = Physics::AU * (1 + i * 0.5);
        double v = std::sqrt(Physics::G * sun->mass / r);

        sim.addBodyWithVelocityColor(
            Vec3(r * cos(angle), 0, r * sin(angle)),
            Vec3(-v * sin(angle), 0, v * cos(angle)),
            Physics::EARTH_MASS * (1 + i * 0.3),
            6.371e6 * (1 + i * 0.1)
        );
    }

    // Pas asteroid z kolizjami
    for (int i = 0; i < 100; i++) {
        double angle = 2 * M_PI * i / 100;
        double r = Physics::AU * 3 + (rand() % 50000000) * 1e3;
        double v = std::sqrt(Physics::G * sun->mass / r) * (0.95 + (rand() % 100) / 1000.0);

        sim.addBodyWithVelocityColor(
            Vec3(r * cos(angle), (rand() % 1000000 - 500000) * 1e4, r * sin(angle)),
            Vec3(-v * sin(angle), 0, v * cos(angle)),
            1e20 + rand() % (int) 1e21,
            1e6 + rand() % (int) 5e5
        );
    }

    // Konfiguracja silnika
    sim.physics->setIntegrator(IntegratorType::YOSHIDA4);
    sim.physics->enableAdaptiveTimestep(true);
    sim.physics->setTimestepRange(100, 7200);
    sim.physics->enableSleeping(true);
    sim.physics->setSleepThreshold(1e3);
    sim.enableCollisions();
    sim.enableGlow();
    sim.enableBloom();
    sim.setTimeStep(3600);

    std::cout << "\n=== ADVANCED ENGINE DEMO ===\n";
    std::cout << "Features:\n";
    std::cout << "- Yoshida 4th order integrator\n";
    std::cout << "- Adaptive timestep\n";
    std::cout << "- Sleeping bodies optimization\n";
    std::cout << "- Velocity-based coloring\n";
    std::cout << "- Collision fragmentation\n";
    std::cout << "- Trail fading\n\n";

    sim.run();

    return 0;
}
