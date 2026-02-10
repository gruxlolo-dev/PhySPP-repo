#include "PhySPP.h"
#include <iostream>
#include <chrono>

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
    sun->radius = 6.96e8;
    sun->flags = 1 | 2; // STATIC | NO_INTEGRATION

    double r = Physics::AU;
    double v = std::sqrt(Physics::G * Physics::SOLAR_MASS / r);

    auto earth = engine.createBody();
    earth->pos = {r, 0, 0};
    earth->vel = {0, v, 0};
    earth->mass = Physics::EARTH_MASS;
    earth->radius = 6.371e6;
    earth->flags = 4; // GRAVITY_ONLY

    engine.finalize();

    constexpr double dt = 3600.0;
    constexpr long long years = 100;
    constexpr long long stepsPerYear = static_cast<long long>(365.25 * 24);
    constexpr long long totalSteps = years * stepsPerYear;

    std::cout << "\n=== PERFORMANCE TEST ===\n";
    std::cout << "Simulating " << years << " years\n";
    std::cout << "Time step: " << dt << " s (1 hour)\n";
    std::cout << "Total steps: " << totalSteps << "\n\n";

    auto start = std::chrono::high_resolution_clock::now();

    for (long long i = 0; i < totalSteps; ++i) {
        engine.stepFast(dt);

        if (i % 10000 == 0) {
            std::cout << "\rProgress: " << (i * 100 / totalSteps) << "%" << std::flush;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "\n\n=== RESULTS ===\n";
    std::cout << "Simulation time: " << duration.count() / 1000.0 << " seconds\n";
    std::cout << "Steps per second: " << totalSteps / (duration.count() / 1000.0) << "\n";
    std::cout << "Final distance: " << earth->pos.length() / Physics::AU << " AU\n";
    std::cout << "Final velocity: " << earth->vel.length() << " m/s\n";
    std::cout << "Expected velocity: " << v << " m/s\n";
    std::cout << "Error: " << std::abs(earth->vel.length() - v) / v * 100 << "%\n";

    return 0;
}
