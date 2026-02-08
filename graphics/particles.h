#pragma once
#include "../core/vec3.h"
#include <random>
#include <vector>

struct Particle {
  Vec3 pos, vel;
  float color[4] = {1, 1, 1, 1};
  float size = 1.0f;
  float life = 1.0f;
  float decay = 0.01f;

  void update(double dt) {
    pos += vel * dt;
    life -= decay * dt;
    color[3] = life;
  }

  bool isDead() const { return life <= 0; }
};

class ParticleSystem {
  std::vector<Particle> particles;
  std::mt19937 rng{std::random_device{}()};

public:
  void emit(Vec3 pos, Vec3 vel, int count, float spread = 1.0f) {
    std::normal_distribution<float> dist(0, spread);
    for (int i = 0; i < count; ++i) {
      Particle p;
      p.pos = pos;
      p.vel = vel + Vec3(dist(rng), dist(rng), dist(rng));
      p.life = 1.0f;
      particles.push_back(p);
    }
  }

  void update(double dt) {
    for (auto &p : particles)
      p.update(dt);
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [](const Particle &p) { return p.isDead(); }),
        particles.end());
  }

  const std::vector<Particle> &getParticles() const { return particles; }
  void clear() { particles.clear(); }
};
