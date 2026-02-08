#pragma once
#include "../core/vec3.h"
#include <vector>
#include <cmath>

struct DiskParticle {
    Vec3 pos, vel;
    double density, temperature, pressure;
    Vec3 magneticField;
    double mass, radius;
};

class AccretionDisk {
    std::vector<DiskParticle> particles;
    double innerRadius, outerRadius;
    double alpha;
    
public:
    AccretionDisk(double rIn, double rOut, int numParticles) 
        : innerRadius(rIn), outerRadius(rOut), alpha(0.1) {
        for(int i=0; i<numParticles; i++) {
            double r = innerRadius + (outerRadius - innerRadius) * i / numParticles;
            double theta = 2.0 * M_PI * rand() / RAND_MAX;
            DiskParticle p;
            p.pos = Vec3(r * cos(theta), r * sin(theta), (rand()%100 - 50) * 1e7);
            double v = sqrt(Physics::G * Physics::SOLAR_MASS * 10 / r);
            p.vel = Vec3(-v * sin(theta), v * cos(theta), 0);
            p.density = 1e-10 * pow(r / innerRadius, -1.5);
            p.temperature = 1e7 * pow(r / innerRadius, -0.75);
            p.pressure = p.density * p.temperature;
            p.magneticField = Vec3(0, 0, 1e-4);
            p.mass = 1e20;
            p.radius = 1e6;
            particles.push_back(p);
        }
    }
    
    void update(double dt, Vec3 bhPos, double bhMass) {
        for(auto& p : particles) {
            Vec3 r = p.pos - bhPos;
            double dist = r.length();
            Vec3 gravity = r * (-Physics::G * bhMass / (dist * dist * dist));
            
            Vec3 viscosity = p.vel * (-alpha * 0.01);
            Vec3 magnetic = p.magneticField.cross(p.vel) * 0.001;
            
            Vec3 acc = gravity + viscosity + magnetic;
            p.vel += acc * dt;
            p.pos += p.vel * dt;
            
            p.temperature *= (1.0 - 0.0001 * dt);
            if(p.temperature < 1e5) p.temperature = 1e5;
        }
    }
    
    std::vector<DiskParticle>& getParticles() { return particles; }
    void setViscosity(double a) { alpha = a; }
};
