# PhySPP - Physics Simulation Engine 🌌

**Advanced 3D Physics Simulation Engine with Relativistic Effects**

*Open-source project by gruxelolo-dev*

## 🚀 Features

- **Advanced Physics Integrators**: RK4, Verlet, Euler
- **Relativistic Effects**: Einstein corrections for extreme gravity
- **Collision Detection**: Realistic body merging and absorption
- **Visual Effects**: Glow, bloom, lens flare
- **Interactive Camera**: Full 3D navigation and object tracking
- **Real-time Statistics**: Energy, center of mass, body count
- **Multiple Scenarios**: Black holes, solar systems, particle accretion

## 📦 Installation

### Prerequisites

```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake libsdl2-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev

# Arch Linux
sudo pacman -S base-devel cmake sdl2 mesa glu freeglut

# Fedora
sudo dnf install gcc-c++ cmake SDL2-devel mesa-libGL-devel mesa-libGLU-devel freeglut-devel

```
### Build

```bash
git clone https://github.com/yourusername/PhySPP.git
cd PhySPP
mkdir build && cd build
cmake ..
make -j$(nproc)
```

SPACE	Pause/Resume simulation

Q	Quit program

M	Toggle slow motion (1/5 speed)

W / S	Zoom in/out

Mouse	Rotate camera

← → ↑ ↓	Rotate camera (arrows)

0-9	Follow specific object

F	Toggle auto-follow (center of mass)

O	Toggle orbits

G	Toggle grid

