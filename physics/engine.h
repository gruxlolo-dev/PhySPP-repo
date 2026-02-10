#pragma once
#include "body.h"
#include "physics_body.h"
#include "deterministic_engine.h"
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
#include <algorithm>
#include <cmath>

enum class IntegratorType { EULER, VERLET, RK4, LEAPFROG, YOSHIDA4 };

enum class BodyFlags {
    NONE = 0,
    STATIC = 1 << 0,
    NO_INTEGRATION = 1 << 1,
    GRAVITY_ONLY = 1 << 2,
    SLEEPING = 1 << 3
};

struct BoundingBox {
    Vec3 min, max;
    Vec3 center() const { return (min + max) * 0.5; }
    double size() const { return (max - min).length(); }
};

inline constexpr int operator|(BodyFlags a, BodyFlags b) {
    return static_cast<int>(a) | static_cast<int>(b);
}

inline constexpr int operator&(int a, BodyFlags b) {
    return a & static_cast<int>(b);
}

class PhysicsEngine {
    std::vector<BodyPtr> bodies;
    std::unique_ptr<DeterministicPhysicsEngine> deterministicEngine;
    std::vector<Vec3> forces;
    IntegratorType integrator = IntegratorType::VERLET;
    bool useRelativistic = false;
    bool detectCollisions = true;
    bool useDeterministicPhysics = false;
    bool useTidalForces = false;
    bool useMHD = false;
    bool useRadiativeTransfer = false;
    bool useParticleInteractions = false;
    bool useOctree = false;
    bool useConstraints = false;
    bool useSleeping = false;
    bool useAdaptiveTimestep = false;
    double timeScale = 1.0;
    double initialEnergy = 0;
    Vec3 initialMomentum;
    std::unique_ptr<Octree> octree;
    bool finalized = false;
    double sleepThreshold = 1e-6;
    double minTimestep = 1e-3;
    double maxTimestep = 1e6;
    size_t stepCount = 0;

public:
    void add(BodyPtr body) {
        bodies.push_back(body);
        if (bodies.size() == 1) {
            initialEnergy = ConservationLaws::totalEnergy(bodies);
            initialMomentum = ConservationLaws::totalMomentum(bodies);
        }
    }

    void setIntegrator(IntegratorType type) { integrator = type; }
    void enableRelativistic(bool enable) { useRelativistic = enable; }
    void enableCollisions(bool enable) { detectCollisions = enable; }

    void enableDeterministicPhysics(bool enable) {
        useDeterministicPhysics = enable;
        if (enable && !deterministicEngine) {
            deterministicEngine = std::make_unique<DeterministicPhysicsEngine>();
            syncToDeterministic();
        }
    }

    void enableTidalForces(bool enable) { useTidalForces = enable; }
    void enableMHD(bool enable) { useMHD = enable; }
    void enableRadiativeTransfer(bool enable) { useRadiativeTransfer = enable; }
    void enableParticleInteractions(bool enable) { useParticleInteractions = enable; }
    void enableOctree(bool enable) { useOctree = enable; }
    void enableConstraints(bool enable) { useConstraints = enable; }
    void enableSleeping(bool enable) { useSleeping = enable; }
    void enableAdaptiveTimestep(bool enable) { useAdaptiveTimestep = enable; }
    void setTimeScale(double scale) { timeScale = scale; }
    void setSleepThreshold(double threshold) { sleepThreshold = threshold; }

    void setTimestepRange(double minDt, double maxDt) {
        minTimestep = minDt;
        maxTimestep = maxDt;
    }

    BodyPtr createBody() {
        auto body = std::make_shared<Body>(Vec3(), Vec3(), 1.0, 1.0);
        bodies.push_back(body);
        return body;
    }

    void finalize() {
        finalized = true;
        forces.resize(bodies.size());
        if (bodies.size() > 0) {
            initialEnergy = ConservationLaws::totalEnergy(bodies);
            initialMomentum = ConservationLaws::totalMomentum(bodies);
        }
    }

