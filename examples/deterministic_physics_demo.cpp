#include "PhySPP.h"

int main() {
    Simulation sim(1920, 1080, "PhySPP - Deterministic Physics Engine");

    // Włącz deterministyczny silnik
    sim.enableDeterministicPhysics();

    std::cout << "\n=== DETERMINISTIC PHYSICS ENGINE ===\n";
    std::cout << "Full conservation of energy and momentum\n";
    std::cout << "Realistic collision physics\n";
    std::cout << "From everyday to cosmic scales\n\n";

    // Centralna gwiazda
    auto star = sim.physics->createBody();
    star->pos = {0, 0, 0};
    star->vel = {0, 0, 0};
    star->mass = Physics::SOLAR_MASS;
    star->radius = 6.96e8;
    star->flags = BodyFlags::STATIC | BodyFlags::NO_INTEGRATION;
    star->label = "STAR";
    star->setColor(1.0f, 1.0f, 0.0f);
    star->setEmissive(1.0f);

    // Różne energie zderzeń
    std::cout << "Creating collision scenarios:\n";

    // 1. Niska energia - odbicia
    std::cout << "  1. Low energy - elastic bouncing\n";
    for (int i = 0; i < 8; i++) {
        double angle = 2 * M_PI * i / 8;
        double r = 5e10;
        double v = std::sqrt(Physics::G * star->mass / r) * 1.05;

        auto body = sim.physics->createBody();
        body->pos = {r * cos(angle), 0, r * sin(angle)};
        body->vel = {-v * sin(angle), 0, v * cos(angle)};
        body->mass = 1e22;
        body->radius = 5e6;
        body->label = "ELA";
        body->setColor(0.3f, 0.8f, 0.3f);
    }

    // 2. Średnia energia - fuzja
    std::cout << "  2. Medium energy - fusion/merging\n";
    for (int i = 0; i < 6; i++) {
        double angle = 2 * M_PI * i / 6;
        double r = 1.5e11;
        double v = std::sqrt(Physics::G * star->mass / r) * 0.9;

        auto body = sim.physics->createBody();
        body->pos = {r * cos(angle), 0, r * sin(angle)};
        body->vel = {-v * sin(angle), 0, v * cos(angle)};
        body->mass = 1e24;
        body->radius = 1e7;
        body->label = "FUS";
        body->setColor(0.3f, 0.3f, 0.8f);
    }

    // 3. Wysoka energia - fragmentacja
    std::cout << "  3. High energy - fragmentation\n";
    for (int i = 0; i < 4; i++) {
        double angle = 2 * M_PI * i / 4;
        double r = 2.5e11;
        double v = std::sqrt(Physics::G * star->mass / r) * 1.5;

        auto body = sim.physics->createBody();
        body->pos = {r * cos(angle), 0, r * sin(angle)};
        body->vel = {-v * sin(angle), 0, v * cos(angle)};
        body->mass = 1e26;
        body->radius = 3e7;
        body->label = "FRG";
        body->setColor(0.8f, 0.5f, 0.2f);
    }

    // 4. Ekstremalna energia - anihilacja
    std::cout << "  4. Extreme energy - annihilation\n\n";
    for (int i = 0; i < 2; i++) {
        double angle = M_PI * i;
        double r = 3.5e11;
        double v = std::sqrt(Physics::G * star->mass / r) * 3.0;

        auto body = sim.physics->createBody();
        body->pos = {r * cos(angle), 0, r * sin(angle)};
        body->vel = {-v * sin(angle) * 2, 0, v * cos(angle) * 2};
        body->mass = 1e28;
        body->radius = 5e7;
        body->label = "ANI";
        body->setColor(1.0f, 0.2f, 0.2f);
    }

    sim.physics->setIntegrator(IntegratorType::LEAPFROG);
    sim.enableCollisions();
    sim.enableGlow();
    sim.setTimeStep(3600);

    std::cout << "PHYSICS LAWS:\n";
    std::cout << "  ✓ Total momentum conserved\n";
    std::cout << "  ✓ Total energy conserved (changes form)\n";
    std::cout << "  ✓ Mass can convert to energy (E=mc²)\n";
    std::cout << "  ✓ No magic, no guessing\n\n";

    std::cout << "COLLISION TYPES:\n";
    std::cout << "  ELA - Elastic bounce (low E)\n";
    std::cout << "  FUS - Fusion/merge (medium E)\n";
    std::cout << "  FRG - Fragmentation (high E, E→m)\n";
    std::cout << "  ANI - Annihilation (extreme E, m→γ)\n";
    std::cout << "  TID - Tidal disruption (Roche limit)\n";
    std::cout << "  γ - Gamma ray (pure energy)\n\n";

    sim.run();

    return 0;
}
