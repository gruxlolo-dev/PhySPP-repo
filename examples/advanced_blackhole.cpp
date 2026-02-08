#include "PhySPP.h"
#include "physics/accretion_disk.h"
#include "physics/magnetic_field.h"
#include "physics/radiation.h"
#include "physics/relativity.h"
#include "physics/jet.h"
#include "physics/advanced_blackhole.h"
#include "physics/data_analyzer.h"
#include "api/scenario_manager.h"
#include <iostream>

int main() {
    std::cout << "Advanced Black Hole Simulation\n\n";
    
    Simulation sim(1920, 1080, "Advanced Black Hole");
    
    AdvancedBlackHole bh(Vec3(0,0,0), 50 * Physics::SOLAR_MASS, BlackHoleType::KERR);
    bh.setSpin(0.9);
    
    std::cout << "Event Horizon: " << bh.eventHorizonRadius() / 1e3 << " km\n";
    std::cout << "ISCO: " << bh.innerStableOrbit() / 1e3 << " km\n";
    
    AccretionDisk disk(bh.innerStableOrbit(), bh.innerStableOrbit() * 100, 500);
    disk.setViscosity(0.1);
    
    MagneticField magField(1e-4, Vec3(0,0,1));
    
    Jet jet(Vec3(0,0,1), 1e38);
    
    sim.addBlackHole(Vec3(0,0,0), 50 * Physics::SOLAR_MASS);
    
    for(auto& p : disk.getParticles()) {
        sim.addBody(p.pos, p.vel, p.mass, p.radius);
    }
    
    sim.useRK4();
    sim.enableRelativity();
    sim.enableCollisions();
    sim.enableGlow();
    sim.setTimeStep(100);
    
    std::cout << "\nSimulation started...\n";
    std::cout << "Press M for slow motion\n";
    std::cout << "Press Q to quit\n\n";
    
    sim.run();
    
    return 0;
}
