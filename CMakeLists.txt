cmake_minimum_required(VERSION 3.10)
project(PhySPP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}
)

add_library(PhySPP INTERFACE)
target_include_directories(PhySPP INTERFACE ${CMAKE_SOURCE_DIR})

add_executable(simple_example examples/simple_example.cpp)
target_link_libraries(simple_example PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(blackhole_example examples/blackhole_example.cpp)
target_link_libraries(blackhole_example PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(full_demo_v2 examples/full_demo.cpp)
target_link_libraries(full_demo_v2 PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(custom_physics examples/custom_physics.cpp)
target_link_libraries(custom_physics PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(blackhole_accretion examples/blackhole_accretion.cpp)
target_link_libraries(blackhole_accretion PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(milky_way examples/milky_way.cpp)
target_link_libraries(milky_way PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(planet_collision examples/planet_collision.cpp)
target_link_libraries(planet_collision PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(four_particle_collision examples/four_particle_collision.cpp)
target_link_libraries(four_particle_collision PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(bouncing_ball examples/bouncing_ball.cpp)
target_link_libraries(bouncing_ball PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(solar_system_complete examples/solar_system_complete.cpp)
target_link_libraries(solar_system_complete PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(collision_camera_tracking examples/collision_camera_tracking.cpp)
target_link_libraries(collision_camera_tracking PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(earth_3d_model examples/earth_3d_model.cpp)
target_link_libraries(earth_3d_model PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(advanced_blackhole examples/advanced_blackhole.cpp)
target_link_libraries(advanced_blackhole PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(neutron_collision examples/neutron_collision.cpp)
target_link_libraries(neutron_collision PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(binary_star_system examples/binary_star_system.cpp)
target_link_libraries(binary_star_system PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(asteroid_belt examples/asteroid_belt.cpp)
target_link_libraries(asteroid_belt PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(galaxy_collision examples/galaxy_collision.cpp)
target_link_libraries(galaxy_collision PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(moon_orbit examples/moon_orbit.cpp)
target_link_libraries(moon_orbit PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(supernova_explosion examples/supernova_explosion.cpp)
target_link_libraries(supernova_explosion PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(collision_demo examples/collision_demo.cpp)
target_link_libraries(collision_demo PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(performance_test examples/performance_test.cpp)
target_link_libraries(performance_test PhySPP)

add_executable(advanced_engine_demo examples/advanced_engine_demo.cpp)
target_link_libraries(advanced_engine_demo PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(realistic_solar_system examples/realistic_solar_system.cpp)
target_link_libraries(realistic_solar_system PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(collision_physics_demo examples/collision_physics_demo.cpp)
target_link_libraries(collision_physics_demo PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(deterministic_physics_demo examples/deterministic_physics_demo.cpp)
target_link_libraries(deterministic_physics_demo PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(nuclear_physics_demo examples/nuclear_physics_demo.cpp)
target_link_libraries(nuclear_physics_demo PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(stellar_fusion examples/stellar_fusion.cpp)
target_link_libraries(stellar_fusion PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

add_executable(big_bang examples/big_bang.cpp)
target_link_libraries(big_bang PhySPP ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

if(EXISTS ${CMAKE_SOURCE_DIR}/simple_demo.cpp)
    add_executable(simple_demo simple_demo.cpp physpp.cpp)
    target_link_libraries(simple_demo ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} GLU)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/advanced_demo.cpp)
    add_executable(advanced_demo advanced_demo.cpp physpp.cpp)
    target_link_libraries(advanced_demo ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} GLU)
endif()

install(DIRECTORY core physics graphics api DESTINATION include/PhySPP)
install(FILES PhySPP.h DESTINATION include)
