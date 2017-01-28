# skunkwork
A lightweight OpenGL renderer with naive hardcoded shader program

Loosely based on [OGLdev-tutorials](ogldev.atspace.co.uk) with marching cubes implementation built upon Paul Bourke's [paper](http://paulbourke.net/geometry/polygonise/).

Model to be loaded is currently harcoded in scene.cpp as is it's scale. Interaction includes a virtual trackball -implementation and moving the model with arrows and shift - up/down.

Layer file parser is a marching cubes implementation which takes numbered (\*.1 - \*.n) layer files with a header in the same folder. Header is constructed as follows:

```
name        MRbrain
filename    MRbrain.
treshold    1400
resX        256
resY        256
layers      109
aspX        1
aspY        1
aspZ        2
```

^^This is used to parse MRbrain from https://graphics.stanford.edu/data/voldata/
