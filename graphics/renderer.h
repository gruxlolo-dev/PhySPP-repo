#pragma once
#include "../graphics/effects.h"
#include "../graphics/particles.h"
#include "../graphics/blackhole_visualizer.h"
#include "../physics/body.h"
#include "../physics/relativity.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <SDL2/SDL.h>
#include <cmath>

class Renderer {
  SDL_Window *window = nullptr;
  SDL_GLContext context;
  int width, height;

  Vec3 cameraPos{0, 0, 5e11};
  Vec3 cameraTarget{0, 0, 0};
  float cameraDistance = 5e11;
  float cameraAngleX = 0, cameraAngleY = 0;

  EffectsManager effects;
  ParticleSystem particles;

  bool showOrbits = true;
  bool showGrid = false;

public:
  Renderer(int w, int h, const char *title) : width(w), height(h) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window =
        SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    context = SDL_GL_CreateContext(window);

    int argc = 1;
    char *argv[1] = {(char*)"PhySPP"};
    glutInit(&argc, argv);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    effects.addGlow(1.0f);
    effects.addBloom(0.8f);
    effects.addLensFlare(0.5f);
  }

  ~Renderer() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

  void clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, cameraTarget.x,
              cameraTarget.y, cameraTarget.z, 0, 1, 0);
  }

  void drawBody(const Body &body) {
    double rs = 2.0 * Physics::G * body.mass / (Physics::C * Physics::C);
    
    if(body.mass > 5.0 * Physics::SOLAR_MASS) {
      double isco = Relativity::innerStableOrbit(body.mass, 0.0);
      BlackHoleVisualizer::drawEventHorizon(body.pos, body.mass);
      BlackHoleVisualizer::drawPhotonSphere(body.pos, body.mass);
      BlackHoleVisualizer::drawAccretionDiskGlow(body.pos, isco, isco * 10);
      BlackHoleVisualizer::drawGravitationalLensing(body.pos, body.mass, cameraPos);
      
      if(!body.label.empty()) {
        Vec3 screenPos = worldToScreen(body.pos);
        if(screenPos.z > 0) {
          drawText(body.label.c_str(), (int)screenPos.x + 10, (int)screenPos.y);
        }
      }
      return;
    }
    
    glPushMatrix();
    glTranslated(body.pos.x, body.pos.y, body.pos.z);

    float ambient[] = {body.color[0] * 0.2f, body.color[1] * 0.2f,
                       body.color[2] * 0.2f, 1};
    float diffuse[] = {body.color[0], body.color[1], body.color[2], 1};
    float emission[] = {body.color[0] * body.emissive,
                        body.color[1] * body.emissive,
                        body.color[2] * body.emissive, 1};

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

    double renderRadius = body.radius * body.renderScale;
    if(renderRadius < 1e6) renderRadius = 1e6;

    GLUquadric *quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluSphere(quad, renderRadius, 16, 16);
    gluDeleteQuadric(quad);

    if (body.emissive > 0 && effects.isEnabled(VisualEffect::GLOW)) {
      drawGlow(body);
    }

    glPopMatrix();

    if(!body.label.empty()) {
      Vec3 screenPos = worldToScreen(body.pos);
      if(screenPos.z > 0) {
        drawText(body.label.c_str(), (int)screenPos.x + 10, (int)screenPos.y);
      }
    }

    if (body.showTrail && !body.trail.empty()) {
      drawTrail(body.trail, body.color);
    }
  }

  Vec3 worldToScreen(const Vec3 &worldPos) {
    GLdouble modelview[16], projection[16];
    GLint viewport[4];
    GLdouble winX, winY, winZ;
    
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    gluProject(worldPos.x, worldPos.y, worldPos.z,
               modelview, projection, viewport,
               &winX, &winY, &winZ);
    
    return Vec3(winX, height - winY, winZ);
  }

  void drawGlow(const Body &body) {
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);

    float intensity = effects.getIntensity(VisualEffect::GLOW);
    for (int i = 1; i <= 3; ++i) {
      float alpha = intensity / (i * 2.0f);
      glColor4f(body.color[0], body.color[1], body.color[2], alpha);

      GLUquadric *quad = gluNewQuadric();
      gluSphere(quad, body.radius * body.renderScale * (1 + i * 0.3), 16, 16);
      gluDeleteQuadric(quad);
    }

    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
  }

  void drawTrail(const std::vector<Vec3> &trail, const float color[3]) {
    if (trail.size() < 2)
      return;

    glDisable(GL_LIGHTING);
    glBegin(GL_LINE_STRIP);

    for (size_t i = 0; i < trail.size(); ++i) {
      float alpha = (float)i / trail.size();
      float fade = alpha * alpha;
      glColor4f(color[0], color[1], color[2], fade * 0.6f);
      glVertex3d(trail[i].x, trail[i].y, trail[i].z);
    }

    glEnd();
    glEnable(GL_LIGHTING);
  }

  void drawGrid(double size, int divisions) {
    if (!showGrid)
      return;

    glDisable(GL_LIGHTING);
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);

    double step = size / divisions;
    for (int i = -divisions; i <= divisions; ++i) {
      glVertex3d(i * step, 0, -size);
      glVertex3d(i * step, 0, size);
      glVertex3d(-size, 0, i * step);
      glVertex3d(size, 0, i * step);
    }

    glEnd();
    glEnable(GL_LIGHTING);
  }

  void drawText(const char* text, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glColor3f(0.0f, 1.0f, 0.0f);
    glRasterPos2i(x, height - y);
    
    for(const char* c = text; *c != '\0'; c++) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  }

  void swap() { SDL_GL_SwapWindow(window); }

  void setCamera(Vec3 pos, Vec3 target) {
    cameraPos = pos;
    cameraTarget = target;
  }

  void focusOn(const Body &body, double distance) {
    cameraTarget = body.pos;
    cameraDistance = distance;
    updateCameraPosition();
  }

  void rotateCamera(float dx, float dy) {
    cameraAngleX += dx;
    cameraAngleY += dy;
    updateCameraPosition();
  }

  void zoom(float delta) {
    cameraDistance *= (1.0f + delta);
    updateCameraPosition();
  }

  void updateCameraPosition() {
    cameraPos.x =
        cameraTarget.x + cameraDistance * cos(cameraAngleY) * cos(cameraAngleX);
    cameraPos.y = cameraTarget.y + cameraDistance * sin(cameraAngleY);
    cameraPos.z =
        cameraTarget.z + cameraDistance * cos(cameraAngleY) * sin(cameraAngleX);
  }

  void toggleOrbits() { showOrbits = !showOrbits; }
  void toggleGrid() { showGrid = !showGrid; }

  void setPerspective() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / height, 1e6, 1e14);
    glMatrixMode(GL_MODELVIEW);
  }

  EffectsManager &getEffects() { return effects; }
  ParticleSystem &getParticles() { return particles; }
};
