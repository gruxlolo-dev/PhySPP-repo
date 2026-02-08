#pragma once
#include "../core/vec3.h"
#include <cmath>
#include <vector>

struct VisualEffect {
  enum Type { GLOW, BLOOM, LENS_FLARE, MOTION_BLUR, ATMOSPHERIC_GLOW };
  Type type;
  bool enabled = true;
  float intensity = 1.0f;
};

class EffectsManager {
  std::vector<VisualEffect> effects;

public:
  void addGlow(float intensity = 1.0f) {
    effects.push_back({VisualEffect::GLOW, true, intensity});
  }

  void addBloom(float intensity = 0.8f) {
    effects.push_back({VisualEffect::BLOOM, true, intensity});
  }

  void addLensFlare(float intensity = 0.5f) {
    effects.push_back({VisualEffect::LENS_FLARE, true, intensity});
  }

  void addMotionBlur(float intensity = 0.3f) {
    effects.push_back({VisualEffect::MOTION_BLUR, true, intensity});
  }

  void addAtmosphericGlow(float intensity = 0.6f) {
    effects.push_back({VisualEffect::ATMOSPHERIC_GLOW, true, intensity});
  }

  const std::vector<VisualEffect> &getEffects() const { return effects; }

  void toggle(VisualEffect::Type type) {
    for (auto &e : effects) {
      if (e.type == type)
        e.enabled = !e.enabled;
    }
  }

  bool isEnabled(VisualEffect::Type type) const {
    for (const auto &e : effects) {
      if (e.type == type)
        return e.enabled;
    }
    return false;
  }

  float getIntensity(VisualEffect::Type type) const {
    for (const auto &e : effects) {
      if (e.type == type)
        return e.intensity;
    }
    return 0.0f;
  }
};

class TrailRenderer {
  struct TrailPoint {
    Vec3 pos;
    float alpha;
  };

public:
  static std::vector<TrailPoint> generateTrail(const std::vector<Vec3> &points,
                                               int maxPoints = 100) {
    std::vector<TrailPoint> trail;
    int skip = std::max(1, (int)points.size() / maxPoints);

    for (size_t i = 0; i < points.size(); i += skip) {
      float alpha = (float)i / points.size();
      trail.push_back({points[i], alpha});
    }
    return trail;
  }
};
