#pragma once
#include <cmath>

class PlasmaThermodynamics {
public:
    static double ionizationFraction(double temperature, double density, double ionizationEnergy) {
        const double k = 1.38e-23;
        const double h = 6.626e-34;
        const double me = 9.11e-31;
        double ne = density;
        double Saha = 2.4e21 * pow(temperature, 1.5) / ne * exp(-ionizationEnergy / (k * temperature));
        return Saha / (1.0 + Saha);
    }
    
    static double recombinationRate(double temperature) {
        return 2.6e-19 * pow(temperature / 1e4, -0.7);
    }
    
    static double collisionalIonizationRate(double temperature, double threshold) {
        const double k = 1.38e-23;
        if(k * temperature < threshold) return 0.0;
        return 1e-13 * sqrt(k * temperature / threshold) * exp(-threshold / (k * temperature));
    }
    
    static double nuclearReactionRate(double temperature, double density) {
        const double T9 = temperature / 1e9;
        if(T9 < 0.01) return 0.0;
        return density * density * 1e-43 * pow(T9, -2.0/3.0) * exp(-3.38 / pow(T9, 1.0/3.0));
    }
    
    static double plasmaFrequency(double density) {
        const double e = 1.6e-19;
        const double me = 9.11e-31;
        const double epsilon0 = 8.85e-12;
        return sqrt(density * e * e / (me * epsilon0));
    }
    
    static double debyeLength(double temperature, double density) {
        const double k = 1.38e-23;
        const double e = 1.6e-19;
        const double epsilon0 = 8.85e-12;
        return sqrt(epsilon0 * k * temperature / (density * e * e));
    }
};
