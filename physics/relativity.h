#pragma once
#include "../core/vec3.h"
#include <cmath>

class Relativity {
public:
    static double schwarzschildRadius(double mass) {
        return 2.0 * Physics::G * mass / (Physics::C * Physics::C);
    }
    
    static double timeDilation(double r, double rs) {
        if(r <= rs) return 0.0;
        return sqrt(1.0 - rs / r);
    }
    
    static double gravitationalRedshift(double r, double rs) {
        if(r <= rs) return INFINITY;
        return 1.0 / sqrt(1.0 - rs / r);
    }
    
    static Vec3 kerrMetricForce(Vec3 pos, Vec3 vel, double mass, double spin) {
        double r = pos.length();
        double rs = schwarzschildRadius(mass);
        double a = spin * rs / 2.0;
        
        if(r < rs) return Vec3(0,0,0);
        
        double r2 = r * r;
        double a2 = a * a;
        double cos_theta = pos.z / r;
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        double rho2 = r2 + a2 * cos_theta * cos_theta;
        double delta = r2 - rs * r + a2;
        
        Vec3 n = pos.normalized();
        double gravMag = Physics::G * mass / r2;
        gravMag *= (1.0 + rs * r / rho2);
        Vec3 correction = n * (-gravMag);
        
        double omega = 2.0 * a * rs * r / ((r2 + a2) * (r2 + a2) - a2 * delta * sin_theta * sin_theta);
        Vec3 frameDrag = Vec3(-pos.y, pos.x, 0).normalized() * (omega * r * sin_theta);
        
        return correction + frameDrag * (Physics::G * mass / (r * r));
    }
    
    static double innerStableOrbit(double mass, double spin) {
        double rs = schwarzschildRadius(mass);
        double Z1 = 1.0 + pow(1.0 - spin*spin, 1.0/3.0) * (pow(1.0 + spin, 1.0/3.0) + pow(1.0 - spin, 1.0/3.0));
        double Z2 = sqrt(3.0 * spin * spin + Z1 * Z1);
        return rs * (3.0 + Z2 - sqrt((3.0 - Z1) * (3.0 + Z1 + 2.0 * Z2)));
    }
    
    static double photonSphere(double mass) {
        return 1.5 * schwarzschildRadius(mass);
    }
    
    static Vec3 gravitationalLensing(Vec3 lightDir, Vec3 massPos, double mass, Vec3 observerPos) {
        Vec3 toMass = massPos - observerPos;
        double b = toMass.cross(lightDir).length();
        double rs = schwarzschildRadius(mass);
        
        if(b < rs * 2.0) {
            return Vec3(0,0,0);
        }
        
        double deflection = 4.0 * Physics::G * mass / (Physics::C * Physics::C * b);
        Vec3 perpendicular = (toMass - lightDir * toMass.dot(lightDir)).normalized();
        return lightDir + perpendicular * deflection;
    }
};
