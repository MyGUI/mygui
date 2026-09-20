* PNG files use RGBA8 with no colour-profile chunks.
* TransparentRgb.png: top row (0,0,0,255), (255,255,255,0), (173,49,231,128), (0,0,0,255); bottom row (10,20,30,255),
  (40,50,60,0), (70,80,90,64), (100,110,120,255).
* Known.txt contains a NUL and 0xff to exercise exact binary stream reads. Mask.png tests the existing all-bytes-white
  picking rule, not alpha-only picking.
* PlatformSwap_* shaders preserve the renderer's normal input/sampler convention,
  then swap red and blue. The shared shader test distinguishes custom output from
  default output and checks restoration. Vulkan's GLSL source is compiled to
  SPIR-V in the build directory by glslangValidator.
