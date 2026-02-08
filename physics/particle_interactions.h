#pragma once
#include "../core/vec3.h"
#include <cmath>

class ParticleInteractions {
public:
    static Vec3 viscousForce(Vec3 velocity, double viscosity, double density) {
        return velocity * (-viscosity * density);
    }
    
    static Vec3 collisionForce(Vec3 v1, Vec3 v2, double m1, double m2, double crossSection) {
        Vec3 vrel = v1 - v2;
        double vrel_mag = vrel.length();
        double collisionRate = crossSection * vrel_mag;
        return vrel * (-collisionRate * m2 / (m1 + m2));
    }
    
    static double meanFreePath(double density, double crossSection) {
        return 1.0 / (density * crossSection);
    }
    
    static double diffusionCoefficient(double temperature, double mass, double viscosity) {
        const double k = 1.38e-23;
        return k * temperature / (6.0 * M_PI * viscosity * sqrt(mass));
    }
    
    static Vec3 thermalDiffusion(Vec3 gradT, double diffusivity) {
        return gradT * (-diffusivity);
    }
    
    static double coulombCollisionRate(double density, double temperature) {
        const double e = 1.6e-19;
        const double epsilon0 = 8.85e-12;
        const double k = 1.38e-23;
        double logLambda = 15.0;
        return density * e*e*e*e * logLambda / (12.0 * M_PI * epsilon0*epsilon0 * sqrt(k*temperature) * temperature);
    }
};
