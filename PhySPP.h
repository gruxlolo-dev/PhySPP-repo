#pragma once

#include "core/constants.h"
#include "core/vec3.h"

#include "physics/body.h"
#include "physics/engine.h"
#include "physics/forces.h"
#include "physics/integrators.h"

#include "graphics/effects.h"
#include "graphics/particles.h"
#include "graphics/renderer.h"

#include "api/presets.h"
#include "api/simulation.h"

namespace PhySPP {
using ::Physics::AU;
using ::Physics::C;
using ::Physics::EARTH_MASS;
using ::Physics::G;
using ::Physics::JUPITER_MASS;
using ::Physics::SOLAR_MASS;

using ::Presets::Asteroid;
using ::Presets::BlackHole;
using ::Presets::Comet;
using ::Presets::Earth;
using ::Presets::Jupiter;
using ::Presets::Mars;
using ::Presets::NeutronStar;
using ::Presets::Sun;
} // namespace PhySPP
