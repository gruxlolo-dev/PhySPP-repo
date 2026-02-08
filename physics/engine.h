#pragma once
#include "body.h"
#include "forces.h"
#include "integrators.h"
#include <memory>
#include <vector>

enum class IntegratorType { EULER, VERLET, RK4 };

class PhysicsEngine {
  std::vector<BodyPtr> bodies;
  IntegratorType integrator = IntegratorType::VERLET;
  bool useRelativistic = false;
  bool detectCollisions = true;
  bool useTidalForces = false;
  double timeScale = 1.0;

public:
  void add(BodyPtr body) { bodies.push_back(body); }

  void setIntegrator(IntegratorType type) { integrator = type; }
  void enableRelativistic(bool enable) { useRelativistic = enable; }
  void enableCollisions(bool enable) { detectCollisions = enable; }
  void enableTidalForces(bool enable) { useTidalForces = enable; }
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

        Vec3 force = useRelativistic
                         ? Forces::relativisticGravity(*body, *other)
                         : Forces::gravity(*body, *other);

        totalForce += force;

        if (useTidalForces) {
          totalForce += Forces::tidalForce(*body, *other);
        }
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
};
