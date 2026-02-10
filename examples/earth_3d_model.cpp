#include "PhySPP.h"
#include <iostream>

int main() {
    std::cout << "Earth 3D Model with Detailed Rendering\n";
    std::cout << "======================================\n\n";

    Simulation sim(1920, 1080, "Earth 3D Model");
    sim.setTimeStep(100);
    sim.physics->setTimeScale(10000000.0);

    std::cout << "Creating detailed Earth model...\n";

    sim.addSun();

    auto earth = std::make_shared<Body>(Vec3(Physics::AU, 0, 0),
                                        Vec3(0, 29780, 0), Physics::EARTH_MASS,
                                        Physics::EARTH_RADIUS, BodyType::PLANET);
    earth->setColor(0.2f, 0.5f, 1.0f);
    earth->name = "Earth";
    earth->renderScale = 1000;

    auto moon = std::make_shared<Body>(Vec3(Physics::AU + 3.844e8, 0, 0),
                                       Vec3(0, 29780 + 1022, 0), 7.342e22,
                                       1.737e6, BodyType::MOON);
    moon->setColor(0.7f, 0.7f, 0.7f);
    moon->name = "Moon";
    moon->renderScale = 500;

    sim.addEarth();
    sim.addBody(Vec3(Physics::AU + 3.844e8, 0, 0), Vec3(0, 29780 + 1022, 0),
                7.342e22, 1.737e6);

    std::cout << "Earth system created:\n";
    std::cout << "  - Sun (center)\n";
    std::cout << "  - Earth (1 AU)\n";
    std::cout << "  - Moon (384,400 km from Earth)\n\n";

    std::cout << "Rendering features:\n";
    std::cout << "  - High-resolution sphere (32x32 segments)\n";
    std::cout << "  - Smooth normals for realistic lighting\n";
    std::cout << "  - Color-coded planets\n";
    std::cout << "  - Glow effects\n\n";

    sim.useRK4();
    sim.enableGlow();
    sim.enableBloom();
    sim.setTimeStep(3600 * 6);

    std::cout << "=== Controls ===\n";
    std::cout << "Mouse  - Rotate camera\n";
    std::cout << "W/S    - Zoom\n";
    std::cout << "0      - Focus on Sun\n";
    std::cout << "1      - Follow Earth\n";
    std::cout << "2      - Follow Moon\n";
    std::cout << "SPACE  - Pause\n";
    std::cout << "Q      - Quit\n\n";

    std::cout << "Starting Earth simulation...\n\n";

    sim.run();

    return 0;
}
