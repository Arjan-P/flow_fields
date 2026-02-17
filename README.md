# Flow field using perlin noise function

![flow_field](./gifs/output.gif)

using compute shaders

- For each particle find output of noise function
- Use noise output to calculate a direction vector
- move particle along direction vector

Build:

> from project root:

```
  mkdir build && cd build
  cmake ..
  cmake --build .
  cp -r ../src/shaders .
```
