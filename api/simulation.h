#pragma once
#include "../api/presets.h"
#include "../graphics/renderer.h"
#include "../physics/engine.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>

class Simulation {
  std::unique_ptr<PhysicsEngine> physics;
  std::unique_ptr<Renderer> renderer;
  bool running = true;
  bool paused = false;
  double timeStep = 3600.0;

public:
  Simulation(int width = 1280, int height = 720,
             const char *title = "PhySPP Simulation")
      : physics(std::make_unique<PhysicsEngine>()),
        renderer(std::make_unique<Renderer>(width, height, title)) {
    renderer->setPerspective();
  }

  void addSun() { physics->add(Presets::Sun()); }
  void addEarth() { physics->add(Presets::Earth()); }
  void addMars() { physics->add(Presets::Mars()); }
  void addJupiter() { physics->add(Presets::Jupiter()); }

  void addBlackHole(Vec3 pos, double mass = 10 * Physics::SOLAR_MASS) {
    physics->add(Presets::BlackHole(pos, mass));
  }

  void addNeutronStar(Vec3 pos) { physics->add(Presets::NeutronStar(pos)); }

  void addAsteroid(Vec3 pos, Vec3 vel) {
    physics->add(Presets::Asteroid(pos, vel));
  }

  void addComet(Vec3 pos, Vec3 vel) { physics->add(Presets::Comet(pos, vel)); }

  void addBody(Vec3 pos, Vec3 vel, double mass, double radius) {
    physics->add(std::make_shared<Body>(pos, vel, mass, radius));
  }

  void useRK4() { physics->setIntegrator(IntegratorType::RK4); }
  void useVerlet() { physics->setIntegrator(IntegratorType::VERLET); }
  void useEuler() { physics->setIntegrator(IntegratorType::EULER); }

  void enableRelativity() { physics->enableRelativistic(true); }
  void disableRelativity() { physics->enableRelativistic(false); }

  void enableCollisions() { physics->enableCollisions(true); }
  void disableCollisions() { physics->enableCollisions(false); }

  void enableMHD() { physics->enableMHD(true); }
  void disableMHD() { physics->enableMHD(false); }
  
  void enableRadiativeTransfer() { physics->enableRadiativeTransfer(true); }
  void disableRadiativeTransfer() { physics->enableRadiativeTransfer(false); }
  
  void enableParticleInteractions() { physics->enableParticleInteractions(true); }
  void disableParticleInteractions() { physics->enableParticleInteractions(false); }

  void setTimeScale(double scale) { physics->setTimeScale(scale); }
  void setTimeStep(double dt) { timeStep = dt; }

  void enableGlow() { renderer->getEffects().addGlow(); }
  void enableBloom() { renderer->getEffects().addBloom(); }
  void enableLensFlare() { renderer->getEffects().addLensFlare(); }

  void toggleOrbits() { renderer->toggleOrbits(); }
  void toggleGrid() { renderer->toggleGrid(); }

  void focusOnBody(int index) {
    auto &bodies = physics->getBodies();
    if (index >= 0 && index < bodies.size()) {
      renderer->focusOn(*bodies[index], bodies[index]->radius * 10);
    }
  }

  void followBody(int index) {
    auto &bodies = physics->getBodies();
    if (index >= 0 && index < bodies.size()) {
      renderer->setCamera(bodies[index]->pos + Vec3(bodies[index]->radius * 5,
                                                    bodies[index]->radius * 5,
                                                    bodies[index]->radius * 3),
                          bodies[index]->pos);
    }
  }

  void setCamera(Vec3 pos, Vec3 target) { renderer->setCamera(pos, target); }

