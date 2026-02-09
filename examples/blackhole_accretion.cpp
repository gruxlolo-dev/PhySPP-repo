#include "PhySPP.h"
#include <iostream>
#include <string>

void mode1_ParticleAccretion() {
  std::cout << "\n[MODE 1] Particle Accretion - Dust cloud absorbed\n";
  Simulation sim(1920, 1080, "Black Hole - Particle Accretion");
  sim.addBlackHole(Vec3(0, 0, 0), 50 * Physics::SOLAR_MASS);

  for (int i = 0; i < 150; i++) {
    double angle = i * 2.0 * M_PI / 150;
    double r = Physics::AU * (0.2 + i * 0.006);
    Vec3 pos(r * cos(angle), r * sin(angle), (rand() % 1000 - 500) * 1e9);
    double v = sqrt(Physics::G * 50 * Physics::SOLAR_MASS / r) * 0.88;
    Vec3 vel(-v * sin(angle), v * cos(angle), 0);
    sim.addBodyWithVelocityColor(pos, vel, 1e19, 5e5);
  }

  sim.useRK4();
  sim.enableRelativity();
  sim.enableCollisions();
  sim.enableGlow();
  sim.setTimeStep(3600 * 2);
  sim.run();
}

void mode2_EarthAccretion() {
  std::cout << "\n[MODE 2] Earth Accretion - Planet being consumed!\n";
  Simulation sim(1920, 1080, "Black Hole - Earth Destruction");
  sim.addBlackHole(Vec3(0, 0, 0), 100 * Physics::SOLAR_MASS);

  Vec3 earthPos(Physics::AU * 0.5, 0, 0);
  Vec3 earthVel(
      0,
      sqrt(Physics::G * 100 * Physics::SOLAR_MASS / (Physics::AU * 0.5)) * 0.85,
      0);
  sim.addBody(earthPos, earthVel, Physics::EARTH_MASS, 6.371e6);

  Vec3 moonPos = earthPos + Vec3(3.844e8, 0, 0);
  Vec3 moonVel = earthVel + Vec3(0, 1022, 0);
  sim.addBody(moonPos, moonVel, 7.342e22, 1.737e6);

  for (int i = 0; i < 50; i++) {
    double angle = i * 2.0 * M_PI / 50;
    double r = Physics::AU * (0.3 + i * 0.005);
    Vec3 pos(r * cos(angle), r * sin(angle), (rand() % 500 - 250) * 1e9);
    double v = sqrt(Physics::G * 100 * Physics::SOLAR_MASS / r) * 0.9;
    Vec3 vel(-v * sin(angle), v * cos(angle), 0);
    sim.addBodyWithVelocityColor(pos, vel, 1e22, 1e6);
  }

  sim.useRK4();
  sim.enableRelativity();
  sim.enableCollisions();
  sim.enableGlow();
  sim.setTimeStep(3600 * 4);
  sim.run();
}

void mode3_SolarSystemAccretion() {
  std::cout << "\n[MODE 3] Solar System Destruction - Everything consumed!\n";
  Simulation sim(1920, 1080, "Black Hole - Solar System Apocalypse");
  sim.addBlackHole(Vec3(0, 0, 0), 200 * Physics::SOLAR_MASS);

  Vec3 sunPos(Physics::AU * 0.8, 0, 0);
  Vec3 sunVel(
      0,
      sqrt(Physics::G * 200 * Physics::SOLAR_MASS / (Physics::AU * 0.8)) * 0.8,
      0);
  sim.addBody(sunPos, sunVel, Physics::SOLAR_MASS, 6.96e8);

  double distances[] = {0.4, 0.7, 1.0, 1.5, 5.2};
  double masses[] = {3.3e23, 4.87e24, 5.97e24, 6.39e23, 1.898e27};
  for (int i = 0; i < 5; i++) {
    double angle = i * 1.2;
    double r = Physics::AU * distances[i];
    Vec3 pos(r * cos(angle), r * sin(angle), 0);
    double v = sqrt(Physics::G * 200 * Physics::SOLAR_MASS / r) * 0.82;
    Vec3 vel(-v * sin(angle), v * cos(angle), 0);
    sim.addBody(pos, vel, masses[i], 1e7);
  }

  sim.useRK4();
  sim.enableRelativity();
  sim.enableCollisions();
  sim.enableGlow();
  sim.setTimeStep(3600 * 6);
  sim.run();
}

int main(int argc, char *argv[]) {
  std::cout
      << "\n╔══════════════════════════════════════════════════════════╗\n";
  std::cout << "║        Black Hole Accretion - Multi-Mode Simulator      ║\n";
  std::cout
      << "╚══════════════════════════════════════════════════════════╝\n\n";

  std::cout << "Wybierz tryb:\n";
  std::cout << "  1 - Particle Accretion (cząstki wciągane)\n";
  std::cout << "  2 - Earth Accretion (Ziemia wciągana)\n";
  std::cout << "  3 - Solar System Destruction (cały układ)\n";

  int mode = 1;
  if (argc > 1) {
    mode = std::stoi(argv[1]);
  } else {
    std::cout << "Tryb (1-4): ";
    std::cin >> mode;
  }

  switch (mode) {
  case 1:
    mode1_ParticleAccretion();
    break;
  case 2:
    mode2_EarthAccretion();
    break;
  case 3:
    mode3_SolarSystemAccretion();
    break;
  default:
    std::cout << "Nieprawidłowy tryb! Uruchamiam tryb 1...\n";
    mode1_ParticleAccretion();
  }

  return 0;
}
