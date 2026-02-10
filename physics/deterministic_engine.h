#pragma once
#include "physics_body.h"
#include "physics_forces.h"
#include <vector>
#include <algorithm>
#include <cmath>

enum class CollisionResult {
  ELASTIC_BOUNCE, INELASTIC_MERGE, FRAGMENTATION, FUSION,
  ANNIHILATION, TIDAL_DISRUPTION, EXPLOSION
};

struct CollisionEvent {
  PhysicsBodyPtr a, b;
  Vec3 collisionPoint;
  double relativeSpeed;
  double collisionEnergy;
  CollisionResult result;
};

class DeterministicPhysicsEngine {
  std::vector<PhysicsBodyPtr> bodies;
  double totalEnergy = 0;
  double simulationTime = 0;
  
public:
  void addBody(PhysicsBodyPtr body) {
    bodies.push_back(body);
  }
  
  void step(double dt) {
    simulationTime += dt;
    
    for(auto &body : bodies) {
      if(body->destroyed || body->invMass == 0) continue;
      
      Vec3 totalForce(0, 0, 0);
      for(auto &other : bodies) {
        if(body == other || other->destroyed) continue;
        totalForce += PhysicsForces::gravity(*body, *other);
      }
      
      Vec3 acc = totalForce * body->invMass;
      body->velocity += acc * dt;
      body->position += body->velocity * dt;
      body->updateEnergy();
    }
    
    auto collisions = detectCollisions();
    for(auto &col : collisions) {
      resolveCollision(col);
    }
    
    bodies.erase(std::remove_if(bodies.begin(), bodies.end(),
                 [](const PhysicsBodyPtr &b) { return b->destroyed; }),
                 bodies.end());
  }
  
  std::vector<CollisionEvent> detectCollisions() {
    std::vector<CollisionEvent> collisions;
    
    for(size_t i = 0; i < bodies.size(); ++i) {
      if(bodies[i]->destroyed) continue;
      for(size_t j = i + 1; j < bodies.size(); ++j) {
        if(bodies[j]->destroyed) continue;
        
        auto &a = bodies[i];
        auto &b = bodies[j];
        double dist = (a->position - b->position).length();
        
        if(dist < a->radius + b->radius) {
          CollisionEvent col;
          col.a = a;
          col.b = b;
          col.collisionPoint = (a->position * b->mass + b->position * a->mass) / (a->mass + b->mass);
          col.relativeSpeed = (a->velocity - b->velocity).length();
          
          double reducedMass = (a->mass * b->mass) / (a->mass + b->mass);
          col.collisionEnergy = 0.5 * reducedMass * col.relativeSpeed * col.relativeSpeed;
          
          collisions.push_back(col);
        }
      }
    }
    
    return collisions;
  }
  
  void resolveCollision(CollisionEvent &col) {
    auto &a = col.a;
    auto &b = col.b;
    
    const double ELASTIC_THRESHOLD = 1e20;
    const double MERGE_THRESHOLD = 1e25;
    const double FRAGMENT_THRESHOLD = 1e30;
    
    if(col.collisionEnergy < ELASTIC_THRESHOLD) {
      elasticBounce(a, b);
    } else if(col.collisionEnergy < MERGE_THRESHOLD) {
      inelasticMerge(a, b);
    } else {
      fragmentate(a, b, col.collisionEnergy);
    }
  }
  
  void elasticBounce(PhysicsBodyPtr &a, PhysicsBodyPtr &b) {
    Vec3 normal = (b->position - a->position).normalized();
    Vec3 relVel = a->velocity - b->velocity;
    double velAlongNormal = relVel.dot(normal);
    
    if(velAlongNormal > 0) return;
    
    double e = 0.8;
    double j = -(1 + e) * velAlongNormal / (a->invMass + b->invMass);
    
    Vec3 impulse = normal * j;
    a->velocity += impulse * a->invMass;
    b->velocity -= impulse * b->invMass;
  }
  
  void inelasticMerge(PhysicsBodyPtr &a, PhysicsBodyPtr &b) {
    Vec3 totalMomentum = a->momentumVector() + b->momentumVector();
    double totalMass = a->mass + b->mass;
    
    auto merged = std::make_shared<PhysicsBody>(
      (a->position * a->mass + b->position * b->mass) / totalMass,
      totalMomentum / totalMass,
      totalMass,
      std::pow(a->radius*a->radius*a->radius + b->radius*b->radius*b->radius, 1.0/3.0),
      a->type
    );
    
    merged->label = "MRG";
    merged->setColor(0.9f, 0.7f, 0.3f);
    
    a->destroy();
    b->destroy();
    bodies.push_back(merged);
  }
  
  void fragmentate(PhysicsBodyPtr &a, PhysicsBodyPtr &b, double collisionEnergy) {
    Vec3 totalMomentum = a->momentumVector() + b->momentumVector();
    double totalMass = a->mass + b->mass;
    Vec3 collisionPoint = (a->position * a->mass + b->position * b->mass) / totalMass;
    
    int numFragments = 3 + rand() % 5;
    double fragmentMass = totalMass / numFragments;
    double fragmentRadius = std::pow(fragmentMass / totalMass, 1.0/3.0) * std::max(a->radius, b->radius);
    
    for(int i = 0; i < numFragments; i++) {
      double theta = 2.0 * M_PI * i / numFragments;
      double phi = M_PI * (rand() / (double)RAND_MAX - 0.5);
      
      Vec3 dir(cos(theta) * cos(phi), sin(phi), sin(theta) * cos(phi));
      double speed = std::sqrt(2.0 * collisionEnergy / (fragmentMass * numFragments));
      
      Vec3 fragmentVel = totalMomentum / totalMass + dir * speed * (0.5 + rand() / (double)RAND_MAX);
      Vec3 fragmentPos = collisionPoint + dir * fragmentRadius * 2;
      
      auto fragment = std::make_shared<PhysicsBody>(fragmentPos, fragmentVel, fragmentMass, fragmentRadius, PhysicsBodyType::ASTEROID_RUBBLE);
      fragment->label = "FRG";
      fragment->setColor(0.8f, 0.5f, 0.2f);
      bodies.push_back(fragment);
    }
    
    a->destroy();
    b->destroy();
  }
  
  const std::vector<PhysicsBodyPtr>& getBodies() const { return bodies; }
  double getTotalEnergy() const { return totalEnergy; }
  double getSimulationTime() const { return simulationTime; }
};
