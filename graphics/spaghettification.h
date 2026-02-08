#pragma once
#include "../core/vec3.h"
#include "../physics/relativity.h"
#include <GL/gl.h>
#include <vector>

class SpaghettificationEffect {
    struct StretchedParticle {
        Vec3 pos;
        Vec3 stretchDir;
        float stretchFactor;
        float lifetime;
    };
    
    std::vector<StretchedParticle> particles;
    
public:
    void addParticle(Vec3 pos, Vec3 bhPos, double bhMass) {
        StretchedParticle p;
        p.pos = pos;
        p.stretchDir = (bhPos - pos).normalized();
        
        double dist = (bhPos - pos).length();
        double rs = Relativity::schwarzschildRadius(bhMass);
        p.stretchFactor = fmin(10.0, rs / dist);
        p.lifetime = 1.0;
        
        particles.push_back(p);
    }
    
    void update(double dt) {
        for(auto& p : particles) {
            p.lifetime -= dt * 2.0;
            p.stretchFactor *= 1.1;
        }
        
        particles.erase(
            std::remove_if(particles.begin(), particles.end(),
                [](const StretchedParticle& p) { return p.lifetime <= 0; }),
            particles.end()
        );
    }
    
    void render() {
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        
        for(const auto& p : particles) {
            glBegin(GL_LINES);
            glColor4f(1.0f, 0.5f, 0.0f, p.lifetime);
            
            Vec3 start = p.pos - p.stretchDir * (p.stretchFactor * 1e8);
            Vec3 end = p.pos + p.stretchDir * (p.stretchFactor * 1e8);
            
            glVertex3d(start.x, start.y, start.z);
            glVertex3d(end.x, end.y, end.z);
            glEnd();
        }
        
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    }
    
    void clear() { particles.clear(); }
};
