#pragma once
#include "../core/vec3.h"
#include <vector>
#include <memory>

class ConservationLaws {
public:
    static double totalEnergy(const std::vector<std::shared_ptr<Body>>& bodies) {
        double kinetic = 0, potential = 0;
        
        for(const auto& b : bodies) {
            if(b->destroyed) continue;
            kinetic += 0.5 * b->mass * b->vel.lengthSq();
        }
        
        for(size_t i = 0; i < bodies.size(); i++) {
            for(size_t j = i+1; j < bodies.size(); j++) {
                if(bodies[i]->destroyed || bodies[j]->destroyed) continue;
                double r = (bodies[i]->pos - bodies[j]->pos).length();
                if(r > 1e-10) {
                    potential -= Physics::G * bodies[i]->mass * bodies[j]->mass / r;
                }
            }
        }
        
        return kinetic + potential;
    }
    
    static Vec3 totalMomentum(const std::vector<std::shared_ptr<Body>>& bodies) {
        Vec3 p(0,0,0);
        for(const auto& b : bodies) {
            if(!b->destroyed) {
                p += b->vel * b->mass;
            }
        }
        return p;
    }
    
    static Vec3 totalAngularMomentum(const std::vector<std::shared_ptr<Body>>& bodies, Vec3 origin = Vec3(0,0,0)) {
        Vec3 L(0,0,0);
        for(const auto& b : bodies) {
            if(!b->destroyed) {
                Vec3 r = b->pos - origin;
                Vec3 p = b->vel * b->mass;
                L += r.cross(p);
            }
        }
        return L;
    }
    
    static double energyError(double E0, double E1) {
        if(fabs(E0) < 1e-10) return 0;
        return fabs((E1 - E0) / E0);
    }
};
