
# Description

console3D is a simple console-based 3D renderer written in C++. It renders 3D objects (triangles or meshes from .obj files) in the terminal using an ASCII gradient for shading. The project supports basic camera movement and rotation in 3D space, simulating a ray-tracing-like effect for real-time interaction. It uses external libraries for math operations and console handling.

Key features:

* Ray-triangle intersection for rendering.
* Lighting with point light sources.
* Keyboard controls for movement (W/A/S/D for forward/left/back/right, Q/E for up/down) and rotation (arrows for pitch/yaw, R/F for roll).

This project is designed for Windows (due to Windows.h dependencies) and demonstrates basic 3D graphics principles without graphical libraries.

# Installation and Build

1. Clone the repository:
```bash
git clone https://github.com/w1vern/console3D
cd console3D
```
2. Initialize and update submodules:
```bash
make install_submodules
```
3. Build:
```bash
make
```

# Running the Application

* Navigate to the build directory.
* Run the executable: console3D.exe (or ./console3D if on a Unix-like system with adjustments).
* The console will render the 3D scene. Use keyboard controls to navigate.
