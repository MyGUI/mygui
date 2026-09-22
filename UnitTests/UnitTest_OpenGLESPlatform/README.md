# OpenGL ES platform tests

Configure a native ES 3 build with `MYGUI_RENDERSYSTEM=8`,
`MYGUI_BUILD_UNITTESTS=ON` and `MYGUI_BUILD_PLATFORM_TESTS=ON`.
The target requires GLES 3 and EGL development headers/libraries. Nonstandard
installations can set `MYGUI_GLES_INCLUDE_DIR`, `MYGUI_GLES_LIBRARY`,
`MYGUI_EGL_INCLUDE_DIR` and `MYGUI_EGL_LIBRARY` in CMake.

Run `ctest --test-dir <build> -R UnitTest_OpenGLESPlatform --output-on-failure`.
The test creates an EGL pbuffer. Set `MYGUI_TEST_EGL_PLATFORM=angle-metal` to use
an ANGLE installation's Metal renderer on macOS. Supply the library search path
using the normal platform loader configuration; Firefox is not a project dependency.

These are native backend regression checks: hostile pixel-transfer state and PBO
contents, lock ordering and cleanup, initial image uploads, shader replacement,
per-program RTT orientation, separate framebuffer bindings, nested targets,
callback failure and raster/object-state restoration. Pixel checks use actual
GPU output. The image loader is a test double; PNG encoding is not covered here.

The shared SDL `UnitTest_Platform` suite is a separate target. With an ANGLE
installation, set `ANGLE_DEFAULT_PLATFORM=metal`, `SDL_VIDEO_EGL_DRIVER` to its
EGL library and `SDL_VIDEO_GL_DRIVER` to its GLES library before running
`ctest --test-dir <build> -R '^UnitTest_Platform_' --output-on-failure`.
Browser/WebGL execution is not part of these native targets. Run with address/undefined-behaviour sanitizers to check allocation
and outstanding-lock cleanup as well.

The Firefox ANGLE distribution used for local testing conflicts with macOS
AddressSanitizer during libmozglue allocator initialization, before main. Normal
and UndefinedBehaviorSanitizer runs pass; use another ANGLE/EGL distribution for
AddressSanitizer coverage.
