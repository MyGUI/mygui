#!/usr/bin/env python3
"""Prepare browser demos; optionally copy and commit them to ../mygui.info/demos/."""

# Build and preview (from the repository root; defaults to MYGUI_RENDERSYSTEM=8):
# source ~/libs/emsdk/emsdk_env.sh
# python3 Scripts/publish_web_demos.py build-web --build
# python3 -m http.server 8000 --bind 127.0.0.1 --directory build-web/website-combined
# Open http://localhost:8000/
# Copy to ../mygui.info/demos/ and commit (without pushing):
# python3 Scripts/publish_web_demos.py build-web --build --commit

# Ogre 14.6 SDK (web defaults; skip optional downloads, ImGui and Ogre samples):
# emcmake cmake -S /path/to/ogre -B build-web/ogre -DCMAKE_BUILD_TYPE=Release \
#   -DCMAKE_INSTALL_PREFIX=/path/to/ogre-web -DOGRE_BUILD_DEPENDENCIES=OFF \
#   -DOGRE_BUILD_COMPONENT_OVERLAY_IMGUI=OFF -DOGRE_BUILD_SAMPLES=OFF
# cmake --build build-web/ogre --parallel
# cmake --install build-web/ogre
# python3 Scripts/publish_web_demos.py build-web-ogre --build --ogre-dir /path/to/ogre-web/lib/OGRE/cmake

import argparse
from pathlib import Path
import shutil
import subprocess


def build_demos(repository, build, ogre_dir=None):
    cache = build / "CMakeCache.txt"
    settings = {}
    if cache.exists():
        for line in cache.read_text().splitlines():
            if not line.startswith(("//", "#")) and ":" in line and "=" in line:
                key, value = line.split("=", 1)
                settings[key.split(":", 1)[0]] = value
    if cache.exists() and settings.get("EMSCRIPTEN") != "1":
        raise ValueError(f"{build} is not an Emscripten build; choose a separate build directory")
    if shutil.which("emcmake") is None:
        raise ValueError("Activate Emscripten first: source ~/libs/emsdk/emsdk_env.sh")

    renderer = "3" if ogre_dir else settings.get("MYGUI_RENDERSYSTEM", "8")
    if renderer not in ("3", "8"):
        raise ValueError(f"Unsupported browser render system: {renderer}")
    if ogre_dir and not (ogre_dir / "OGREConfig.cmake").is_file():
        raise ValueError(f"Missing Ogre WebAssembly SDK configuration: {ogre_dir / 'OGREConfig.cmake'}")

    configure = [
        "emcmake", "cmake", "-S", str(repository), "-B", str(build),
        f"-DMYGUI_RENDERSYSTEM={renderer}", "-DMYGUI_BUILD_WEB_DEMOS=ON",
        "-DMYGUI_BUILD_DEMOS=ON", "-DMYGUI_BUILD_ADVANCED_DEMOS=ON",
        "-DMYGUI_USE_FREETYPE=ON", "-DMYGUI_MSDF_FONTS=ON",
    ]
    if not cache.exists():
        configure.append("-DCMAKE_BUILD_TYPE=Release")
    if ogre_dir:
        configure.append(f"-DOGRE_DIR={ogre_dir}")
    subprocess.run(configure, check=True)
    subprocess.run(["cmake", "--build", str(build), "--target", "MyGUI_Demos", "--parallel"], check=True)


def commit_demos(site, website, names):
    if not website.is_dir():
        raise ValueError(f"Missing website checkout: {website}")
    root = subprocess.run(
        ["git", "-C", str(website), "rev-parse", "--show-toplevel"],
        check=True, capture_output=True, text=True,
    ).stdout.strip()
    if Path(root).resolve() != website.resolve():
        raise ValueError(f"Expected a Git checkout rooted at {website}")

    destination = website / "demos"
    destination.mkdir(exist_ok=True)
    for name in names:
        shutil.copy2(site / name, destination / name)

    paths = [f"demos/{name}" for name in names]
    git = ["git", "-C", str(website)]
    subprocess.run([*git, "add", "--", *paths], check=True)
    changes = subprocess.run([*git, "diff", "--cached", "--quiet", "--", *paths])
    if changes.returncode == 0:
        print("Website demos are already up to date; nothing to commit.")
        return
    if changes.returncode != 1:
        changes.check_returncode()
    # A path-limited commit leaves unrelated staged changes out of this commit.
    subprocess.run([*git, "commit", "--only", "-m", "Update browser demos", "--", *paths], check=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("build_dir", type=Path)
    parser.add_argument("--build", action="store_true", help="Configure with Emscripten and build MyGUI_Demos first")
    parser.add_argument("--ogre-dir", type=Path,
                        help="Use an Ogre WebAssembly SDK (directory containing OGREConfig.cmake)")
    parser.add_argument("--commit", action="store_true", help="Copy to ../mygui.info/demos/ and commit those files; do not push")
    args = parser.parse_args()
    if args.ogre_dir and not args.build:
        parser.error("--ogre-dir requires --build")
    build = args.build_dir.resolve()
    if args.build:
        try:
            build_demos(Path(__file__).resolve().parents[1], build, args.ogre_dir.resolve() if args.ogre_dir else None)
        except (OSError, ValueError, subprocess.CalledProcessError) as error:
            parser.exit(1, f"Unable to build browser demos: {error}\n")
    binaries = build / "bin"
    inputs = [binaries / ("MyGUI_Demos" + suffix) for suffix in [".html", ".js", ".wasm", ".data"]]
    for path in inputs:
        if not path.is_file():
            parser.error(f"Missing {path}; build target MyGUI_Demos first")

    site = build / "website-combined"
    site.mkdir(exist_ok=True)
    for source in inputs:
        target = site / ("index.html" if source.suffix == ".html" else source.name)
        shutil.copy2(source, target)

    print(site)
    if args.commit:
        website = Path(__file__).resolve().parents[2] / "mygui.info"
        names = ["index.html", "MyGUI_Demos.js", "MyGUI_Demos.wasm", "MyGUI_Demos.data"]
        try:
            commit_demos(site, website, names)
        except (OSError, ValueError, subprocess.CalledProcessError) as error:
            parser.exit(1, f"Unable to commit website demos: {error}\n")


if __name__ == "__main__":
    main()
