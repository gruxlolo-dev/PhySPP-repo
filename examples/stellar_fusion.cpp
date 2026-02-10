#include "PhySPP.h"
#include <iostream>

const double SCALE_MASS = 1e27;
const double PROTON_MASS = 1.673e-27 * SCALE_MASS;
const double DEUTERON_MASS = 2 * PROTON_MASS;
const double HE3_MASS = 3 * PROTON_MASS;
const double HE4_MASS = 4 * PROTON_MASS;

int main() {
    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║     STELLAR FUSION - PP CHAIN                        ║\n";
    std::cout << "║     Nuclear Fusion in Stars (like the Sun)          ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n\n";

    Simulation sim(1920, 1080, "Stellar Fusion - PP Chain");

    sim.physics->setIntegrator(IntegratorType::LEAPFROG);
    sim.enableCollisions();
    sim.enableGlow();
    sim.enableBloom();
    sim.setTimeStep(3600.0);
    sim.physics->setTimeScale(0.15);

    sim.renderer->setCamera(Vec3(0, 1.5e11, 3e11), Vec3(0, 0, 0));

    std::cout << "⭐ PROTON-PROTON CHAIN (PP Chain)\n";
    std::cout << "The main fusion process in stars like the Sun\n\n";

    std::cout << "=== STEP 1: p + p → Deuteron (D) ===\n";
    std::cout << "Two protons fuse to form deuterium\n\n";

    // Krok 1: p + p → D (góra)
    for (int i = 0; i < 4; i++) {
        auto p1 = sim.physics->createBody();
        p1->pos = {-4e10, 8e10 + i * 2e10, 0};
        p1->vel = {3e7, 0, 0};
        p1->mass = PROTON_MASS;
        p1->radius = 1.5e9;
        p1->label = "p";
        p1->setColor(1.0f, 0.3f, 0.3f);
        p1->setEmissive(0.5f);
        p1->elasticity = 0.1;

        auto p2 = sim.physics->createBody();
        p2->pos = {4e10, 8e10 + i * 2e10, 0};
        p2->vel = {-3e7, 0, 0};
        p2->mass = PROTON_MASS;
        p2->radius = 1.5e9;
        p2->label = "p";
        p2->setColor(1.0f, 0.3f, 0.3f);
        p2->setEmissive(0.5f);
        p2->elasticity = 0.1;
    }

    std::cout << "=== STEP 2: D + p → Helium-3 (He³) ===\n";
    std::cout << "Deuteron captures another proton\n\n";

    // Krok 2: D + p → He3 (środek)
    for (int i = 0; i < 3; i++) {
        auto d = sim.physics->createBody();
        d->pos = {-4e10, i * 3e10, 0};
        d->vel = {2.5e7, 0, 0};
        d->mass = DEUTERON_MASS;
        d->radius = 2e9;
        d->label = "D";
        d->setColor(1.0f, 0.7f, 0.3f);
        d->setEmissive(0.4f);
        d->elasticity = 0.1;

        auto p = sim.physics->createBody();
        p->pos = {4e10, i * 3e10, 0};
        p->vel = {-2.5e7, 0, 0};
        p->mass = PROTON_MASS;
        p->radius = 1.5e9;
        p->label = "p";
        p->setColor(1.0f, 0.3f, 0.3f);
        p->setEmissive(0.5f);
        p->elasticity = 0.1;
    }

    std::cout << "=== STEP 3: He³ + He³ → He⁴ + 2p ===\n";
    std::cout << "Two Helium-3 nuclei fuse to form Helium-4\n\n";

    // Krok 3: He3 + He3 → He4 (dół)
    for (int i = 0; i < 2; i++) {
        auto he3_1 = sim.physics->createBody();
        he3_1->pos = {-4e10, -8e10 - i * 3e10, 0};
        he3_1->vel = {2e7, 0, 0};
        he3_1->mass = HE3_MASS;
        he3_1->radius = 2.5e9;
        he3_1->label = "He3";
        he3_1->setColor(0.3f, 1.0f, 0.5f);
        he3_1->setEmissive(0.4f);
        he3_1->elasticity = 0.1;

        auto he3_2 = sim.physics->createBody();
        he3_2->pos = {4e10, -8e10 - i * 3e10, 0};
        he3_2->vel = {-2e7, 0, 0};
        he3_2->mass = HE3_MASS;
        he3_2->radius = 2.5e9;
        he3_2->label = "He3";
        he3_2->setColor(0.3f, 1.0f, 0.5f);
        he3_2->setEmissive(0.4f);
        he3_2->elasticity = 0.1;
    }

    // Efekty wizualne - cząstki energii
    for (int i = 0; i < 20; i++) {
        double angle = 2 * M_PI * i / 20;
        double r = 6e10;
        double v = 1.5e7;

        auto particle = sim.physics->createBody();
        particle->pos = {r * cos(angle) * 0.2, r * sin(angle) * 0.2, 0};
        particle->vel = {v * cos(angle) * 0.3, v * sin(angle) * 0.3, 0};
        particle->mass = PROTON_MASS * 0.05;
        particle->radius = 5e8;
        particle->label = "";
        particle->setColor(1.0f, 1.0f, 0.3f);
        particle->colorByVelocity = true;
        particle->maxVelocityForColor = v * 2;
        particle->setEmissive(0.9f);
        particle->showTrail = true;
        particle->elasticity = 0.3;
    }

    std::cout << "=== STELLAR CONDITIONS ===\n";
    std::cout << "Temperature: ~15 million K (Sun's core)\n";
    std::cout << "Pressure: ~250 billion atmospheres\n";
    std::cout << "Density: ~150 g/cm³\n\n";

    std::cout << "=== ENERGY RELEASE ===\n";
    std::cout << "Total energy per cycle: 26.7 MeV\n";
    std::cout << "This powers the Sun for billions of years!\n\n";

    std::cout << "=== COLLISION PHYSICS ===\n";
    std::cout << "Energy thresholds:\n";
    std::cout << "  < 1e20 J  → Elastic bounce\n";
    std::cout << "  1e20-1e25 → FUSION (merge)\n";
    std::cout << "  1e25-1e30 → FRAGMENTATION\n";
    std::cout << "  > 1e30    → Annihilation\n\n";

    std::cout << "=== LEGEND ===\n";
    std::cout << "  p    - Proton (red)\n";
    std::cout << "  D    - Deuteron (orange)\n";
    std::cout << "  He3  - Helium-3 (green)\n";
    std::cout << "  MRG  - Fusion product (orange/yellow)\n\n";

    std::cout << "=== PROCESS SUMMARY ===\n";
    std::cout << "4 protons → 1 Helium-4 + 2 positrons + 2 neutrinos + energy\n";
    std::cout << "This is how stars convert hydrogen to helium!\n\n";

    std::cout << "Controls:\n";
    std::cout << "  SPACE - Pause/Resume\n";
    std::cout << "  W/S   - Zoom\n";
    std::cout << "  Mouse - Rotate view\n";
    std::cout << "  M     - Toggle slow motion\n";
    std::cout << "  +/-   - Adjust time speed\n";
    std::cout << "  Q     - Quit\n\n";

    std::cout << "Starting stellar fusion simulation in SLOW MOTION...\n";
    std::cout << "⭐ Watch the three stages of the PP chain!\n";
    std::cout << "   TOP: p + p → D\n";
    std::cout << "   MIDDLE: D + p → He³\n";
    std::cout << "   BOTTOM: He³ + He³ → He⁴\n\n";

    sim.run();

    return 0;
}
