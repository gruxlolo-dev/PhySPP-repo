#pragma once
#include "../core/vec3.h"
#include "relativity.h"

enum class BlackHoleType { SCHWARZSCHILD, KERR, REISSNER_NORDSTROM };

class AdvancedBlackHole {
    Vec3 pos, vel;
    double mass, spin, charge;
    BlackHoleType type;
    
public:
    AdvancedBlackHole(Vec3 p, double m, BlackHoleType t = BlackHoleType::SCHWARZSCHILD)
        : pos(p), vel(0,0,0), mass(m), spin(0), charge(0), type(t) {}
    
    void setSpin(double a) { 
        spin = a; 
        if(spin != 0) type = BlackHoleType::KERR;
    }
    
    void setCharge(double Q) { 
        charge = Q;
        if(charge != 0) type = BlackHoleType::REISSNER_NORDSTROM;
    }
    
    double eventHorizonRadius() const {
        double rs = Relativity::schwarzschildRadius(mass);
        if(type == BlackHoleType::KERR) {
            double a = spin * rs / 2.0;
            return rs / 2.0 * (1.0 + sqrt(1.0 - 4.0 * a * a / (rs * rs)));
        }
        if(type == BlackHoleType::REISSNER_NORDSTROM) {
            const double k = 8.99e9;
            double rQ2 = k * charge * charge / (Physics::G * mass);
            return rs / 2.0 * (1.0 + sqrt(1.0 - 4.0 * rQ2 / (rs * rs)));
        }
        return rs;
    }
    
    double innerStableOrbit() const {
        if(type == BlackHoleType::KERR) {
            return Relativity::innerStableOrbit(mass, spin);
        }
        return 3.0 * Relativity::schwarzschildRadius(mass);
    }
    
    Vec3 getForce(Vec3 targetPos, double targetMass) const {
        if(type == BlackHoleType::KERR) {
            return Relativity::kerrMetricForce(targetPos - pos, Vec3(0,0,0), mass, spin);
        }
        
        Vec3 r = pos - targetPos;
        double dist = r.length();
        return r * (Physics::G * mass * targetMass / (dist * dist * dist));
    }
    
    Vec3 getPos() const { return pos; }
    double getMass() const { return mass; }
    double getSpin() const { return spin; }
    double getCharge() const { return charge; }
    BlackHoleType getType() const { return type; }
};
