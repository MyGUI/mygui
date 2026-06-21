#!/usr/bin/env python3
"""Runs clang-format on all C++ source files in the project.

Usage:
  ./run-clang-format.py          # check-only (dry run, exit 1 on mismatch)
  ./run-clang-format.py --fix    # format files in-place
"""

import argparse
import os
import shutil
import subprocess
import sys

SRC_DIRS = [
    "MyGUIEngine",
    "Platforms",
    "Demos",
    "Tools",
    "Common",
    "UnitTests",
    "Plugins",
    #"Wrappers",
]

EXTENSIONS = (".cpp", ".h")

EXCLUDE_PREFIXES = (
    "MyGUIEngine/src/msdfgen/",
)

EXCLUDE_FILENAMES = (
)


def find_sources():
    sources = []
    for d in SRC_DIRS:
        for root, _, files in os.walk(d):
            for f in files:
                if f.endswith(EXTENSIONS):
                    full = os.path.join(root, f)
                    if any(full.startswith(p) for p in EXCLUDE_PREFIXES):
                        continue
                    if f in EXCLUDE_FILENAMES:
                        continue
                    sources.append(full)
    return sorted(sources)


def main():
    parser = argparse.ArgumentParser(description="Run clang-format on all source files")
    parser.add_argument(
        "--fix",
        action="store_true",
        help="Format files in-place instead of just checking",
    )
    args = parser.parse_args()

    mode = "-i" if args.fix else "--dry-run --Werror"
    sources = find_sources()

    if not sources:
        print("No source files found.")
        sys.exit(0)

    # force using clang-format-20 if available
    CLANG_FORMAT = "clang-format-20" if shutil.which("clang-format-20") else "clang-format"
    subprocess.run([CLANG_FORMAT, "--version"])
    cmd = [CLANG_FORMAT, mode] if args.fix else [CLANG_FORMAT, "--dry-run", "--Werror"]
    cmd.extend(sources)

    result = subprocess.run(cmd)
    if result.returncode != 0:
        print("Formatting issues found. Run `Scripts/run-clang-format.py --fix` to auto-format.")
    sys.exit(result.returncode)


if __name__ == "__main__":
    main()
