#pragma once
#include "../core/vec3.h"
#include <cmath>

class Radiation {
public:
    static double bremsstrahlung(double density, double temperature) {
        const double sigma_T = 6.65e-29;
        return sigma_T * density * density * sqrt(temperature);
    }
    
    static double comptonCooling(double temperature, double photonDensity) {
        const double sigma_T = 6.65e-29;
        const double c = 299792458.0;
        return 4.0 * sigma_T * c * photonDensity * temperature;
    }
    
    static double radiativePressure(double luminosity, double distance) {
        const double c = 299792458.0;
        return luminosity / (4.0 * M_PI * distance * distance * c);
    }
    
    static double eddingtonLuminosity(double mass) {
        const double c = 299792458.0;
        const double sigma_T = 6.65e-29;
        const double m_p = 1.67e-27;
        return 4.0 * M_PI * Physics::G * mass * m_p * c / sigma_T;
    }
};
