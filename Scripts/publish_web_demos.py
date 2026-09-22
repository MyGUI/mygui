#!/usr/bin/env python3
"""Prepare browser demos; optionally copy and commit them to ../mygui.info/demos/."""

# Build and preview (from the repository root):
# source ~/libs/emsdk/emsdk_env.sh
# emcmake cmake -S . -B build-web -DCMAKE_BUILD_TYPE=Release -DMYGUI_RENDERSYSTEM=8 -DMYGUI_BUILD_WEB_DEMOS=ON
# cmake --build build-web --target MyGUI_Demos --parallel
# python3 Scripts/publish_web_demos.py build-web
# python3 -m http.server 8000 --bind 127.0.0.1 --directory build-web/website-combined
# Open http://localhost:8000/
# Copy to ../mygui.info/demos/ and commit (without pushing):
# python3 Scripts/publish_web_demos.py build-web --commit

import argparse
from pathlib import Path
import shutil
import subprocess


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
    parser.add_argument("--commit", action="store_true", help="Copy to ../mygui.info/demos/ and commit those files; do not push")
    args = parser.parse_args()
    build = args.build_dir.resolve()
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
