#!/usr/bin/env python3
"""Build, run all unit tests, and report MyGUIEngine coverage.

Requires CMake 3.22+, Clang, llvm-cov, and gcovr (pip install gcovr) on macOS or Linux.
Uses a headless Debug build without FreeType/MSDF. Prints a summary and writes
HTML to build-cov/coverage/index.html, sorted by uncovered line count (largest first).
Reads the root gcovr.cfg to merge source-line coverage across template instantiations.
"""

import argparse
import os
from pathlib import Path
import shlex
import subprocess
import sys

SOURCE_DIR = Path(__file__).resolve().parent.parent


def run(command, **kwargs):
    print("+ " + shlex.join(map(str, command)), flush=True)
    return subprocess.run(command, cwd=SOURCE_DIR, check=True, text=True, **kwargs)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", type=Path, default=SOURCE_DIR / "build-cov")
    parser.add_argument("--jobs", type=int, default=os.cpu_count() or 1)
    args = parser.parse_args()
    build = args.build_dir.resolve()
    if sys.platform not in ("darwin", "linux") or args.jobs < 1 or build == SOURCE_DIR:
        parser.error("Use macOS/Linux, a positive --jobs value, and an out-of-source build directory")

    def llvm(name):
        if sys.platform == "darwin":
            return run(["xcrun", "--find", name], capture_output=True).stdout.strip()
        return name

    run(["gcovr", "--version"])
    clang, clangxx, cov = map(llvm, ("clang", "clang++", "llvm-cov"))
    configure = [
        "cmake", "--preset", "minimal", "-B", build,
        "-DCMAKE_BUILD_TYPE=Debug", f"-DCMAKE_C_COMPILER={clang}", f"-DCMAKE_CXX_COMPILER={clangxx}",
        "-DCMAKE_C_FLAGS=--coverage", "-DCMAKE_CXX_FLAGS=--coverage",
        "-DCMAKE_EXE_LINKER_FLAGS=--coverage", "-DCMAKE_SHARED_LINKER_FLAGS=--coverage",
        "-DBUILD_SHARED_LIBS=ON", "-DMYGUI_BUILD_UNITTESTS=ON", "-DMYGUI_RENDERSYSTEM=1",
    ]
    run(configure)
    run(["cmake", "--build", build, "--config", "Debug", "--parallel", str(args.jobs)])

    # Reset counters so earlier test runs cannot inflate coverage.
    for profile in build.rglob("*.gcda"):
        profile.unlink()
    run(["ctest", "--test-dir", build, "-C", "Debug", "--output-on-failure", "--no-tests=error"])

    output = build / "coverage"
    output.mkdir(exist_ok=True)
    run(["gcovr", "--root", SOURCE_DIR, "--filter", "MyGUIEngine/",
         "--gcov-executable", shlex.join([cov, "gcov"]), "--print-summary",
         "--sort", "uncovered-number", "--sort-reverse",
         "--html-details", str(output / "index.html"), str(build)])
    print(f"HTML report: {(output / 'index.html').as_uri()}")


if __name__ == "__main__":
    try:
        main()
    except (OSError, RuntimeError, subprocess.CalledProcessError) as error:
        sys.exit(f"Coverage failed: {error}")
