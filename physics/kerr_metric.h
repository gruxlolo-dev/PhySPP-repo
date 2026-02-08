#pragma once
#include "../core/vec3.h"
#include <cmath>

class KerrMetric {
    double mass, spin;
    double rs, a;
    
public:
    KerrMetric(double M, double a_spin) : mass(M), spin(a_spin) {
        const double c = 299792458.0;
        rs = 2.0 * Physics::G * mass / (c * c);
        a = spin * rs / 2.0;
    }
    
    double sigma(double r, double theta) const {
        return r*r + a*a * cos(theta)*cos(theta);
    }
    
    double delta(double r) const {
        return r*r - rs*r + a*a;
    }
    
    double A(double r, double theta) const {
        double sig = sigma(r, theta);
        return (r*r + a*a)*(r*r + a*a) - a*a * delta(r) * sin(theta)*sin(theta);
    }
    
    Vec3 frameDragging(Vec3 pos) const {
        double r = pos.length();
        double theta = acos(pos.z / r);
        double omega = 2.0 * a * rs * r / A(r, theta);
        return Vec3(-pos.y, pos.x, 0) * omega;
    }
    
    double timeDilation(double r, double theta) const {
        double sig = sigma(r, theta);
        double del = delta(r);
        return sqrt(sig * del / A(r, theta));
    }
    
    double gravitationalRedshift(double r, double theta) const {
        return 1.0 / timeDilation(r, theta);
    }
    
    double ergoSphereRadius(double theta) const {
        return rs / 2.0 * (1.0 + sqrt(1.0 - (2.0*a/rs)*(2.0*a/rs) * cos(theta)*cos(theta)));
    }
    
    double photonOrbitRadius() const {
        double Z1 = 1.0 + pow(1.0 - spin*spin, 1.0/3.0) * (pow(1.0 + spin, 1.0/3.0) + pow(1.0 - spin, 1.0/3.0));
        double Z2 = sqrt(3.0*spin*spin + Z1*Z1);
        return rs * (3.0 + Z2 - sqrt((3.0 - Z1)*(3.0 + Z1 + 2.0*Z2)));
    }
};
