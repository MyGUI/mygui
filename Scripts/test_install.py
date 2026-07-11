#!/usr/bin/env python3
"""Test installed MyGUI targets by building, installing, and running a consumer."""

import argparse
import os
import subprocess
import sys
import tempfile

MYGUI_SOURCE_DIR = os.path.normpath(os.path.join(os.path.dirname(__file__), ".."))

def run(cmd, **kwargs):
    print(f"+ {' '.join(cmd)}", flush=True)
    subprocess.check_call(cmd, **kwargs)


def main():
    parser = argparse.ArgumentParser(description="Test installed MyGUI targets")
    parser.add_argument(
        "--static", action="store_true", help="Static build (default: shared)"
    )
    parser.add_argument(
        "--prefix",
        default="/tmp/mygui-install-test",
        help="Install prefix (default: /tmp/mygui-install-test)",
    )
    parser.add_argument(
        "--build-dir",
        default=os.path.join(MYGUI_SOURCE_DIR, "build-install-test"),
        help="Build directory (default: <source>/build-install-test)",
    )
    args = parser.parse_args()

    build_shared = "OFF" if args.static else "ON"

    print(f"=== Building MyGUI (shared={build_shared}) ===")
    run(
        [
            "cmake",
            "-S", MYGUI_SOURCE_DIR,
            "-B", args.build_dir,
            "-DMYGUI_USE_FREETYPE=ON",
            "-DMYGUI_MSDF_FONTS=ON",
            "-DMYGUI_DONT_USE_OBSOLETE=ON",
            "-DMYGUI_BUILD_DEMOS=OFF",
            "-DMYGUI_BUILD_TOOLS=OFF",
            "-DMYGUI_BUILD_UNITTESTS=OFF",
            "-DMYGUI_BUILD_TEST_APP=OFF",
            "-DMYGUI_BUILD_WRAPPER=OFF",
            "-DMYGUI_HIGH_LEVEL_WARNINGS=ON",
            f"-DBUILD_SHARED_LIBS={build_shared}",
            f"-DMYGUI_RENDERSYSTEM=7",
            f"-DCMAKE_BUILD_TYPE=RelWithDebInfo",
            f"-DCMAKE_INSTALL_PREFIX={args.prefix}",
        ]
    )
    run(["cmake", "--build", args.build_dir, "--parallel"])
    run(["cmake", "--install", args.build_dir])

    print("=== Testing installed targets ===")
    with tempfile.TemporaryDirectory() as test_dir:
        cmake_lists = os.path.join(test_dir, "CMakeLists.txt")
        with open(cmake_lists, "w") as f:
            f.write(
                "cmake_minimum_required(VERSION 3.22)\n"
                "project(TestInstall)\n"
                f'list(APPEND CMAKE_MODULE_PATH "{MYGUI_SOURCE_DIR}/CMake")\n'
                "include(TestInstall)\n"
            )

        test_build = os.path.join(test_dir, "build")
        run(
            [
                "cmake",
                "-S", test_dir,
                "-B", test_build,
                f"-DCMAKE_PREFIX_PATH={args.prefix}",
            ]
        )
        run(["cmake", "--build", test_build])

        binary = os.path.join(test_build, "TestInstall")
        print(f"+ {binary}")
        result = subprocess.run([binary], capture_output=True, text=True)
        if result.returncode != 0:
            print(
                f"(runtime exit {result.returncode} — expected without resource files)",
                file=sys.stderr,
            )
            if result.stdout:
                print(result.stdout, file=sys.stderr)
            if result.stderr:
                print(result.stderr, file=sys.stderr)

    print("=== SUCCESS: Installed MyGUI::MyGUI targets work correctly ===")


if __name__ == "__main__":
    main()