    BoundingBox getBoundingBox() const {
        if (bodies.empty()) return {{0, 0, 0}, {0, 0, 0}};
        Vec3 minPos = bodies[0]->pos;
        Vec3 maxPos = bodies[0]->pos;
        for (const auto &b: bodies) {
            if (b->destroyed) continue;
            minPos.x = std::min(minPos.x, b->pos.x);
            minPos.y = std::min(minPos.y, b->pos.y);
            minPos.z = std::min(minPos.z, b->pos.z);
            maxPos.x = std::max(maxPos.x, b->pos.x);
            maxPos.y = std::max(maxPos.y, b->pos.y);
            maxPos.z = std::max(maxPos.z, b->pos.z);
        }
        return {minPos, maxPos};
    }

    const std::vector<BodyPtr> &getBodies() const { return bodies; }
    size_t getStepCount() const { return stepCount; }

    void computeForces() {
        if (forces.size() != bodies.size()) {
            forces.resize(bodies.size());
        }
        std::fill(forces.begin(), forces.end(), Vec3(0, 0, 0));

        for (size_t i = 0; i < bodies.size(); ++i) {
            auto &body = bodies[i];
            if (!body || body->destroyed || body->flags & 3) continue;

            for (size_t j = i + 1; j < bodies.size(); ++j) {
                auto &other = bodies[j];
                if (!other || other->destroyed) continue;

                Vec3 force = useRelativistic
                                 ? Forces::relativisticGravity(*body, *other)
                                 : Forces::gravity(*body, *other);

                forces[i] += force;
                forces[j] -= force;

                if (useTidalForces) {
                    Vec3 tidal = Forces::tidalForce(*body, *other);
                    forces[i] += tidal;
                }
            }

            if (useMHD) {
                Vec3 B(0, 0, 1e-4);
                forces[i] += MHD::lorentzForce(body->vel, B, 1e-10);
            }

            if (useParticleInteractions) {
                forces[i] += ParticleInteractions::viscousForce(body->vel, 0.01, 1e-10);
            }

            if (useRadiativeTransfer) {
                double cooling = RadiativeTransfer::radiativeCooling(1e-10, 1e7);
                forces[i] += body->vel * (-cooling * 1e-20);
            }
        }
    }

    void step(double dt) {
        if (useDeterministicPhysics && deterministicEngine) {
            deterministicEngine->step(dt);
            syncFromDeterministic();
            return;
        }


        stepCount++;

        if (useAdaptiveTimestep) {
            dt = computeAdaptiveTimestep(dt);
        }

        if (forces.size() != bodies.size()) {
            forces.resize(bodies.size());
        }

        computeForces();

        for (size_t i = 0; i < bodies.size(); ++i) {
            auto &body = bodies[i];
            if (!body || body->destroyed || body->flags & 3) continue;

            if (useSleeping && body->vel.lengthSq() < sleepThreshold * sleepThreshold) {
                body->flags |= 8;
                continue;
            }

            auto forceFunc = [this, i](const Body &) { return forces[i]; };

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
                case IntegratorType::LEAPFROG:
                    Integrators::leapfrog(*body, dt, forceFunc);
                    break;
                case IntegratorType::YOSHIDA4:
                    Integrators::yoshida4(*body, dt, forceFunc);
                    break;
            }

            if (body->showTrail) {
                body->updateTrail();
            }
            if (!body->destroyed) {
                body->updateColorByVelocity();
            }
        }

        if (detectCollisions) {
            checkCollisions();
        }

        bodies.erase(std::remove_if(bodies.begin(), bodies.end(),
                                    [](const BodyPtr &b) { return !b || b->destroyed; }),
                     bodies.end());

