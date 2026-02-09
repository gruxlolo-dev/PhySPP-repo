#pragma once
#include "../core/constants.h"
#include "../physics/body.h"
#include <memory>

namespace Presets {
inline BodyPtr Sun() {
  auto sun =
      std::make_shared<Body>(Vec3(0, 0, 0), Vec3(0, 0, 0), Physics::SOLAR_MASS,
                             Physics::SOLAR_RADIUS, BodyType::STAR);
  sun->name = "Sun";
  sun->label = "SUN";
  sun->setColor(1.0f, 0.9f, 0.2f);
  sun->setEmissive(1.0f);
  sun->luminosity = 3.828e26;
  sun->temperature = 5778;
  return sun;
}

inline BodyPtr Earth(Vec3 offset = Vec3(Physics::AU, 0, 0)) {
  double v = std::sqrt(Physics::G * Physics::SOLAR_MASS / Physics::AU);
  auto earth =
      std::make_shared<Body>(offset, Vec3(0, v, 0), Physics::EARTH_MASS,
                             Physics::EARTH_RADIUS, BodyType::PLANET);
  earth->name = "Earth";
  earth->label = "EAR";
  earth->setColor(0.2f, 0.5f, 1.0f);
  earth->temperature = 288;
  return earth;
}

inline BodyPtr Mars(Vec3 offset = Vec3(1.524 * Physics::AU, 0, 0)) {
  double v =
      std::sqrt(Physics::G * Physics::SOLAR_MASS / (1.524 * Physics::AU));
  auto mars = std::make_shared<Body>(offset, Vec3(0, v, 0), 6.39e23, 3.3895e6,
                                     BodyType::PLANET);
  mars->name = "Mars";
  mars->label = "MAR";
  mars->setColor(0.9f, 0.3f, 0.1f);
  mars->temperature = 210;
  return mars;
}

inline BodyPtr Jupiter(Vec3 offset = Vec3(5.2 * Physics::AU, 0, 0)) {
  double v = std::sqrt(Physics::G * Physics::SOLAR_MASS / (5.2 * Physics::AU));
  auto jupiter =
      std::make_shared<Body>(offset, Vec3(0, v, 0), Physics::JUPITER_MASS,
                             Physics::JUPITER_RADIUS, BodyType::PLANET);
  jupiter->name = "Jupiter";
  jupiter->label = "JUP";
  jupiter->setColor(0.8f, 0.7f, 0.5f);
  jupiter->temperature = 165;
  return jupiter;
}

inline BodyPtr BlackHole(Vec3 pos, double mass = 10 * Physics::SOLAR_MASS) {
  double rs = 2.0 * Physics::G * mass / (Physics::C * Physics::C);
  auto bh = std::make_shared<Body>(pos, Vec3(0, 0, 0), mass, rs * 100, BodyType::BLACK_HOLE);
  bh->name = "Black Hole";
  bh->label = "BH";
  bh->setColor(0.0f, 0.0f, 0.0f);
  bh->setEmissive(0.0f);
  bh->renderScale = 1.0;
  return bh;
}

inline BodyPtr NeutronStar(Vec3 pos, double mass = 1.4 * Physics::SOLAR_MASS) {
  auto ns = std::make_shared<Body>(pos, Vec3(0, 0, 0), mass, 10000,
                                   BodyType::NEUTRON_STAR);
  ns->name = "Neutron Star";
  ns->label = "NS";
  ns->setColor(0.8f, 0.9f, 1.0f);
  ns->setEmissive(0.8f);
  ns->temperature = 1e6;
  return ns;
}

inline BodyPtr Asteroid(Vec3 pos, Vec3 vel, double mass = 1e15,
                        double radius = 1e3) {
  auto ast = std::make_shared<Body>(pos, vel, mass, radius, BodyType::ASTEROID);
  ast->name = "Asteroid";
  ast->label = "AST";
  ast->setColor(0.5f, 0.5f, 0.5f);
  return ast;
}

inline BodyPtr Comet(Vec3 pos, Vec3 vel) {
  auto comet = std::make_shared<Body>(pos, vel, 1e13, 5e3, BodyType::COMET);
  comet->name = "Comet";
  comet->label = "COM";
  comet->setColor(0.7f, 0.8f, 0.9f);
  comet->showTrail = true;
  return comet;
}
} // namespace Presets
