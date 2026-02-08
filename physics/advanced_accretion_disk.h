#pragma once
#include "../core/vec3.h"
#include <vector>
#include <cmath>

struct DiskLayer {
    double radius, height, density, temperature, pressure;
    double angularVelocity, viscosity;
    Vec3 magneticField;
};

class AdvancedAccretionDisk {
    std::vector<DiskLayer> layers;
    double innerRadius, outerRadius;
    double alpha, mdot;
    
public:
    AdvancedAccretionDisk(double rIn, double rOut, int numLayers) 
        : innerRadius(rIn), outerRadius(rOut), alpha(0.1), mdot(1e-8) {
        for(int i=0; i<numLayers; i++) {
            double r = innerRadius * pow(outerRadius/innerRadius, (double)i/numLayers);
            DiskLayer layer;
            layer.radius = r;
            layer.height = 0.1 * r;
            layer.density = mdot / (3.0 * M_PI * alpha * layer.height * r);
            layer.temperature = 1e7 * pow(r / innerRadius, -0.75);
            layer.pressure = layer.density * layer.temperature;
            layer.angularVelocity = sqrt(Physics::G * Physics::SOLAR_MASS * 10 / (r*r*r));
            layer.viscosity = alpha * layer.pressure / layer.density;
            layer.magneticField = Vec3(0, 0, sqrt(8.0 * M_PI * layer.pressure));
            layers.push_back(layer);
        }
    }
    
    void updateLayer(DiskLayer& layer, double dt) {
        double cs = sqrt(layer.pressure / layer.density);
        double H = cs / layer.angularVelocity;
        layer.height = H;
        
        double Q = 1.5 * layer.viscosity * layer.density * layer.angularVelocity * layer.angularVelocity;
        double cooling = 5.67e-8 * pow(layer.temperature, 4);
        layer.temperature += (Q - cooling) * dt / layer.density;
        
        double dOmega_dr = -1.5 * layer.angularVelocity / layer.radius;
        double turbulence = alpha * cs * H * dOmega_dr;
        layer.viscosity = turbulence;
    }
    
    void update(double dt) {
        for(auto& layer : layers) {
            updateLayer(layer, dt);
        }
    }
    
    std::vector<DiskLayer>& getLayers() { return layers; }
    void setAccretionRate(double rate) { mdot = rate; }
    void setAlpha(double a) { alpha = a; }
};
