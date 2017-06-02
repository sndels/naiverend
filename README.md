# naiverend 
A lightweight OpenGL renderer with naive hardcoded shader program

Loosely based on [OGLdev-tutorials](http://ogldev.atspace.co.uk) with marching cubes implementation built upon Paul Bourke's [paper](http://paulbourke.net/geometry/polygonise/).

Cmake build should work^tm on windows (vs2015) and osx (clang) if GLFW3 is installed. `git submodule init` and `git submodule update` are needed to pull imgui.

Model to be loaded is currently hardcoded in scene.cpp as is it's scale and orientation. Interaction includes an arcball-implementation with mouse wheel controlling the distance plus arrows and shift - up/down controlling the model's position.

Layer file parser is a marching cubes implementation which takes numbered (\*.1 - \*.n) layer files with a header in the same folder. Header is constructed as follows:

```
name        MRbrain
filename    MRbrain.
threshold   1400
resX        256
resY        256
layers      109
aspX        1
aspY        1
aspZ        2
```

^^This is used to parse MRbrain from https://graphics.stanford.edu/data/voldata/