  void run() {
    SDL_Event event;
    SDL_SetRelativeMouseMode(SDL_TRUE);
    int followIndex = -1;
    bool autoFollow = true;
    int frameCount = 0;
    auto lastPrint = std::chrono::high_resolution_clock::now();

    std::cout << "\n=== Simulation Started ===\n";
    std::cout << "Bodies: " << physics->getBodies().size() << "\n";
    std::cout << "Time step: " << timeStep << " s\n\n";

    while (running) {

      while (SDL_PollEvent(&event)) {
        handleEvent(event);
        if (event.type == SDL_KEYDOWN) {
          if (event.key.keysym.sym >= SDLK_0 &&
              event.key.keysym.sym <= SDLK_9) {
            followIndex = event.key.keysym.sym - SDLK_0;
            autoFollow = true;
          } else if (event.key.keysym.sym == SDLK_f) {
            autoFollow = !autoFollow;
          }
        }
      }

      if (!paused) {
        physics->step(timeStep);
        frameCount++;
      }

      auto now = std::chrono::high_resolution_clock::now();
      if (std::chrono::duration<double>(now - lastPrint).count() >= 1.0) {
        std::cout << "\r[Frame " << frameCount << "] ";
        std::cout << "Bodies: " << physics->getBodies().size() << " | ";
        std::cout << "Energy: " << std::scientific << std::setprecision(3)
                  << physics->totalEnergy() << " J | ";
        Vec3 com = physics->centerOfMass();
        std::cout << "CoM: (" << std::fixed << std::setprecision(2)
                  << com.x / 1e9 << ", " << com.y / 1e9 << ", " << com.z / 1e9
                  << ") Gm";
        std::cout << std::flush;
        lastPrint = now;
      }

      if (autoFollow && !physics->getBodies().empty()) {
        if (followIndex < 0 || followIndex >= physics->getBodies().size()) {
          Vec3 com = physics->centerOfMass();
          double maxDist = 0;
          for (const auto &body : physics->getBodies()) {
            double dist = (body->pos - com).length();
            if (dist > maxDist)
              maxDist = dist;
          }
          double camDist = maxDist * 1.5;
          if (camDist < 1e10)
            camDist = 1e10;
          if (camDist > 5e11)
            camDist = 5e11;
          renderer->setCamera(com + Vec3(camDist, camDist * 0.5, camDist * 0.3),
                              com);
        } else {
          auto &body = physics->getBodies()[followIndex];
          double camDist = body->radius * 50;
          if (camDist < body->radius * 10)
            camDist = body->radius * 10;
          renderer->setCamera(body->pos +
                                  Vec3(camDist, camDist * 0.5, camDist * 0.3),
                              body->pos);
        }
      }

      renderer->clear();
      renderer->drawGrid(Physics::AU * 10, 20);

      for (const auto &body : physics->getBodies()) {
        renderer->drawBody(*body);
      }

      char info[256];
      snprintf(info, sizeof(info), "Frame: %d | Bodies: %d | Energy: %.2e J", 
               frameCount, (int)physics->getBodies().size(), physics->totalEnergy());
      renderer->drawText(info, 10, 20);

      Vec3 com = physics->centerOfMass();
      snprintf(info, sizeof(info), "CoM: (%.2f, %.2f, %.2f) Gm", 
               com.x/1e9, com.y/1e9, com.z/1e9);
      renderer->drawText(info, 10, 40);

      renderer->swap();
      SDL_Delay(16);
    }
  }

  void handleEvent(SDL_Event &event) {
    if (event.type == SDL_QUIT) {
      running = false;
    } else if (event.type == SDL_MOUSEMOTION) {
      renderer->rotateCamera(event.motion.xrel * 0.002f,
                             -event.motion.yrel * 0.002f);
    } else if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_SPACE:
        paused = !paused;
        break;
      case SDLK_q:
        running = false;
        break;
      case SDLK_o:
        renderer->toggleOrbits();
        break;
      case SDLK_g:
        renderer->toggleGrid();
        break;
      case SDLK_PLUS:
      case SDLK_EQUALS:
        setTimeScale(physics->getBodies()[0]->mass * 2);
        break;
      case SDLK_MINUS:
        setTimeScale(physics->getBodies()[0]->mass * 0.5);
        break;
      case SDLK_w:
        renderer->zoom(-0.1f);
        break;
      case SDLK_s:
        renderer->zoom(0.1f);
        break;
      case SDLK_m: {
        static bool slowMo = false;
        slowMo = !slowMo;
        physics->setTimeScale(slowMo ? 1.0 / 5.0 : 1.0);
        std::cout << "\n SLOW MOTION: " << (slowMo ? "ON (1/1,000,000)" : "OFF")
                  << "\n";
        break;
      }
      case SDLK_LEFT:
        renderer->rotateCamera(-0.1f, 0);
        break;
      case SDLK_RIGHT:
        renderer->rotateCamera(0.1f, 0);
        break;
      case SDLK_UP:
        renderer->rotateCamera(0, 0.1f);
        break;
      case SDLK_DOWN:
        renderer->rotateCamera(0, -0.1f);
        break;
      case SDLK_0:
      case SDLK_1:
      case SDLK_2:
      case SDLK_3:
      case SDLK_4:
      case SDLK_5:
      case SDLK_6:
      case SDLK_7:
      case SDLK_8:
      case SDLK_9:
        focusOnBody(event.key.keysym.sym - SDLK_0);
        break;
      }
    }
  }

  int getBodyCount() const { return physics->getBodies().size(); }
  double getTotalEnergy() const { return physics->totalEnergy(); }
  Vec3 getCenterOfMass() const { return physics->centerOfMass(); }

  void stop() { running = false; }
  void pause() { paused = true; }
  void resume() { paused = false; }
};
