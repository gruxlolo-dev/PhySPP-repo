#pragma once
#include "../core/vec3.h"
#include <cmath>

class Thermodynamics {
public:
    static double accretionHeating(double mdot, double radius, double mass) {
        double eddingtonRate = 2.2e-8 * mass / Physics::SOLAR_MASS;
        double efficiency = 0.1;
        double luminosity = efficiency * mdot * Physics::C * Physics::C;
        return luminosity / (4.0 * M_PI * radius * radius);
    }
    
    static double viscousHeating(double density, double viscosity, double shearRate) {
        return viscosity * density * shearRate * shearRate;
    }
    
    static double compressionalHeating(double pressure, double volumeChangeRate) {
        return pressure * volumeChangeRate;
    }
    
    static double radiativeCooling(double temperature, double density) {
        const double sigma = 5.67e-8;
        return sigma * pow(temperature, 4);
    }
    
    static double soundSpeed(double temperature, double meanMolecularWeight = 0.6) {
        const double k = 1.38e-23;
        const double m_p = 1.67e-27;
        return sqrt(k * temperature / (meanMolecularWeight * m_p));
    }
    
    static double pressureScaleHeight(double temperature, double mass, double radius) {
        double cs = soundSpeed(temperature);
        double omega = sqrt(Physics::G * mass / (radius * radius * radius));
        return cs / omega;
    }
    
    static double opticalDepth(double density, double radius, double opacity = 0.4) {
        return opacity * density * radius;
    }
};
