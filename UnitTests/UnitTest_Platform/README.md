# Native platform contract tests

Enable `MYGUI_BUILD_PLATFORM_TESTS=ON` together with `MYGUI_BUILD_UNITTESTS=ON`
and select a native `MYGUI_RENDERSYSTEM`. Demos/tools may remain disabled. See
[the implementation plan](../../Docs/PlatformTestPlan.md) for scope, current
failures, and the validation matrix.

```sh
ctest --test-dir build-platform-osg -L platform --output-on-failure
build-platform-osg/bin/UnitTest_Platform --list
build-platform-osg/bin/UnitTest_Platform --group rendering --case rtt-chain \
  --artifacts build-platform-osg/chain-artifacts
```

`--case` is a substring filter; no matching cases is an error. `--visible`
overrides hidden-window creation for debugging. Each resource/rendering group
shares one context; lifecycle cases explicitly recreate fixtures. Every context
has a separately reported initialization capture, so a recoverable startup API
error does not masquerade as a pixel-case failure. The group still fails.
Context/device loss, out-of-memory, timeout and unrecoverable cleanup errors stop
the invocation. CTest groups are serialized and time out after 120 seconds.

Lifecycle cases check rendering with successive, successfully initialized
fixtures. Application startup failure injection and recovery are outside this
suite's scope. Each created fixture is closed once; incomplete startup ends the
invocation without attempting partial cleanup.

The fixture uses `base::BaseManager` and public MyGUI interfaces. A custom layer
issues geometry/RTT work inside the normal GUI render callback. No test creates
DummyPlatform or calls graphics APIs. Native readback and hostile raster-state
setup live in `Common/Base/`. Vulkan binds a pipeline with rasterizer discard
before GUI rendering, checking that MyGUI binds its own pipeline. The render
pass owner still supplies viewport/scissor, as required by the Vulkan renderer.
Ogre cameras supply wireframe state, and the OpenGLES hook uses culling without
wireframe. OpenGLES/Emscripten is currently excluded by the native suite's build
configuration. Capture has a maximum eight-frame completion bound;
the queued-update case also checks that synchronous capture does not introduce
extra rendered frames.

Test media uses the shared `unittest::getResourcePath` helper. The Vulkan test
shader binary is committed alongside its source in the test media directory.

Portable coverage extracted from the OSG suite includes duplicate-name rejection
without replacing the original texture, rejection of `PixelFormat::Unknow`, and
RTT destruction/recreation before its first frame. Two separate same-frame tests
reuse a vertex buffer or texture between left/right draws and verify both draws;
they complement the uncaptured multi-frame update burst.

Transfer regressions cover odd-width RGB readback and partial read/write updates,
editing file-loaded textures, and preserving GPU-produced RTT pixels when
read/write access is advertised. OpenGL-specific PBO and pixel-store state checks
live in `UnitTest_OpenGLPlatform` and `UnitTest_OpenGL3Platform` and use the same fixture.
Nested RTT coverage verifies that drawing resumes in the outer target after
an inner target ends, with both targets retaining their expected pixels.

`shader-selection` uses a fragment shader that swaps red and blue. It verifies
custom output, a subsequent draw with another texture's default shader, and
switching the original texture back to `Default`. Source selection is isolated
in `ShaderSources.cpp`; assertions remain shared. OpenGL/DirectX9 have no shader
implementation and skip. Ogre/OgreNext use GLSL or HLSL fixtures; other shader
languages currently skip. Vulkan loads the committed SPIR-V fixture, so building
the suite does not require a shader compiler. After editing Vulkan shader sources,
run `python3 Scripts/update_spv.py` to regenerate production, demo, and test binaries
and commit the updated `.spv` files. The script finds `glslangValidator` or `glslang`
on PATH or in `$VULKAN_SDK/bin`; use `--compiler` to select an explicit executable.

`scene-depth-preservation` first verifies an empty frame's depth value, then
checks that an opaque GUI quad renders without changing that value. Five depth
samples are captured before presentation. OpenGL, OpenGL3 and OSG provide the
native probe; other harnesses report it unavailable and skip this case. This
checks observable scene depth, not a particular backend's depth-state objects.

Captures are owned top-down RGBA8. Uploaded/locked MyGUI colour textures use
BGR(A) bytes; vertex colours respect `getVertexFormat()`. PNG bytes and expected
pixels are independent fixture constants. Pixel comparisons allow one byte per
RGB channel, use interior sample points, and test alpha through compositing.
Failures save PPM images when readback is available. Optional capabilities may
skip only when not advertised; advertised-but-broken behavior fails normally.
Native depth-probe and shader-fixture availability are reported separately from
renderer format/usage capabilities; missing harness support is not a renderer
capability claim.

Cleanup first removes consumers and captures an empty frame, then destroys
case-owned buffers/textures. It restores the fixed window size even after a
resize assertion fails. This synchronization is deliberately outside the
uncaptured update burst. A final image cannot prove every earlier queued frame
was correct; use native validation layers to supplement the suite.

There are no reference-image updates, expected-failure exemptions, timing
thresholds, or backend workarounds. Keep known renderer failures visible until a
separately authorized renderer fix is implemented.
