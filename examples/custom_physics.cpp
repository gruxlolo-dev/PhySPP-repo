#include "PhySPP.h"
#include <iostream>

int main() {
    PhysicsEngine engine;

    engine.setIntegrator(IntegratorType::LEAPFROG);
    engine.enableRelativistic(false);
    engine.enableCollisions(false);
    engine.enableConstraints(false);
    engine.enableSleeping(false);

    auto sun = engine.createBody();
    sun->pos = {0, 0, 0};
    sun->vel = {0, 0, 0};
    sun->mass = Physics::SOLAR_MASS;
    sun->flags = BodyFlags::STATIC | BodyFlags::NO_INTEGRATION;

    double r = Physics::AU;
    double v = std::sqrt(Physics::G * Physics::SOLAR_MASS / r);

    auto earth = engine.createBody();
    earth->pos = {r, 0, 0};
    earth->vel = {0, v, 0};
    earth->mass = Physics::EARTH_MASS;
    earth->flags = static_cast<int>(BodyFlags::GRAVITY_ONLY);

    engine.finalize();

    constexpr double dt = 3600.0;
    constexpr long long years = 100;
    constexpr long long stepsPerYear = static_cast<long long>(365.25 * 24);
    constexpr long long totalSteps = years * stepsPerYear;

    std::cout << "Simulating " << years << " years...\n";

    for (long long i = 0; i < totalSteps; ++i) {
        engine.stepFast(dt);
        if (i % 10000 == 0) {
            std::cout << "\rProgress: " << (i * 100 / totalSteps) << "%" << std::flush;
        }
    }

    std::cout << "\n\nFinal r = " << earth->pos.length() / Physics::AU << " AU\n";
    std::cout << "Final v = " << earth->vel.length() << " m/s\n";
    std::cout << "Expected v = " << v << " m/s\n";

    return 0;
}
