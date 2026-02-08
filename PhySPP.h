#pragma once

#include "core/constants.h"
#include "core/vec3.h"

#include "physics/body.h"
#include "physics/engine.h"
#include "physics/forces.h"
#include "physics/integrators.h"
#include "physics/accretion_disk.h"
#include "physics/advanced_accretion_disk.h"
#include "physics/magnetic_field.h"
#include "physics/mhd.h"
#include "physics/radiation.h"
#include "physics/radiative_transfer.h"
#include "physics/relativity.h"
#include "physics/kerr_metric.h"
#include "physics/jet.h"
#include "physics/advanced_blackhole.h"
#include "physics/particle_interactions.h"
#include "physics/plasma_thermodynamics.h"
#include "physics/data_analyzer.h"
#include "physics/advanced_visualization.h"
#include "physics/conservation_laws.h"
#include "physics/thermodynamics.h"
#include "physics/electromagnetic_forces.h"
#include "physics/octree.h"

#include "graphics/effects.h"
#include "graphics/particles.h"
#include "graphics/renderer.h"
#include "graphics/blackhole_visualizer.h"
#include "graphics/spaghettification.h"
#include "graphics/vector_visualizer.h"

#include "api/presets.h"
#include "api/simulation.h"
#include "api/scenario_manager.h"

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
