#pragma once
#include "../core/vec3.h"
#include <cmath>

class MagneticField {
    double strength;
    Vec3 axis;
    
public:
    MagneticField(double B0 = 1e-4, Vec3 ax = Vec3(0,0,1)) 
        : strength(B0), axis(ax.normalized()) {}
    
    Vec3 getField(Vec3 pos) const {
        double r = pos.length();
        if(r < 1e-10) return Vec3(0,0,0);
        
        Vec3 dipole = axis * (3.0 * axis.dot(pos) / (r*r)) - pos / (r*r);
        return dipole * (strength * pow(r, -3));
    }
    
    Vec3 lorentzForce(Vec3 pos, Vec3 vel, double charge) const {
        Vec3 B = getField(pos);
        return vel.cross(B) * charge;
    }
    
    void setStrength(double B) { strength = B; }
    void setAxis(Vec3 ax) { axis = ax.normalized(); }
};
