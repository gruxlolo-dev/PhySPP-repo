#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "../physics/body.h"

class ScenarioManager {
public:
    static void saveScenario(const std::string& filename, 
                             const std::vector<BodyPtr>& bodies,
                             double timeStep, double timeScale) {
        std::ofstream file(filename);
        file << bodies.size() << "\n";
        file << timeStep << " " << timeScale << "\n";
        
        for(const auto& body : bodies) {
            file << body->pos.x << " " << body->pos.y << " " << body->pos.z << "\n";
            file << body->vel.x << " " << body->vel.y << " " << body->vel.z << "\n";
            file << body->mass << " " << body->radius << "\n";
            file << body->color[0] << " " << body->color[1] << " " << body->color[2] << "\n";
        }
        file.close();
    }
    
    static void loadScenario(const std::string& filename,
                             std::vector<BodyPtr>& bodies,
                             double& timeStep, double& timeScale) {
        std::ifstream file(filename);
        if(!file.is_open()) return;
        
        int numBodies;
        file >> numBodies;
        file >> timeStep >> timeScale;
        
        bodies.clear();
        for(int i=0; i<numBodies; i++) {
            Vec3 pos, vel;
            double mass, radius;
            float r, g, b;
            
            file >> pos.x >> pos.y >> pos.z;
            file >> vel.x >> vel.y >> vel.z;
            file >> mass >> radius;
            file >> r >> g >> b;
            
            auto body = std::make_shared<Body>(pos, vel, mass, radius);
            body->setColor(r, g, b);
            bodies.push_back(body);
        }
        file.close();
    }
};
