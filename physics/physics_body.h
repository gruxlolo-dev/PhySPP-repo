#pragma once
#include "../core/vec3.h"
#include "../core/constants.h"
#include <string>
#include <vector>
#include <memory>

enum class PhysicsBodyType {
  // Codzienne
  RIGID_BODY, SOFT_BODY, FLUID_VOLUME, GAS_VOLUME, PLASMA_VOLUME,
  
  // Kosmiczne małe
  DUST, MICROMETEOROID, METEOROID, ASTEROID_RUBBLE, ASTEROID_SOLID,
  COMET_ICY, COMET_ACTIVE,
  
  // Planetarne
  DWARF_PLANET, ROCKY_PLANET, OCEAN_PLANET, ICE_PLANET,
  GAS_GIANT, ICE_GIANT, RING_SYSTEM, MOON, ARTIFICIAL_SATELLITE,
  
  // Gwiazdowe
  PROTOSTAR, MAIN_SEQUENCE_STAR, RED_GIANT, WHITE_DWARF,
  NEUTRON_STAR, MAGNETAR,
  
  // Relatywistyczne
  BLACK_HOLE_STELLAR, BLACK_HOLE_SUPERMASSIVE,
  ACCRETION_DISK, RELATIVISTIC_JET,
  
  // Energetyczne
  RADIATION_PACKET, SHOCKWAVE, EXPLOSION_EVENT
};

struct AstroProperties {
  double gravityWellStrength = 0;
  double bindingEnergy = 0;
  double fusionRate = 0;
  double fissionRate = 0;
  double tidalStressLimit = 0;
  double rocheLimit = 0;
  double luminosity = 0;
  double radiationPressure = 0;
};

class PhysicsBody {
public:
  // Podstawowe
  Vec3 position;
  Vec3 velocity;
  Vec3 angularVelocity;
  double mass;
  double invMass;
  double radius;
  double density;
  
  // Energia
  double kineticEnergy = 0;
  double internalEnergy = 0;
  double temperature = 0;
  
  // Materiał
  double restitution = 0.5;
  double friction = 0.1;
  double elasticity = 0.5;
  
  // Typ i stan
  PhysicsBodyType type;
  int stateFlags = 0;
  bool destroyed = false;
  
  // Astrofizyka
  AstroProperties astro;
  
  // Identyfikacja
  std::string name;
  std::string label;
  
  // Rendering (opcjonalne)
  float color[3] = {1, 1, 1};
  float emissive = 0;
  bool showTrail = false;
  std::vector<Vec3> trail;
  
  PhysicsBody(Vec3 pos, Vec3 vel, double m, double r, PhysicsBodyType t = PhysicsBodyType::RIGID_BODY)
    : position(pos), velocity(vel), mass(m), radius(r), type(t) {
    invMass = (m > 0) ? 1.0 / m : 0;
    angularVelocity = Vec3(0, 0, 0);
    density = mass / (4.0/3.0 * M_PI * radius * radius * radius);
    updateEnergy();
    computeBindingEnergy();
  }
  
  void updateEnergy() {
    kineticEnergy = 0.5 * mass * velocity.lengthSq();
  }
  
  void computeBindingEnergy() {
    // Energia wiązania grawitacyjnego (self-gravity)
    astro.bindingEnergy = 0.6 * Physics::G * mass * mass / radius;
  }
  
  double momentum() const {
    return mass * velocity.length();
  }
  
  Vec3 momentumVector() const {
    return velocity * mass;
  }
  
  double escapeVelocity() const {
    return std::sqrt(2.0 * Physics::G * mass / radius);
  }
  
  void setColor(float r, float g, float b) {
    color[0] = r; color[1] = g; color[2] = b;
  }
  
  void destroy() { destroyed = true; }
};

using PhysicsBodyPtr = std::shared_ptr<PhysicsBody>;
