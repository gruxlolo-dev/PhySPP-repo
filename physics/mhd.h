#pragma once
#include "../core/vec3.h"
#include <cmath>

class MHD {
public:
    static Vec3 lorentzForce(Vec3 velocity, Vec3 magneticField, double chargeDensity) {
        return velocity.cross(magneticField) * chargeDensity;
    }
    
    static Vec3 magneticPressureGradient(Vec3 B, Vec3 gradB) {
        double B2 = B.x*B.x + B.y*B.y + B.z*B.z;
        const double mu0 = 4.0 * M_PI * 1e-7;
        return gradB * (B2 / (2.0 * mu0));
    }
    
    static Vec3 magneticTension(Vec3 B, Vec3 curlB) {
        const double mu0 = 4.0 * M_PI * 1e-7;
        return B.cross(curlB) / mu0;
    }
    
    static double alfvenSpeed(Vec3 B, double density) {
        const double mu0 = 4.0 * M_PI * 1e-7;
        double B2 = B.x*B.x + B.y*B.y + B.z*B.z;
        return sqrt(B2 / (mu0 * density));
    }
    
    static Vec3 inductionEquation(Vec3 B, Vec3 v, double eta) {
        return v.cross(B) * (-1.0) + B * (eta);
    }
};
