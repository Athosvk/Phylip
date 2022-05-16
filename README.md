# Phylip
Cloth simulation using [Fast Simulation of Mass Spring System](https://www.cs.utah.edu/~ladislav/liu13fast/liu13fast.html) by Liu et Al.

### Wind on the +Y axis, falling and wrapping on a sphere
![](https://media.giphy.com/media/0Nx6Ea6XJ8PzfODchu/giphy.gif) ![](https://media.giphy.com/media/yu3J8kNGY0Icyfs9sA/giphy.gif)

### Shooting spheres at hanging cloth and showing the effect of some options
![](https://media.giphy.com/media/ZK7LztV355DsubZDyO/giphy.gif)


# Dependencies
- Raylib and RayGui are used for rendering and GUI;
- Eigen is used for all the linear algebra black magic;
- yaml-cpp is needed to parse scene and configuration files;

As already mentioned, this project uses raylib to handle the rendering. 
We had to modify some raylib functions to fit it to our needs, therefore to be able to build the project correctly, follow the next steps:

```
git clone --recursive https://github.com/Athosvk/Phylip
cd external/raylib/
git checkout 7584ce6f
git apply ../../raylib.diff
```

# Build Instructions:

Be sure you are in the root of the project (same directory were this README.md is located):

```
mkdir Release
cd Release
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j8
```

# Run Instructions  

```
./Phylip resources/config_flat.yaml
./Phylip resources/config_vertical.yaml
```