        if (bodies.size() != forces.size()) {
            forces.resize(bodies.size());
        }
    }

    double computeAdaptiveTimestep(double dt) {
        double minDt = maxTimestep;
        for (const auto &body: bodies) {
            if (body->destroyed || body->flags & 3) continue;
            double acc = body->acc.length();
            if (acc > 1e-10) {
                double suggestedDt = std::sqrt(body->radius / acc) * 0.1;
                minDt = std::min(minDt, suggestedDt);
            }
        }
        return std::clamp(minDt, minTimestep, maxTimestep);
    }

    void checkCollisions() {
        std::vector<std::pair<size_t, size_t> > collisions;

        for (size_t i = 0; i < bodies.size(); ++i) {
            if (bodies[i]->destroyed) continue;
            for (size_t j = i + 1; j < bodies.size(); ++j) {
                if (bodies[j]->destroyed) continue;

                auto &a = bodies[i];
                auto &b = bodies[j];
                double dist = (a->pos - b->pos).length();
                if (dist < a->radius + b->radius) {
                    collisions.push_back({i, j});
                }
            }
        }

        for (auto &collision: collisions) {
            if (collision.first >= bodies.size() || collision.second >= bodies.size()) continue;
            auto &a = bodies[collision.first];
            auto &b = bodies[collision.second];
            if (!a->destroyed && !b->destroyed) {
                handleCollision(a, b);
            }
        }
    }

    void handleCollision(BodyPtr &a, BodyPtr &b) {
        if (!a || !b || a->destroyed || b->destroyed) return;

        Vec3 totalMomentum = a->vel * a->mass + b->vel * b->mass;
        double totalMass = a->mass + b->mass;
        Vec3 collisionPos = (a->pos * a->mass + b->pos * b->mass) / totalMass;
        Vec3 relativeVel = a->vel - b->vel;
        double relativeSpeed = relativeVel.length();


        double reducedMass = (a->mass * b->mass) / totalMass;
        double collisionEnergy = 0.5 * reducedMass * relativeSpeed * relativeSpeed;


        const double ELASTIC_THRESHOLD = 1e20;
        const double MERGE_THRESHOLD = 1e25;
        const double FRAGMENT_THRESHOLD = 1e30;
        const double ANNIHILATION_THRESHOLD = 1e35;

        a->kineticEnergyAtCollision = collisionEnergy;
        b->kineticEnergyAtCollision = collisionEnergy;


        if (collisionEnergy < ELASTIC_THRESHOLD) {
            Vec3 normal = (b->pos - a->pos).normalized();
            Vec3 relVel = a->vel - b->vel;
            double velAlongNormal = relVel.dot(normal);

            if (velAlongNormal > 0) return;

            double restitution = (a->elasticity + b->elasticity) * 0.5;
            double impulse = -(1 + restitution) * velAlongNormal;
            impulse /= (1.0 / a->mass + 1.0 / b->mass);

            Vec3 impulseVec = normal * impulse;
            a->vel += impulseVec / a->mass;
            b->vel -= impulseVec / b->mass;

            a->lastCollisionType = Body::CollisionType::ELASTIC_BOUNCE;
            b->lastCollisionType = Body::CollisionType::ELASTIC_BOUNCE;

            // Rozdziel ciała
            Vec3 separation = normal * ((a->radius + b->radius) * 1.01 - (b->pos - a->pos).length());
            a->pos -= separation * 0.5;
            b->pos += separation * 0.5;
            return;
        }

        // 2. ŚREDNIA ENERGIA - Połączenie (fuzja)
        if (collisionEnergy < MERGE_THRESHOLD) {
            auto merged = std::make_shared<Body>(collisionPos, totalMomentum / totalMass, totalMass,
                                                 std::pow(
                                                     a->radius * a->radius * a->radius + b->radius * b->radius * b->
                                                     radius, 1.0 / 3.0));
            merged->label = "MRG";
            merged->setColor(0.9f, 0.7f, 0.3f);
            merged->colorByVelocity = true;
            merged->maxVelocityForColor = relativeSpeed * 2;
            merged->lastCollisionType = Body::CollisionType::MERGE;

            a->destroy();
            b->destroy();
            bodies.push_back(merged);
            return;
        }

        // 3. WYSOKA ENERGIA - Fragmentacja (powstawanie nowych cząstek)
        if (collisionEnergy < FRAGMENT_THRESHOLD) {
            int numFragments = 3 + rand() % 5;
            double fragmentMass = totalMass / numFragments;
            double fragmentRadius = std::pow(fragmentMass / totalMass, 1.0 / 3.0) * std::max(a->radius, b->radius);

            // Część energii zamienia się w masę nowych cząstek (E=mc²)
            double energyToMass = collisionEnergy * 0.01 / (Physics::C * Physics::C);
            fragmentMass += energyToMass / numFragments;

            std::vector<BodyPtr> newFragments;
            for (int i = 0; i < numFragments; i++) {
                double theta = 2.0 * M_PI * i / numFragments + (rand() / (double) RAND_MAX) * 0.5;
                double phi = M_PI * (rand() / (double) RAND_MAX - 0.5);
                double speed = relativeSpeed * 0.4 * (0.5 + rand() / (double) RAND_MAX);

                Vec3 dir(cos(theta) * cos(phi), sin(phi), sin(theta) * cos(phi));
                Vec3 fragmentVel = totalMomentum / totalMass + dir * speed;
                Vec3 fragmentPos = collisionPos + dir * (fragmentRadius * 3);

                auto fragment = std::make_shared<Body>(fragmentPos, fragmentVel, fragmentMass, fragmentRadius);
                fragment->label = "FRG";
                fragment->isFragment = true;
                fragment->colorByVelocity = true;
                fragment->maxVelocityForColor = relativeSpeed * 2;
                fragment->setColor(1.0f, 0.5f, 0.0f);
                fragment->lastCollisionType = Body::CollisionType::FRAGMENTATION;
                newFragments.push_back(fragment);
            }

            a->destroy();
            b->destroy();

            for (auto &frag: newFragments) {
                bodies.push_back(frag);
            }
            return;
        }

        // 4. BARDZO WYSOKA ENERGIA - Anihilacja (cząstka + antycząstka)
        if (collisionEnergy >= ANNIHILATION_THRESHOLD) {
            // Całkowita anihilacja - energia zamienia się w promieniowanie
            // Tworzymy kilka "fotonów" (małe szybkie cząstki)
            int numPhotons = 4 + rand() % 4;
            double photonEnergy = (a->mass + b->mass) * Physics::C * Physics::C / numPhotons;
            double photonMass = photonEnergy / (Physics::C * Physics::C) * 0.001; // Bardzo małe

            for (int i = 0; i < numPhotons; i++) {
                double theta = 2.0 * M_PI * i / numPhotons;
                double phi = M_PI * (rand() / (double) RAND_MAX - 0.5);
                double speed = Physics::C * 0.1; // 10% prędkości światła

                Vec3 dir(cos(theta) * cos(phi), sin(phi), sin(theta) * cos(phi));
                Vec3 photonVel = dir * speed;
                Vec3 photonPos = collisionPos + dir * 1e6;

                auto photon = std::make_shared<Body>(photonPos, photonVel, photonMass, 1e5);
                photon->label = "γ"; // Gamma ray
                photon->setColor(1.0f, 1.0f, 0.0f);
                photon->setEmissive(1.0f);
                photon->showTrail = true;
                photon->colorByVelocity = false;
                photon->lastCollisionType = Body::CollisionType::ANNIHILATION;
                bodies.push_back(photon);
            }

            a->destroy();
            b->destroy();
            return;
        }
    }

    double totalEnergy() const {
        double KE = 0, PE = 0;
        for (const auto &b: bodies) {
            if (!b->destroyed)
                KE += b->kineticEnergy();
        }
        for (size_t i = 0; i < bodies.size(); ++i) {
            for (size_t j = i + 1; j < bodies.size(); ++j) {
                if (bodies[i]->destroyed || bodies[j]->destroyed) continue;
                double r = (bodies[i]->pos - bodies[j]->pos).length();
                if (r > 0) {
                    PE -= Physics::G * bodies[i]->mass * bodies[j]->mass / r;
                }
            }
        }
        return KE + PE;
    }

    Vec3 centerOfMass() const {
        Vec3 com(0, 0, 0);
        double totalMass = 0;
        for (const auto &b: bodies) {
            if (!b->destroyed) {
                com += b->pos * b->mass;
                totalMass += b->mass;
            }
        }
        return totalMass > 0 ? com / totalMass : Vec3();
    }

    void clear() {
        bodies.clear();
        forces.clear();
        stepCount = 0;
    }

    double getEnergyError() const {
        if (initialEnergy == 0) return 0;
        double currentEnergy = totalEnergy();
        return std::abs(currentEnergy - initialEnergy) / std::abs(initialEnergy);
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

    void stepFast(double dt) {
        dt *= timeScale;
        stepCount++;

        for (auto &body: bodies) {
            if (!body || body->destroyed || body->flags & 3) continue;

            Vec3 totalForce(0, 0, 0);
            for (auto &other: bodies) {
                if (!other || body == other || other->destroyed) continue;
                totalForce += Forces::gravity(*body, *other);
            }

            Vec3 acc = totalForce / body->mass;
            body->vel += acc * (dt * 0.5);
            body->pos += body->vel * dt;

            totalForce = Vec3(0, 0, 0);
            for (auto &other: bodies) {
                if (!other || body == other || other->destroyed) continue;
                totalForce += Forces::gravity(*body, *other);
            }
            acc = totalForce / body->mass;
            body->vel += acc * (dt * 0.5);
            body->acc = acc;
        }
    }

    void syncToDeterministic() {
        if (!deterministicEngine) return;

        for (auto &body: bodies) {
            if (!body || body->destroyed) continue;

            auto pb = std::make_shared<PhysicsBody>(
                body->pos, body->vel, body->mass, body->radius, PhysicsBodyType::RIGID_BODY
            );
            pb->name = body->name;
            pb->label = body->label;
            pb->angularVelocity = Vec3(0, 0, 0);
            pb->restitution = body->elasticity;
            pb->internalEnergy = 0;
            pb->temperature = body->temperature;
            pb->setColor(body->color[0], body->color[1], body->color[2]);
            pb->emissive = body->emissive;
            pb->showTrail = body->showTrail;

            deterministicEngine->addBody(pb);
        }
    }

    void syncFromDeterministic() {
        if (!deterministicEngine) return;

        bodies.clear();
        auto &detBodies = deterministicEngine->getBodies();

        for (auto &pb: detBodies) {
            if (!pb || pb->destroyed) continue;

            auto body = std::make_shared<Body>(pb->position, pb->velocity, pb->mass, pb->radius);
            body->name = pb->name;
            body->label = pb->label;
            body->elasticity = pb->restitution;
            body->temperature = pb->temperature;
            body->setColor(pb->color[0], pb->color[1], pb->color[2]);
            body->emissive = pb->emissive;
            body->showTrail = pb->showTrail;
            body->trail = pb->trail;

            bodies.push_back(body);
        }
    }

    void wakeAll() {
        for (auto &body: bodies) {
            body->flags &= ~8;
        }
    }

    size_t countActive() const {
        size_t count = 0;
        for (const auto &b: bodies) {
            if (!b->destroyed && !(b->flags & 8)) count++;
        }
        return count;
    }

    double getTimeScale() const { return timeScale; }

    DeterministicPhysicsEngine *getDeterministicEngine() {
        return deterministicEngine.get();
    }
};