#pragma once
#include "../core/vec3.h"
#include <cmath>

class RadiativeTransfer {
public:
    static double opticalDepth(double density, double distance, double opacity) {
        return opacity * density * distance;
    }
    
    static double radiativeFlux(double temperature, double opticalDepth) {
        const double sigma = 5.67e-8;
        double T4 = temperature * temperature * temperature * temperature;
        return sigma * T4 / (1.0 + opticalDepth);
    }
    
    static double comptonScattering(double photonEnergy, double electronTemp) {
        const double k = 1.38e-23;
        const double me_c2 = 8.19e-14;
        return photonEnergy * (4.0 * k * electronTemp / me_c2);
    }
    
    static double photoionization(double photonEnergy, double threshold) {
        if(photonEnergy < threshold) return 0.0;
        return 6.3e-18 * pow(threshold / photonEnergy, 3);
    }
    
    static double radiativeCooling(double density, double temperature) {
        const double Lambda = 1e-22;
        return Lambda * density * density * sqrt(temperature);
    }
    
    static double radiativeHeating(double flux, double density) {
        return flux / density;
    }
};
