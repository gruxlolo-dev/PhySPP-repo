#include "PhySPP.h"

int main() {
  Simulation sim(1920, 1080, "PhySPP - Realistic Solar System");

  // Słońce
  sim.addSun();

  // Planety wewnętrzne
  auto mercury = sim.physics->createBody();
  mercury->pos = {0.387 * Physics::AU, 0, 0};
  mercury->vel = {0, 47870, 0};
  mercury->mass = 3.285e23;
  mercury->radius = 2.4397e6;
  mercury->label = "MER";
  mercury->setColor(0.7f, 0.7f, 0.7f);

  auto venus = sim.physics->createBody();
  venus->pos = {0.723 * Physics::AU, 0, 0};
  venus->vel = {0, 35020, 0};
  venus->mass = 4.867e24;
  venus->radius = 6.0518e6;
  venus->label = "VEN";
  venus->setColor(0.9f, 0.8f, 0.5f);

  sim.addEarth();
  sim.addMars();

  // Pas asteroid
  for(int i = 0; i < 50; i++) {
    double angle = 2 * M_PI * i / 50;
    double r = (2.2 + (rand() % 100) / 200.0) * Physics::AU;
    double v = std::sqrt(Physics::G * Physics::SOLAR_MASS / r);
    
    auto ast = sim.physics->createBody();
    ast->pos = {r * cos(angle), (rand() % 1000000 - 500000) * 1e3, r * sin(angle)};
    ast->vel = {-v * sin(angle), 0, v * cos(angle)};
    ast->mass = 1e18 + rand() % (int)1e19;
    ast->radius = 5e5;
    ast->label = "";
    ast->colorByVelocity = true;
    ast->maxVelocityForColor = 5e4;
    ast->setColor(0.6f, 0.5f, 0.4f);
    ast->showTrail = false;
  }

  sim.addJupiter();

  // Saturn
  auto saturn = sim.physics->createBody();
  saturn->pos = {9.54 * Physics::AU, 0, 0};
  saturn->vel = {0, 9690, 0};
  saturn->mass = 5.683e26;
  saturn->radius = 5.8232e7;
  saturn->label = "SAT";
  saturn->setColor(0.9f, 0.8f, 0.6f);

  // Komety
  for(int i = 0; i < 5; i++) {
    double angle = 2 * M_PI * i / 5;
    double r = (10 + i * 5) * Physics::AU;
    double v = std::sqrt(Physics::G * Physics::SOLAR_MASS / r) * 0.7;
    
    auto comet = sim.physics->createBody();
    comet->pos = {r * cos(angle), Physics::AU * 2, r * sin(angle)};
    comet->vel = {-v * sin(angle), -5000, v * cos(angle)};
    comet->mass = 1e13;
    comet->radius = 5e3;
    comet->label = "COM";
    comet->setColor(0.7f, 0.8f, 0.9f);
    comet->showTrail = true;
    comet->colorByVelocity = true;
    comet->maxVelocityForColor = 5e4;
  }

  // Optymalizacja silnika
  sim.physics->setIntegrator(IntegratorType::LEAPFROG);
  sim.physics->enableAdaptiveTimestep(false);
  sim.physics->enableSleeping(false);
  sim.enableCollisions();
  sim.enableGlow();
  sim.setTimeStep(3600 * 6);

  std::cout << "\n=== REALISTIC SOLAR SYSTEM ===\n";
  std::cout << "Bodies: " << sim.physics->getBodies().size() << "\n";
  std::cout << "Integrator: Leapfrog (symplectic)\n";
  std::cout << "Labels: Enabled\n";
  std::cout << "Effects: Glow, Velocity colors\n\n";
  std::cout << "Controls:\n";
  std::cout << "  SPACE - Pause\n";
  std::cout << "  W/S - Zoom\n";
  std::cout << "  Mouse - Rotate\n";
  std::cout << "  0-9 - Follow body\n";
  std::cout << "  F - Auto follow\n";
  std::cout << "  Q - Quit\n\n";

  sim.run();

  return 0;
}
