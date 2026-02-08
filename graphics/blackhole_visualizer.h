#pragma once
#include "../core/vec3.h"
#include "../physics/relativity.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

class BlackHoleVisualizer {
public:
    static void drawEventHorizon(Vec3 pos, double mass, int segments = 32) {
        double rs = Relativity::schwarzschildRadius(mass);
        double renderRadius = rs * 1000;
        
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        glPushMatrix();
        glTranslated(pos.x, pos.y, pos.z);
        
        GLUquadric* quad = gluNewQuadric();
        gluSphere(quad, renderRadius, segments, segments);
        gluDeleteQuadric(quad);
        
        glPopMatrix();
        glEnable(GL_LIGHTING);
    }
    
    static void drawPhotonSphere(Vec3 pos, double mass, int segments = 64) {
        double rph = Relativity::photonSphere(mass);
        double renderRadius = rph * 1000;
        
        glDisable(GL_LIGHTING);
        glLineWidth(3.0f);
        glColor4f(1.0f, 0.5f, 0.0f, 0.8f);
        glPushMatrix();
        glTranslated(pos.x, pos.y, pos.z);
        
        for(int axis = 0; axis < 3; axis++) {
            glBegin(GL_LINE_LOOP);
            for(int i = 0; i < segments; i++) {
                double angle = 2.0 * M_PI * i / segments;
                if(axis == 0) glVertex3d(renderRadius * cos(angle), renderRadius * sin(angle), 0);
                else if(axis == 1) glVertex3d(renderRadius * cos(angle), 0, renderRadius * sin(angle));
                else glVertex3d(0, renderRadius * cos(angle), renderRadius * sin(angle));
            }
            glEnd();
        }
        
        glPopMatrix();
        glLineWidth(1.0f);
        glEnable(GL_LIGHTING);
    }
    
    static void drawAccretionDiskGlow(Vec3 pos, double innerR, double outerR, int segments = 64) {
        double renderInner = innerR * 1000;
        double renderOuter = outerR * 1000;
        
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        
        glPushMatrix();
        glTranslated(pos.x, pos.y, pos.z);
        
        for(int ring = 0; ring < 20; ring++) {
            double r1 = renderInner + (renderOuter - renderInner) * ring / 20.0;
            double r2 = renderInner + (renderOuter - renderInner) * (ring + 1) / 20.0;
            double temp = 1e7 * pow(r1 / renderInner, -0.75);
            float intensity = fmin(1.0, temp / 1e8);
            
            glBegin(GL_QUAD_STRIP);
            for(int i = 0; i <= segments; i++) {
                double angle = 2.0 * M_PI * i / segments;
                double c = cos(angle);
                double s = sin(angle);
                
                glColor4f(intensity, intensity * 0.7f, intensity * 0.3f, 0.6f);
                glVertex3d(r1 * c, r1 * s, 0);
                glVertex3d(r2 * c, r2 * s, 0);
            }
            glEnd();
        }
        
        glPopMatrix();
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    }
    
    static void drawGravitationalLensing(Vec3 bhPos, double mass, Vec3 cameraPos) {
        double rs = Relativity::schwarzschildRadius(mass);
        double lensRadius = rs * 5000;
        
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(2.0f);
        
        glPushMatrix();
        glTranslated(bhPos.x, bhPos.y, bhPos.z);
        
        for(int ring = 1; ring <= 5; ring++) {
            double r = lensRadius * ring / 5.0;
            float alpha = 0.3f / ring;
            
            glBegin(GL_LINE_LOOP);
            glColor4f(0.3f, 0.5f, 1.0f, alpha);
            for(int i = 0; i < 64; i++) {
                double angle = 2.0 * M_PI * i / 64;
                glVertex3d(r * cos(angle), r * sin(angle), 0);
            }
            glEnd();
        }
        
        glPopMatrix();
        glLineWidth(1.0f);
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    }
};
