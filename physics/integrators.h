#pragma once
#include "body.h"
#include <functional>

namespace Integrators {
using ForceFunc = std::function<Vec3(const Body &)>;

inline void euler(Body &body, double dt, const ForceFunc &force) {
  Vec3 acc = force(body) / body.mass;
  body.vel += acc * dt;
  body.pos += body.vel * dt;
  body.acc = acc;
}

inline void verlet(Body &body, double dt, const ForceFunc &force) {
  Vec3 acc = force(body) / body.mass;
  body.pos += body.vel * dt + acc * (0.5 * dt * dt);
  Vec3 newAcc = force(body) / body.mass;
  body.vel += (acc + newAcc) * (0.5 * dt);
  body.acc = newAcc;
}

inline void rk4(Body &body, double dt, const ForceFunc &force) {
  Vec3 p0 = body.pos, v0 = body.vel;

  Vec3 k1v = force(body) / body.mass;
  Vec3 k1p = v0;

  body.pos = p0 + k1p * (dt * 0.5);
  body.vel = v0 + k1v * (dt * 0.5);
  Vec3 k2v = force(body) / body.mass;
  Vec3 k2p = body.vel;

  body.pos = p0 + k2p * (dt * 0.5);
  body.vel = v0 + k2v * (dt * 0.5);
  Vec3 k3v = force(body) / body.mass;
  Vec3 k3p = body.vel;

  body.pos = p0 + k3p * dt;
  body.vel = v0 + k3v * dt;
  Vec3 k4v = force(body) / body.mass;
  Vec3 k4p = body.vel;

  body.pos = p0 + (k1p + k2p * 2.0 + k3p * 2.0 + k4p) * (dt / 6.0);
  body.vel = v0 + (k1v + k2v * 2.0 + k3v * 2.0 + k4v) * (dt / 6.0);
  body.acc = k4v;
}

inline void leapfrog(Body &body, double dt, const ForceFunc &force) {
  Vec3 acc = force(body) / body.mass;
  body.vel += acc * (dt * 0.5);
  body.pos += body.vel * dt;
  Vec3 newAcc = force(body) / body.mass;
  body.vel += newAcc * (dt * 0.5);
  body.acc = newAcc;
}

inline void yoshida4(Body &body, double dt, const ForceFunc &force) {
  const double w0 = -1.702414383919315;
  const double w1 = 1.351207191959658;
  const double c1 = w1 / 2.0;
  const double c2 = (w0 + w1) / 2.0;
  const double d1 = w1;
  const double d2 = w0;
  
  body.pos += body.vel * (c1 * dt);
  Vec3 acc = force(body) / body.mass;
  body.vel += acc * (d1 * dt);
  body.pos += body.vel * (c2 * dt);
  acc = force(body) / body.mass;
  body.vel += acc * (d2 * dt);
  body.pos += body.vel * (c2 * dt);
  acc = force(body) / body.mass;
  body.vel += acc * (d1 * dt);
  body.pos += body.vel * (c1 * dt);
  body.acc = acc;
}
}
