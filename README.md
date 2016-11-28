# skunkwork
A lightweight OpenGL renderer

LayerFile parser is a marching cubes implementation which takes numbered (*.1 - *.n)
layer files with a header in the same folder. Header is constructed as follows:

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
