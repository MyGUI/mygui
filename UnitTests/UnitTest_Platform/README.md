# Shared platform tests

`UnitTest_Platform` runs the same resource, rendering and lifecycle tests against
MyGUI's rendering backends. Each build tests the backend selected by
`MYGUI_RENDERSYSTEM`; use separate build directories to test different backends.
Building several libraries with `MYGUI_BUILD_RENDERSYSTEMS` does not run the
suite against all of them.

The selected backend's dependencies and a working graphics context/display are
required, even though test windows are hidden by default. Emscripten uses
WebGL 2 in headless Chromium, as described below.

Reuse an existing suitable build and its dependencies. Replace `<build-dir>`
and `<renderer-id>` below with the directory and backend to test:

```sh
cmake -S . -B <build-dir> -DMYGUI_RENDERSYSTEM=<renderer-id> \
  -DMYGUI_BUILD_UNITTESTS=ON -DMYGUI_BUILD_PLATFORM_TESTS=ON
cmake --build <build-dir> --target UnitTest_Platform --parallel
ctest --test-dir <build-dir> -R '^UnitTest_Platform_' --output-on-failure
```

For multi-configuration generators, add `--config <configuration>` to the build
command and `-C <configuration>` to CTest. CTest selects the executable and
working directory for that configuration.

CTest registers three groups: `UnitTest_Platform_resources`,
`UnitTest_Platform_rendering` and `UnitTest_Platform_lifecycle`. They have
`platform;graphics` labels, run serially within CTest, and each has a 120-second
timeout. `-L platform` also selects the backend-specific test suites when present;
build their targets as well before using that broader selection.

For individual cases, run the executable directly. Replace `<test-executable>`
with its path, normally under `<build-dir>/bin/` (or `bin/<configuration>/`
for a multi-configuration generator). The executable is named
`UnitTest_Platform`, with an `.exe` suffix on Windows:

```sh
<test-executable>
<test-executable> --group resources --list
<test-executable> --group rendering --case rtt-chain --artifacts <artifact-dir>
```

Without `--group`, all groups run. `--group` accepts `resources`, `rendering` or `lifecycle`.
`--case` is a substring filter; no matches is an error. `--visible` shows the
window for debugging. `--artifacts` selects the failure-image directory;
CTest sets a directory under the suite's build directory automatically.

## Emscripten browser tests

The same three groups run in a browser for `MYGUI_RENDERSYSTEM=8`. Core unit
tests still run separately under Node. The native EGL-specific GLES suite is
not built for Emscripten.

After inspecting and selecting an existing Emscripten build, install the browser
runner there and enable the shared suite:

```sh
python3 -m venv <build-dir>/browser-tests
<build-dir>/browser-tests/bin/python -m pip install -r Scripts/Tests/browser-requirements.txt
<build-dir>/browser-tests/bin/python -m playwright install --with-deps --only-shell chromium
emcmake cmake -S . -B <build-dir> \
  -DMYGUI_BUILD_UNITTESTS=ON -DMYGUI_BUILD_PLATFORM_TESTS=ON \
  -DPython3_EXECUTABLE="$PWD/<build-dir>/browser-tests/bin/python"
cmake --build <build-dir> --target UnitTest_Platform --parallel
ctest --test-dir <build-dir> -L platform --output-on-failure
```

The runner starts a localhost server and an isolated headless Chromium process
using SwiftShader software rendering. A canvas replaces the native window;
the fixture uses `--visible` and a device scale of one. Nonzero program exits,
JavaScript errors, aborts and timeouts fail the test. Logs, failure screenshots
and available PPM captures are saved under the suite's `artifacts/<group>`
directory. CI uploads that directory on failure.

To reuse an installed Chromium-based browser, omit the Playwright browser
download and set `MYGUI_TEST_BROWSER` to the browser executable when running
CTest. The pinned Playwright browser is the default used in CI.

## Coverage

The cases cover:

- Resource streams, texture creation/reloading, supported formats with Static,
  Dynamic and Stream usage, repeated uploads, PNG fidelity,
  filtering, mask picking, lock state and read/write preservation.
- Drawing, blending, clipping, resizing, buffer growth, queued and same-frame
  updates, resource lifetime, separate window/RTT shader selection and host-state isolation.
- Render-to-texture (RTT) output, orientation, persistence, clearing, chaining,
  nested targets, CPU edits, target reuse and destruction.
- Rendering with fresh and replacement GUI/platform fixtures.

Cases use public MyGUI interfaces through `base::BaseManager`. Backend setup,
frame capture and host-state probes live in `Common/Base/`. Resources and
rendering each share a fixture within their group; lifecycle cases create a
new fixture per case. Every fixture starts with a separately reported
initialization capture. Case cleanup clears consumers, captures a frame, and
releases resources before the next case.

Queued-update bursts reuse vertex buffers without texture uploads or intermediate
captures. The RTT variant retains each frame's output and checks all of them in
one final capture. These are public-interface checks; they do not force or inspect
an unsignalled native GPU fence.

CPU byte comparisons are exact. Captures use top-down RGBA8; pixel assertions
compare RGB with a tolerance of one byte per channel and test alpha through
compositing. Colour-texture uploads use MyGUI's BGR/BGRA byte order. Failures
save PPM images when capture data is available. Capture completion is limited
to eight rendered frames. Recoverable failures mark the group as failed and
allow later cases to run; fatal capture or cleanup errors end the invocation.

Optional format/usage combinations skip only when the renderer does not
advertise them. Advertised operations that fail are test failures. Shader
fixtures, scene-depth probes and hostile-state hooks have separate availability
checks; a missing fixture or probe is not a renderer capability claim. Shader
source selection is in `ShaderSources.cpp`; test media and Vulkan shader
regeneration instructions are in the [media README](../../Media/UnitTests/UnitTest_Platform/README.md).

Backend-specific suites supplement these public-interface checks with native
state restoration, allocation reuse, resource retirement and other integration
behavior. The OpenGL, OpenGL3, OpenGLES and Vulkan native executables report
named cases with a pass/fail summary.
They stop after the first failure because native state probes may leave their
graphics context unusable. OpenGL, OpenGL3 and Vulkan save available failure
captures under `platform-artifacts/native-<backend>/` in the working directory.
The standalone GLES fixture does not provide frame-capture artifacts.
See their READMEs for setup requirements, including
[native GLES/ANGLE](../UnitTest_OpenGLESPlatform/README.md). The native-only
probes do not establish browser behavior. This suite does not measure performance.
Arbitrary device-loss and font-atlas recovery are outside its coverage; resize
exercises the selected backend's normal window-resize path.
