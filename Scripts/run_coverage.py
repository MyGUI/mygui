#!/usr/bin/env python3
"""Build, run all unit tests, and report MyGUIEngine coverage.

Requires CMake 3.22+, Clang, llvm-cov, and gcovr (pip install gcovr) on macOS or Linux.
Uses a headless Debug build without FreeType/MSDF. Prints a summary and writes
HTML to build-cov/coverage/index.html, sorted by uncovered line count (largest first).
By default, merges line coverage across template instantiations and counts functions by source definition line.
Functions sharing a definition line count together, including compiler-generated functions.
Use --no-merge to report each template instantiation separately.
Preserves raw per-instantiation coverage in coverage/raw.json.
"""

import argparse
import json
import os
from pathlib import Path
import shlex
import subprocess
import sys

SOURCE_DIR = Path(__file__).resolve().parent.parent


def merge_functions_by_line(report):
    """Group recorded functions by file/definition line, preserving exclusions and line counters."""
    if report.get("gcovr/format_version") != "0.14":
        raise RuntimeError("Function aggregation requires gcovr JSON format 0.14 (gcovr 8.6)")
    for file in report["files"]:
        groups = {}
        for function in file["functions"]:
            key = (function["lineno"], function.get("gcovr/excluded", False))
            groups.setdefault(key, []).append(function)
        functions = []
        aliases = {}
        for (line, excluded), group in groups.items():
            if len(group) == 1:
                functions.extend(group)
                continue
            name = f"Definition at line {line} ({len(group)} emitted functions)"
            if excluded:
                name += " [excluded]"
            merged = {
                "name": name,
                "lineno": line,
                "execution_count": sum(function.get("execution_count", 0) for function in group),
            }
            if excluded:
                merged["gcovr/excluded"] = True
            # Block percentages cannot be combined without the underlying block identities.
            functions.append(merged)
            for function in group:
                for key in ("name", "demangled_name"):
                    if key in function:
                        aliases[function[key]] = name
        file["functions"] = functions
        for line in file["lines"]:
            name = line.get("function_name")
            if name in aliases:
                line["function_name"] = aliases[name]


def run(command, **kwargs):
    print("+ " + shlex.join(map(str, command)), flush=True)
    return subprocess.run(command, cwd=SOURCE_DIR, check=True, text=True, **kwargs)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", type=Path, default=SOURCE_DIR / "build-cov")
    parser.add_argument("--jobs", type=int, default=os.cpu_count() or 1)
    parser.add_argument(
        "--no-merge", dest="merge", action="store_false",
        help="Report template instantiations separately (merging is enabled by default)",
    )
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
    common = ["gcovr", "--root", SOURCE_DIR, "--filter", "MyGUIEngine/"]
    raw = output / "raw.json"
    run([*common, "--gcov-executable", shlex.join([cov, "gcov"]), "--json", raw, build])
    tracefile = raw
    merge_options = []
    title = "MyGUI coverage by template instantiation"
    if args.merge:
        report = json.loads(raw.read_text(encoding="utf-8"))
        merge_functions_by_line(report)
        tracefile = output / "source-lines.json"
        tracefile.write_text(json.dumps(report), encoding="utf-8")
        merge_options = ["--merge-lines"]
        title = "MyGUI coverage by source line (template instantiations combined)"
    run([*common, "--json-add-tracefile", tracefile, "--print-summary",
         *merge_options, "--sort", "uncovered-number", "--sort-reverse",
         "--html-title", title,
         "--html-details", str(output / "index.html")])
    print(f"HTML report: {(output / 'index.html').as_uri()}")


if __name__ == "__main__":
    try:
        main()
    except (OSError, RuntimeError, subprocess.CalledProcessError) as error:
        sys.exit(f"Coverage failed: {error}")
