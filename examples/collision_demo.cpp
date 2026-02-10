#include "PhySPP.h"

int main() {
    Simulation sim(1280, 720, "Collision Demo - Velocity Colors & Fragmentation");

    // Centralna masa
    sim.addBody(Vec3(0, 0, 0), Vec3(0, 0, 0), 1e30, 5e8);

    // Dodaj ciała z kolorowaniem według prędkości
    for (int i = 0; i < 30; i++) {
        double angle = 2 * M_PI * i / 30;
        double r = 2e11;
        double vx = -4e4 * sin(angle);
        double vz = 4e4 * cos(angle);

        sim.addBodyWithVelocityColor(
            Vec3(r * cos(angle), (rand() % 2000000 - 1000000) * 1e4, r * sin(angle)),
            Vec3(vx, 0, vz),
            1e24, 5e7
        );
    }

    // Dodaj kilka szybkich obiektów na kolizyjnych orbitach
    for (int i = 0; i < 10; i++) {
        double angle = 2 * M_PI * i / 10;
        double r = 1e11;
        double v = 8e4;

        sim.addBodyWithVelocityColor(
            Vec3(r * cos(angle), 0, r * sin(angle)),
            Vec3(-v * sin(angle), 0, v * cos(angle)),
            5e23, 3e7
        );
    }

    sim.useRK4();
    sim.enableCollisions();
    sim.enableGlow();
    sim.enableBloom();
    sim.setTimeStep(3600 * 6);
    sim.run();

    return 0;
}
