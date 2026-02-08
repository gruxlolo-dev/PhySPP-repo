#pragma once
#include "../core/vec3.h"
#include <cmath>

class ElectromagneticForces {
public:
    static Vec3 coulombForce(Vec3 r, double q1, double q2) {
        const double k = 8.99e9;
        double dist = r.length();
        if(dist < 1e-10) return Vec3(0,0,0);
        return r * (k * q1 * q2 / (dist * dist * dist));
    }
    
    static Vec3 lorentzForce(Vec3 velocity, Vec3 magneticField, double charge) {
        return velocity.cross(magneticField) * charge;
    }
    
    static Vec3 electricField(Vec3 r, double charge) {
        const double k = 8.99e9;
        double dist = r.length();
        if(dist < 1e-10) return Vec3(0,0,0);
        return r * (k * charge / (dist * dist * dist));
    }
    
    static Vec3 magneticFieldDipole(Vec3 r, Vec3 magneticMoment) {
        double dist = r.length();
        if(dist < 1e-10) return Vec3(0,0,0);
        
        const double mu0 = 4.0 * M_PI * 1e-7;
        Vec3 m_dot_r = magneticMoment * magneticMoment.dot(r);
        return (m_dot_r * 3.0 - magneticMoment * (dist * dist)) * (mu0 / (4.0 * M_PI * dist * dist * dist * dist * dist));
    }
    
    static double radiationPressure(double intensity) {
        return intensity / Physics::C;
    }
};
