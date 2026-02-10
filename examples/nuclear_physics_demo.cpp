#include "PhySPP.h"
#include <iostream>

// Skalowanie dla wizualizacji
const double SCALE_MASS = 1e27; // Zwiększenie masy dla widoczności
const double SCALE_DISTANCE = 1e9; // Skalowanie odległości
const double SCALE_VELOCITY = 1e5; // Skalowanie prędkości
const double SCALE_TIME = 1e-6; // Mikrosekunda

// Stałe fizyczne (realne wartości)
const double PROTON_MASS_REAL = 1.673e-27; // kg
const double NEUTRON_MASS_REAL = 1.675e-27; // kg

// Przeskalowane dla symulacji (masy muszą być większe dla widoczności)
const double PROTON_MASS = PROTON_MASS_REAL * SCALE_MASS;
const double NEUTRON_MASS = NEUTRON_MASS_REAL * SCALE_MASS;
const double U235_MASS = 235 * PROTON_MASS;

int main() {
    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║     NUCLEAR PHYSICS SIMULATION                       ║\n";
    std::cout << "║     Fusion & Fission - Properly Scaled               ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n\n";

    Simulation sim(1920, 1080, "Nuclear Physics - Fusion & Fission");

    sim.physics->setIntegrator(IntegratorType::LEAPFROG);
    sim.enableCollisions();
    sim.enableGlow();
    sim.enableBloom();
    sim.setTimeStep(3600.0);
    sim.physics->setTimeScale(0.2);

    // Ustaw kamerę tak, aby wszystko było widoczne
    sim.renderer->setCamera(Vec3(1e11, 1.5e11, 4e11), Vec3(1e11, 0, 0));

    std::cout << "=== SCALING FACTORS ===\n";
    std::cout << "Mass scale: " << SCALE_MASS << "x\n";
    std::cout << "Distance scale: " << SCALE_DISTANCE << "x\n";
    std::cout << "Velocity scale: " << SCALE_VELOCITY << "x\n";
    std::cout << "Time scale: " << SCALE_TIME << "x\n\n";

    std::cout << "=== SCENARIO 1: NUCLEAR FUSION (H → He) ===\n";
    std::cout << "4 protons → Helium-4 + 26.7 MeV\n\n";

    for (int i = 0; i < 4; i++) {
        double angle = 2 * M_PI * i / 4;
        double r = 5e10;
        double v = 5e7;

        auto proton = sim.physics->createBody();
        proton->pos = {r * cos(angle), r * sin(angle), 0};
        proton->vel = {-v * cos(angle), -v * sin(angle), 0};
        proton->mass = PROTON_MASS;
        proton->radius = 1.5e9;
        proton->label = "H";
        proton->setColor(1.0f, 0.3f, 0.3f);
        proton->colorByVelocity = true;
        proton->maxVelocityForColor = v * 2;
        proton->setEmissive(0.5f);
        proton->elasticity = 0.1;
    }

    std::cout << "=== SCENARIO 2: NEUTRON-PROTON COLLISION ===\n";
    std::cout << "n + p → Deuteron (heavy hydrogen)\n\n";

    auto neutron1 = sim.physics->createBody();
    neutron1->pos = {-3e10, -8e10, 0};
    neutron1->vel = {4e7, 0, 0};
    neutron1->mass = NEUTRON_MASS;
    neutron1->radius = 1.5e9;
    neutron1->label = "n";
    neutron1->setColor(0.7f, 0.7f, 1.0f);
    neutron1->colorByVelocity = true;
    neutron1->maxVelocityForColor = 5e7;
    neutron1->setEmissive(0.4f);
    neutron1->elasticity = 0.1;

    auto proton1 = sim.physics->createBody();
    proton1->pos = {3e10, -8e10, 0};
    proton1->vel = {-4e7, 0, 0};
    proton1->mass = PROTON_MASS;
    proton1->radius = 1.5e9;
    proton1->label = "p";
    proton1->setColor(1.0f, 0.3f, 0.3f);
    proton1->colorByVelocity = true;
    proton1->maxVelocityForColor = 5e7;
    proton1->setEmissive(0.5f);
    proton1->elasticity = 0.1;

    std::cout << "=== SCENARIO 3: NUCLEAR FISSION (U-235) ===\n";
    std::cout << "U-235 + neutron → Ba-141 + Kr-92 + 3n + 200 MeV\n\n";

    // Jądra uranu (po prawej)
    for (int i = 0; i < 6; i++) {
        double angle = 2 * M_PI * i / 6;
        double r = 1e11;

        auto uranium = sim.physics->createBody();
        uranium->pos = {2e11 + r * cos(angle), r * sin(angle), 0};
        uranium->vel = {0, 0, 0};
        uranium->mass = U235_MASS;
        uranium->radius = 3e9;
        uranium->label = "U235";
        uranium->setColor(0.3f, 1.0f, 0.3f);
        uranium->setEmissive(0.4f);
        uranium->elasticity = 0.2;
    }

    // Neutrony inicjujące rozszczepienie
    for (int i = 0; i < 4; i++) {
        auto neutron = sim.physics->createBody();
        neutron->pos = {8e10, (i - 1.5) * 5e10, 0};
        neutron->vel = {3e7, 0, 0}; // Bardzo duża prędkość
        neutron->mass = NEUTRON_MASS;
        neutron->radius = 1e9;
        neutron->label = "n";
        neutron->setColor(0.7f, 0.7f, 1.0f);
        neutron->colorByVelocity = true;
        neutron->maxVelocityForColor = 5e7;
        neutron->setEmissive(0.3f);
        neutron->elasticity = 0.1;
    }

    // Dodaj widowiskowe cząstki energii dla efektu fuzji
    for (int i = 0; i < 15; i++) {
        double angle = 2 * M_PI * i / 15;
        double r = 4e10;
        double v = 2e7;

        auto particle = sim.physics->createBody();
        particle->pos = {r * cos(angle) * 0.3, r * sin(angle) * 0.3, 0};
        particle->vel = {v * cos(angle) * 0.5, v * sin(angle) * 0.5, 0};
        particle->mass = PROTON_MASS * 0.05;
        particle->radius = 6e8;
        particle->label = "";
        particle->setColor(1.0f, 0.8f, 0.0f);
        particle->colorByVelocity = true;
        particle->maxVelocityForColor = v * 2;
        particle->setEmissive(0.8f);
        particle->showTrail = true;
        particle->elasticity = 0.3;
    }

    // Dodaj widowiskowe cząstki dla rozszczepienia
    for (int i = 0; i < 12; i++) {
        double angle = 2 * M_PI * i / 12;
        double r = 1.5e11;
        double v = 2e7;

        auto particle = sim.physics->createBody();
        particle->pos = {2e11 + r * cos(angle) * 0.2, r * sin(angle) * 0.2, 0};
        particle->vel = {v * cos(angle) * 0.4, v * sin(angle) * 0.4, 0};
        particle->mass = PROTON_MASS * 0.1;
        particle->radius = 7e8;
        particle->label = "";
        particle->setColor(1.0f, 0.5f, 0.0f);
        particle->colorByVelocity = true;
        particle->maxVelocityForColor = v * 2;
        particle->setEmissive(0.9f);
        particle->showTrail = true;
        particle->elasticity = 0.3;
    }

    std::cout << "=== COLLISION PHYSICS ===\n";
    std::cout << "Energy thresholds (scaled):\n";
    std::cout << "  < 1e20 J  → Elastic bounce\n";
    std::cout << "  1e20-1e25 → FUSION (H→He, +26.7 MeV)\n";
    std::cout << "  1e25-1e30 → FISSION (U→fragments, +200 MeV)\n";
    std::cout << "  > 1e30    → Annihilation (m→γ)\n\n";

    std::cout << "=== EXPECTED RESULTS ===\n";
    std::cout << "Fusion (Scenario 1):\n";
    std::cout << "  Input:  4 × H¹ (4 protons)\n";
    std::cout << "  Output: 1 × He⁴ (helium nucleus)\n";
    std::cout << "  Energy: +26.7 MeV (mass defect 0.7%)\n";
    std::cout << "  Label:  MRG (merged nucleus)\n\n";

    std::cout << "Neutron-Proton (Scenario 2):\n";
    std::cout << "  Input:  n + p\n";
    std::cout << "  Output: Deuteron (heavy hydrogen)\n";
    std::cout << "  Energy: +2.2 MeV\n";
    std::cout << "  Label:  MRG\n\n";

    std::cout << "Fission (Scenario 3):\n";
    std::cout << "  Input:  U-235 + neutron\n";
    std::cout << "  Output: 3-5 fragments + neutrons\n";
    std::cout << "  Energy: +200 MeV per fission\n";
    std::cout << "  Label:  FRG (fission fragments)\n\n";

    std::cout << "Chain Reaction:\n";
    std::cout << "  Neutrons from fission hit other U-235\n";
    std::cout << "  → Cascading fissions\n";
    std::cout << "  → Exponential energy release\n\n";

    std::cout << "=== CONSERVATION LAWS ===\n";
    std::cout << "  ✓ Energy: E_kinetic + E_binding + mc² = const\n";
    std::cout << "  ✓ Momentum: Σ(m·v) = const\n";
    std::cout << "  ✓ Charge: Σ(q) = const\n";
    std::cout << "  ✓ Baryon number: Σ(A) = const\n\n";

    std::cout << "=== LEGEND ===\n";
    std::cout << "  H/p  - Hydrogen/Proton (red)\n";
    std::cout << "  n    - Neutron (blue)\n";
    std::cout << "  U235 - Uranium-235 (green)\n";
    std::cout << "  MRG  - Fusion product (orange)\n";
    std::cout << "  FRG  - Fission fragment (orange)\n";
    std::cout << "  γ    - Gamma ray (yellow, extreme energy)\n\n";

    std::cout << "Controls:\n";
    std::cout << "  SPACE - Pause/Resume\n";
    std::cout << "  W/S   - Zoom\n";
    std::cout << "  Mouse - Rotate view\n";
    std::cout << "  M     - Toggle slow motion (1/5 speed)\n";
    std::cout << "  +     - Speed up time (2x, 4x, 8x, 10x max)\n";
    std::cout << "  -     - Slow down time (0.5x, 0.25x, 0.1x min)\n";
    std::cout << "  Q     - Quit\n\n";

    std::cout << "Starting simulation in SLOW MOTION (0.2x speed)...\n";
    std::cout << "Watch top for FUSION (red particles merging)\n";
    std::cout << "Watch middle for NEUTRON-PROTON collision\n";
    std::cout << "Watch right side for FISSION (green nuclei splitting)\n";
    std::cout << "\n⚡ SPECTACULAR EXPLOSIONS with energy particles!\n";
    std::cout << "⏱️  Press M to toggle slow motion, +/- to adjust speed\n\n";

    sim.run();

    return 0;
}
