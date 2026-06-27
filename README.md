![MyGUI logo](http://mygui.info/images/MyGUI_Logo.png)

[![CI](https://github.com/MyGUI/mygui/actions/workflows/build.yml/badge.svg)](https://github.com/MyGUI/mygui/actions)

MyGUI is a cross-platform library for creating graphical user interfaces (GUIs) for games and 3D applications. It is
written in C++17 and is designed to be fast, flexible, and easy to integrate with various rendering engines.

## Features

- **Wide widget set:** buttons, edit boxes, combo boxes, lists, tree controls, item boxes, tab controls, scroll bars,
  progress bars, windows, and more
- **Multiple render backends:** Direct3D 9, Direct3D 11, OpenGL (legacy), OpenGL 3.x, OpenGL ES 2.0, and Ogre
- **Skin system:** fully customizable widget appearance via XML-based skin definitions
- **Layout system:** load UI layouts from XML files at runtime; includes a visual **Layout Editor**
- **Tools:** built-in editors for layouts, skins, images, and fonts
- **Font rendering:** TrueType fonts via FreeType, bitmap fonts, and MSDF (multi-channel signed distance field) font
  support
- **Platform support:** Windows, Linux, macOS; also runs in the browser via Emscripten
- **Plugin system:** runtime-loadable plugins to extend functionality
- **Localization:** built-in language string management
- **Animation:** configurable widget controllers for fades, slides, and more

## Rendering Backends

| Backend                    | CMake ID | 
|----------------------------|----------|
| Dummy (no-op)              | 1        |
| Ogre                       | 3        |
| OpenGL (legacy)            | 4        |
| Direct3D 9                 | 5        |
| Direct3D 11                | 6        |
| OpenGL 3.x                 | 7        |
| OpenGL ES 2.0 (Emscripten) | 8        |

Select a backend with the `-DMYGUI_RENDERSYSTEM=<id>` CMake option.

## Build

### Requirements

- CMake ≥ 3.25
- C++17 compiler
- SDL2 for tools and demos
- FreeType (optional, enabled by default)
- One of the render systems

### Quick start

```bash
git clone https://github.com/MyGUI/mygui.git
cd mygui
cmake -S . -B build -DMYGUI_RENDERSYSTEM=7
cmake --build build
```

### Usage in your project

After installing MyGUI, use it from your CMake project with modern targets:

```cmake
find_package(MyGUI REQUIRED)
target_link_libraries(myapp PRIVATE MyGUI::MyGUI MyGUI::OgrePlatform)
```

Available targets:

| Target                    | Description         |
|---------------------------|---------------------|
| `MyGUI::MyGUI`            | Core library        |
| `MyGUI::OgrePlatform`     | Ogre backend        |
| `MyGUI::OpenGLPlatform`   | OpenGL backend      |
| `MyGUI::OpenGL3Platform`  | OpenGL 3.x backend  |
| `MyGUI::OpenGLES`         | OpenGLES backend    |
| `MyGUI::DirectXPlatform`  | Direct3D 9 backend  |
| `MyGUI::DirectX11Platform`| Direct3D 11 backend |

Build options:

| Option                  | Default        | Description                          |
|-------------------------|----------------|--------------------------------------|
| `MYGUI_RENDERSYSTEM`    | 7 (OpenGL 3.x) | Render backend (1-8)                 |
| `MYGUI_BUILD_DEMOS`     | ON             | Build demo applications              |
| `MYGUI_BUILD_TOOLS`     | ON             | Build layout/skin/font/image editors |
| `MYGUI_BUILD_UNITTESTS` | OFF            | Build unit tests                     |
| `MYGUI_USE_FREETYPE`    | ON             | Enable FreeType font rendering       |
| `MYGUI_MSDF_FONTS`      | OFF            | Enable MSDF font support             |
| `MYGUI_DISABLE_PLUGINS` | OFF            | Disable plugin system                |

## Tools

MyGUI ships with several visual editors under `Tools/`:

- **LayoutEditor** — WYSIWYG layout designer (produces `.layout` XML files)
- **SkinEditor** — widget skin creator
- **ImageEditor** — image resource editor
- **FontEditor** — font resource editor

## Documentation

- Website: http://mygui.info/
- API docs: build with `cmake --build build --target api-docs` (requires Doxygen)

## License

This project is licensed under the MIT License — see [COPYING.MIT](COPYING.MIT) for details.

## Links

- Website: http://mygui.info/
- Repository: https://github.com/MyGUI/mygui
- CI: https://github.com/MyGUI/mygui/actions
