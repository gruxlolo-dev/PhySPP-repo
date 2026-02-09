#include "PhySPP.h"

int main() {
  Simulation sim(1920, 1080, "PhySPP - Realistic Collision Physics");

  // Centralna masa
  auto center = sim.physics->createBody();
  center->pos = {0, 0, 0};
  center->vel = {0, 0, 0};
  center->mass = 1e30;
  center->radius = 1e9;
  center->flags = BodyFlags::STATIC | BodyFlags::NO_INTEGRATION;
  center->label = "CTR";
  center->setColor(1.0f, 1.0f, 0.0f);
  center->setEmissive(1.0f);

  // 1. NISKA ENERGIA - Odbicia elastyczne (blisko centrum)
  std::cout << "\n=== COLLISION TYPES ===\n\n";
  std::cout << "1. LOW ENERGY (inner orbit) - Elastic bouncing\n";
  for(int i = 0; i < 10; i++) {
    double angle = 2 * M_PI * i / 10;
    double r = 5e10;
    double v = std::sqrt(Physics::G * center->mass / r) * 1.1;
    
    auto body = sim.physics->createBody();
    body->pos = {r * cos(angle), 0, r * sin(angle)};
    body->vel = {-v * sin(angle), 0, v * cos(angle)};
    body->mass = 1e22;
    body->radius = 5e6;
    body->elasticity = 0.9;
    body->label = "ELA";
    body->setColor(0.3f, 0.8f, 0.3f);
    body->colorByVelocity = true;
    body->maxVelocityForColor = 5e4;
  }

  // 2. ŚREDNIA ENERGIA - Fuzja/łączenie
  std::cout << "2. MEDIUM ENERGY (mid orbit) - Merging/Fusion\n";
  for(int i = 0; i < 8; i++) {
    double angle = 2 * M_PI * i / 8;
    double r = 1.5e11;
    double v = std::sqrt(Physics::G * center->mass / r) * 0.95;
    
    auto body = sim.physics->createBody();
    body->pos = {r * cos(angle), 0, r * sin(angle)};
    body->vel = {-v * sin(angle), 0, v * cos(angle)};
    body->mass = 1e24;
    body->radius = 1e7;
    body->label = "MRG";
    body->setColor(0.3f, 0.3f, 0.8f);
    body->colorByVelocity = true;
    body->maxVelocityForColor = 3e4;
  }

  // 3. WYSOKA ENERGIA - Fragmentacja (nowe cząstki)
  std::cout << "3. HIGH ENERGY (outer orbit) - Fragmentation (new particles)\n";
  for(int i = 0; i < 6; i++) {
    double angle = 2 * M_PI * i / 6;
    double r = 3e11;
    double v = std::sqrt(Physics::G * center->mass / r) * 1.3;
    
    auto body = sim.physics->createBody();
    body->pos = {r * cos(angle), 0, r * sin(angle)};
    body->vel = {-v * sin(angle), 0, v * cos(angle)};
    body->mass = 1e26;
    body->radius = 3e7;
    body->label = "FRG";
    body->setColor(0.8f, 0.5f, 0.2f);
    body->colorByVelocity = true;
    body->maxVelocityForColor = 5e4;
  }

  // 4. BARDZO WYSOKA ENERGIA - Anihilacja (promienie gamma)
  std::cout << "4. EXTREME ENERGY (collision course) - Annihilation (gamma rays)\n\n";
  for(int i = 0; i < 4; i++) {
    double angle = M_PI * i / 2;
    double r = 4e11;
    double v = std::sqrt(Physics::G * center->mass / r) * 2.0;
    
    auto body = sim.physics->createBody();
    body->pos = {r * cos(angle), 0, r * sin(angle)};
    body->vel = {-v * sin(angle) * 1.5, 0, v * cos(angle) * 1.5};
    body->mass = 1e28;
    body->radius = 5e7;
    body->label = "ANI";
    body->setColor(1.0f, 0.2f, 0.2f);
    body->colorByVelocity = true;
    body->maxVelocityForColor = 1e5;
  }

  // Konfiguracja
  sim.physics->setIntegrator(IntegratorType::LEAPFROG);
  sim.enableCollisions();
  sim.enableGlow();
  sim.enableBloom();
  sim.setTimeStep(3600);

  std::cout << "LEGEND:\n";
  std::cout << "  ELA - Elastic (green) - Low energy bouncing\n";
  std::cout << "  MRG - Merge (blue) - Medium energy fusion\n";
  std::cout << "  FRG - Fragment (orange) - High energy particle creation\n";
  std::cout << "  ANI - Annihilation (red) - Extreme energy gamma rays\n";
  std::cout << "  γ - Gamma ray (yellow) - Pure energy\n\n";
  std::cout << "Watch different collision types based on energy!\n\n";

  sim.run();

  return 0;
}
