#pragma once
#include "../core/vec3.h"
#include "../physics/body.h"
#include <GL/gl.h>
#include <vector>
#include <memory>

class VectorVisualizer {
public:
    static void drawVelocityVector(const Body& body, double scale = 1e-6) {
        glDisable(GL_LIGHTING);
        glLineWidth(2.0f);
        
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3d(body.pos.x, body.pos.y, body.pos.z);
        Vec3 end = body.pos + body.vel * scale;
        glVertex3d(end.x, end.y, end.z);
        glEnd();
        
        drawArrowHead(body.pos + body.vel * scale, body.vel.normalized(), body.radius * 2);
        
        glLineWidth(1.0f);
        glEnable(GL_LIGHTING);
    }
    
    static void drawForceVector(const Body& body, Vec3 force, double scale = 1e-20) {
        glDisable(GL_LIGHTING);
        glLineWidth(2.0f);
        
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3d(body.pos.x, body.pos.y, body.pos.z);
        Vec3 end = body.pos + force * scale;
        glVertex3d(end.x, end.y, end.z);
        glEnd();
        
        drawArrowHead(body.pos + force * scale, force.normalized(), body.radius * 2);
        
        glLineWidth(1.0f);
        glEnable(GL_LIGHTING);
    }
    
    static void drawAccelerationVector(const Body& body, double scale = 1e-8) {
        glDisable(GL_LIGHTING);
        glLineWidth(2.0f);
        
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3d(body.pos.x, body.pos.y, body.pos.z);
        Vec3 end = body.pos + body.acc * scale;
        glVertex3d(end.x, end.y, end.z);
        glEnd();
        
        drawArrowHead(body.pos + body.acc * scale, body.acc.normalized(), body.radius * 2);
        
        glLineWidth(1.0f);
        glEnable(GL_LIGHTING);
    }
    
    static void drawEnergyColor(const Body& body) {
        double ke = body.kineticEnergy();
        double maxEnergy = 1e40;
        float intensity = fmin(1.0, ke / maxEnergy);
        
        glDisable(GL_LIGHTING);
        glColor3f(intensity, 1.0f - intensity, 0.0f);
        
        GLUquadric* quad = gluNewQuadric();
        glPushMatrix();
        glTranslated(body.pos.x, body.pos.y, body.pos.z);
        gluSphere(quad, body.radius * body.renderScale * 1.1, 16, 16);
        glPopMatrix();
        gluDeleteQuadric(quad);
        
        glEnable(GL_LIGHTING);
    }
    
private:
    static void drawArrowHead(Vec3 pos, Vec3 dir, double size) {
        Vec3 perp1 = dir.cross(Vec3(0,1,0)).normalized();
        if(perp1.length() < 0.1) perp1 = dir.cross(Vec3(1,0,0)).normalized();
        Vec3 perp2 = dir.cross(perp1).normalized();
        
        glBegin(GL_TRIANGLES);
        Vec3 base = pos - dir * size;
        glVertex3d(pos.x, pos.y, pos.z);
        Vec3 p1 = base + perp1 * (size * 0.3);
        glVertex3d(p1.x, p1.y, p1.z);
        Vec3 p2 = base + perp2 * (size * 0.3);
        glVertex3d(p2.x, p2.y, p2.z);
        glEnd();
    }
};
