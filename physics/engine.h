#pragma once
#include "body.h"
#include "forces.h"
#include "integrators.h"
#include "mhd.h"
#include "radiative_transfer.h"
#include "particle_interactions.h"
#include "plasma_thermodynamics.h"
#include "conservation_laws.h"
#include "thermodynamics.h"
#include "electromagnetic_forces.h"
#include "octree.h"
#include <memory>
#include <vector>
#include <iostream>

enum class IntegratorType { EULER, VERLET, RK4 };

class PhysicsEngine {
  std::vector<BodyPtr> bodies;
  IntegratorType integrator = IntegratorType::VERLET;
  bool useRelativistic = false;
  bool detectCollisions = true;
  bool useTidalForces = false;
  bool useMHD = false;
  bool useRadiativeTransfer = false;
  bool useParticleInteractions = false;
  bool useOctree = false;
  double timeScale = 1.0;
  double initialEnergy = 0;
  Vec3 initialMomentum;
  std::unique_ptr<Octree> octree;

public:
  void add(BodyPtr body) { 
    bodies.push_back(body); 
    if(bodies.size() == 1) {
      initialEnergy = ConservationLaws::totalEnergy(bodies);
      initialMomentum = ConservationLaws::totalMomentum(bodies);
    }
  }

  void setIntegrator(IntegratorType type) { integrator = type; }
  void enableRelativistic(bool enable) { useRelativistic = enable; }
  void enableCollisions(bool enable) { detectCollisions = enable; }
  void enableTidalForces(bool enable) { useTidalForces = enable; }
  void enableMHD(bool enable) { useMHD = enable; }
  void enableRadiativeTransfer(bool enable) { useRadiativeTransfer = enable; }
  void enableParticleInteractions(bool enable) { useParticleInteractions = enable; }
  void enableOctree(bool enable) { useOctree = enable; }
  void setTimeScale(double scale) { timeScale = scale; }

  const std::vector<BodyPtr> &getBodies() const { return bodies; }

  void step(double dt) {
    dt *= timeScale;

    for (auto &body : bodies) {
      if (body->destroyed)
        continue;

      Vec3 totalForce(0, 0, 0);

      for (auto &other : bodies) {
        if (body == other || other->destroyed)
          continue;

        Vec3 r = other->pos - body->pos;
        double dist = r.length();
        
        if(other->mass > 5.0 * Physics::SOLAR_MASS && other->radius < 1e8) {
          const double c = Physics::C;
          double rs = 2.0 * Physics::G * other->mass / (c * c);
          
          if(dist < rs) {
            body->destroy();
            continue;
          }
          
          if(dist < rs * 3.0) {
            double tidalStrength = 2.0 * Physics::G * other->mass * body->radius / (dist * dist * dist);
            if(tidalStrength > body->mass * 1e10) {
              body->destroy();
              continue;
            }
          }
        }

        Vec3 force = useRelativistic
                         ? Forces::relativisticGravity(*body, *other)
                         : Forces::gravity(*body, *other);

        totalForce += force;

        if (useTidalForces) {
          totalForce += Forces::tidalForce(*body, *other);
        }
        
        if (useMHD) {
          Vec3 B(0, 0, 1e-4);
          totalForce += MHD::lorentzForce(body->vel, B, 1e-10);
        }
        
        if (useParticleInteractions) {
          totalForce += ParticleInteractions::viscousForce(body->vel, 0.01, 1e-10);
        }
      }
      
      if (useRadiativeTransfer) {
        double cooling = RadiativeTransfer::radiativeCooling(1e-10, 1e7);
        totalForce += body->vel * (-cooling * 1e-20);
      }

      auto forceFunc = [totalForce](const Body &) { return totalForce; };

      switch (integrator) {
      case IntegratorType::EULER:
        Integrators::euler(*body, dt, forceFunc);
        break;
      case IntegratorType::VERLET:
        Integrators::verlet(*body, dt, forceFunc);
        break;
      case IntegratorType::RK4:
        Integrators::rk4(*body, dt, forceFunc);
        break;
      }

      body->updateTrail();
    }

    if (detectCollisions) {
      checkCollisions();
    }

    bodies.erase(std::remove_if(bodies.begin(), bodies.end(),
                                [](const BodyPtr &b) { return b->destroyed; }),
                 bodies.end());
  }

  void checkCollisions() {
    for (size_t i = 0; i < bodies.size(); ++i) {
      for (size_t j = i + 1; j < bodies.size(); ++j) {
        auto &a = bodies[i];
        auto &b = bodies[j];

        double dist = (a->pos - b->pos).length();
        if (dist < a->radius + b->radius) {
          handleCollision(a, b);
        }
      }
    }
  }

  void handleCollision(BodyPtr &a, BodyPtr &b) {

    if (a->mass > b->mass) {
      a->vel = (a->vel * a->mass + b->vel * b->mass) / (a->mass + b->mass);
      a->mass += b->mass;
      b->destroy();
    } else {
      b->vel = (a->vel * a->mass + b->vel * b->mass) / (a->mass + b->mass);
      b->mass += a->mass;
      a->destroy();
    }
  }

  double totalEnergy() const {
    double E = 0;
    for (const auto &b : bodies) {
      if (!b->destroyed)
        E += b->kineticEnergy();
    }
    return E;
  }

  Vec3 centerOfMass() const {
    Vec3 com(0, 0, 0);
    double totalMass = 0;
    for (const auto &b : bodies) {
      if (!b->destroyed) {
        com += b->pos * b->mass;
        totalMass += b->mass;
      }
    }
    return totalMass > 0 ? com / totalMass : Vec3();
  }

  void clear() { bodies.clear(); }
  
  double getEnergyError() const {
    double currentEnergy = ConservationLaws::totalEnergy(bodies);
    return ConservationLaws::energyError(initialEnergy, currentEnergy);
  }
  
  Vec3 getMomentumError() const {
    Vec3 currentMomentum = ConservationLaws::totalMomentum(bodies);
    return currentMomentum - initialMomentum;
  }
  
  void printConservationStats() const {
    std::cout << "Energy error: " << getEnergyError() * 100 << "%\n";
    Vec3 pErr = getMomentumError();
    std::cout << "Momentum error: (" << pErr.x << ", " << pErr.y << ", " << pErr.z << ")\n";
  }
};
