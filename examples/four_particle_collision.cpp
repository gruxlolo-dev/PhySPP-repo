#include "PhySPP.h"
#include <iostream>

int main() {
    std::cout << "4 Particle Collision\n";

    Simulation sim(1280, 720, "4 Particle Collision");

    double speed = 5000;
    double dist = 1e8;

    sim.addBodyWithVelocityColor(Vec3(-dist, -dist, 0), Vec3(speed, speed, 0), 1e22, 1e6);
    sim.addBodyWithVelocityColor(Vec3(dist, -dist, 0), Vec3(-speed, speed, 0), 1e22, 1e6);
    sim.addBodyWithVelocityColor(Vec3(-dist, dist, 0), Vec3(speed, -speed, 0), 1e22, 1e6);
    sim.addBodyWithVelocityColor(Vec3(dist, dist, 0), Vec3(-speed, -speed, 0), 1e22, 1e6);

    sim.useRK4();
    sim.enableCollisions();
    sim.setTimeStep(100);

    sim.run();
    return 0;
}
