#pragma once
#include "../core/vec3.h"
#include <vector>

struct JetParticle {
    Vec3 pos, vel;
    double energy, lorentzFactor;
};

class Jet {
    std::vector<JetParticle> particles;
    Vec3 axis;
    double power;
    
public:
    Jet(Vec3 ax = Vec3(0,0,1), double P = 1e38) : axis(ax.normalized()), power(P) {}
    
    void emit(Vec3 origin, double dt) {
        const double c = 299792458.0;
        int numNew = (int)(power * dt / (1e20 * c * c));
        
        for(int i=0; i<numNew; i++) {
            JetParticle p;
            p.pos = origin + axis * 1e10;
            double speed = 0.99 * c;
            p.vel = axis * speed;
            p.lorentzFactor = 1.0 / sqrt(1.0 - (speed/c) * (speed/c));
            p.energy = 1e20 * c * c * p.lorentzFactor;
            particles.push_back(p);
        }
    }
    
    void update(double dt) {
        for(auto& p : particles) {
            p.pos += p.vel * dt;
        }
        
        particles.erase(
            std::remove_if(particles.begin(), particles.end(),
                [](const JetParticle& p) { return p.pos.length() > 1e14; }),
            particles.end()
        );
    }
    
    std::vector<JetParticle>& getParticles() { return particles; }
    void setPower(double P) { power = P; }
    void setAxis(Vec3 ax) { axis = ax.normalized(); }
};
