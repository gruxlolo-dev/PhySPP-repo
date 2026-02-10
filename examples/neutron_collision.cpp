#include <iostream>
#include "api/simulation.h"

int main() {
    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║     NEUTRON-NEUTRON COLLISION                        ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n\n";

    Simulation sim(1920, 1080, "Neutron Collision");

    sim.physics->setIntegrator(IntegratorType::LEAPFROG);
    sim.enableCollisions();
    sim.enableGlow();
    sim.enableBloom();
    sim.setTimeStep(100);
    sim.physics->setTimeScale(0.01);

    sim.renderer->setCamera(Vec3(0, 2e11, 3e11), Vec3(0, 0, 0));

    const double SCALE_MASS = 1e27;
    const double NEUTRON_MASS = 1.675e-27 * SCALE_MASS;
    const double COLLISION_SPEED = 1e8;

    auto n1 = sim.physics->createBody();
    n1->pos = Vec3(-5e10, 0, 0);
    n1->vel = Vec3(COLLISION_SPEED, 0, 0);
    n1->mass = NEUTRON_MASS;
    n1->radius = 5e9;
    n1->label = "n₁";
    n1->setColor(0.3f, 0.7f, 1.0f);
    n1->showTrail = true;
    n1->setEmissive(0.8f);
    n1->elasticity = 0.3;

    auto n2 = sim.physics->createBody();
    n2->pos = Vec3(5e10, 0, 0);
    n2->vel = Vec3(-COLLISION_SPEED, 0, 0);
    n2->mass = NEUTRON_MASS;
    n2->radius = 5e9;
    n2->label = "n₂";
    n2->setColor(0.7f, 1.0f, 0.3f);
    n2->showTrail = true;
    n2->setEmissive(0.8f);
    n2->elasticity = 0.3;

    std::cout << "Two neutrons colliding head-on\n";
    std::cout << "Speed: 1×10⁸ m/s (~33% speed of light)\n";
    std::cout << "Distance: 100 Gm\n\n";
    std::cout << "Physics timestep: 0.01s (accurate simulation)\n";
    std::cout << "Playback speed: 0.5x (slow motion for viewing)\n\n";
    std::cout << "Controls: SPACE=Pause | W/S=Zoom | M=Slow-mo | Q=Quit\n\n";
    std::cout << "Starting simulation...\n\n";

    sim.run();
    return 0;
}
