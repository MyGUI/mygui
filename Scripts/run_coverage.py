#!/usr/bin/env python3
"""Build, run all unit tests, and report MyGUIEngine source-based coverage.

Requires CMake 3.22+, Clang, llvm-cov, and llvm-profdata tools
on macOS or Linux. On macOS the LLVM tools are located through xcrun.
Uses a headless Debug build with FreeType/MSDF enabled and obsolete APIs disabled.
Writes HTML to build-cov/coverage/index.html with macro expansions and branch counts,
plus report.txt, summary.json, and raw.json (LLVM's per-instantiation coverage data).
LLVM combines template instantiations in file summaries. Use --show-instantiations
(also accepted as --no-merge) to show additional per-instantiation function details.
"""

import argparse
import json
import os
from pathlib import Path
import shlex
import subprocess
import sys

SOURCE_DIR = Path(__file__).resolve().parent.parent


def coverage_binaries(build):
    """Read current target artifacts, avoiding stale executables in reused builds."""
    reply = build / ".cmake/api/v1/reply"
    index = json.loads(max(reply.glob("index-*.json"), key=lambda p: p.stat().st_mtime).read_text())
    response = index["reply"]["client-coverage"]["codemodel-v2"]
    model = json.loads((reply / response["jsonFile"]).read_text())
    configuration = next((c for c in model["configurations"] if c["name"] in ("Debug", "")), None)
    if configuration is None:
        raise RuntimeError("CMake did not provide a Debug configuration")
    engine = []
    tests = []
    for reference in configuration["targets"]:
        target = json.loads((reply / reference["jsonFile"]).read_text())
        if target["name"] == "MyGUIEngine":
            artifacts = engine
        elif target["type"] == "EXECUTABLE" and target["name"].startswith("UnitTest_"):
            artifacts = tests
        else:
            continue
        artifacts.extend((build / a["path"]).resolve() for a in target.get("artifacts", []))
    if not engine or not tests:
        raise RuntimeError("Could not locate MyGUIEngine and unit test binaries in CMake's target data")
    return list(dict.fromkeys(engine + sorted(tests)))


def run(command, **kwargs):
    print("+ " + shlex.join(map(str, command)), flush=True)
    return subprocess.run(command, cwd=SOURCE_DIR, check=True, text=True, **kwargs)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", type=Path, default=SOURCE_DIR / "build-cov")
    parser.add_argument("--jobs", type=int, default=os.cpu_count() or 1)
    parser.add_argument(
        "--show-instantiations", "--no-merge", dest="show_instantiations", action="store_true",
        help="Show per-instantiation function details and summary; file line coverage stays merged",
    )
    args = parser.parse_args()
    build = args.build_dir.resolve()
    if sys.platform not in ("darwin", "linux") or args.jobs < 1 or build == SOURCE_DIR:
        parser.error("Use macOS/Linux, a positive --jobs value, and an out-of-source build directory")

    def llvm(name):
        if sys.platform == "darwin":
            return run(["xcrun", "--find", name], capture_output=True).stdout.strip()
        return name

    clang, clangxx, cov, profdata = map(llvm, ("clang", "clang++", "llvm-cov", "llvm-profdata"))
    # Ask CMake for the actual library and executable paths, including multi-config builds.
    query = build / ".cmake/api/v1/query/client-coverage"
    query.mkdir(parents=True, exist_ok=True)
    (query / "codemodel-v2").touch()
    flags = "-fprofile-instr-generate -fcoverage-mapping"
    configure = [
        "cmake", "--preset", "minimal", "-B", build,
        "-DCMAKE_BUILD_TYPE=Debug", f"-DCMAKE_C_COMPILER={clang}", f"-DCMAKE_CXX_COMPILER={clangxx}",
        f"-DCMAKE_C_FLAGS={flags}", f"-DCMAKE_CXX_FLAGS={flags}",
        "-DCMAKE_EXE_LINKER_FLAGS=-fprofile-instr-generate",
        "-DCMAKE_SHARED_LINKER_FLAGS=-fprofile-instr-generate",
        "-DBUILD_SHARED_LIBS=ON", "-DMYGUI_BUILD_UNITTESTS=ON", "-DMYGUI_RENDERSYSTEM=1",
        "-DMYGUI_DONT_USE_OBSOLETE=ON",
        "-DMYGUI_USE_FREETYPE=ON", "-DMYGUI_MSDF_FONTS=ON",
    ]
    run(configure)
    run(["cmake", "--build", build, "--config", "Debug", "--parallel", str(args.jobs)])

    output = build / "coverage"
    profiles = output / "profiles"
    profiles.mkdir(parents=True, exist_ok=True)
    # Reset counters so earlier test runs cannot inflate coverage. Both the module
    # signature and PID are needed because tests load the instrumented shared engine.
    for profile in profiles.glob("*.profraw"):
        profile.unlink()
    env = os.environ.copy()
    env["LLVM_PROFILE_FILE"] = str(profiles / "%m-%p.profraw")
    run(["ctest", "--test-dir", build, "-C", "Debug", "--output-on-failure", "--no-tests=error"], env=env)

    raw_profiles = sorted(profiles.glob("*.profraw"))
    if not raw_profiles:
        raise RuntimeError("Tests produced no LLVM coverage profiles")
    profile_data = output / "coverage.profdata"
    run([profdata, "merge", "-sparse", *raw_profiles, "-o", profile_data])
    binaries = coverage_binaries(build)
    common = [binaries[0], f"-instr-profile={profile_data}"]
    for binary in binaries[1:]:
        common.extend(["-object", binary])
    # Include engine headers instantiated in the tests, while excluding test and
    # dependency source files from all report totals.
    sources = sorted(p for p in (SOURCE_DIR / "MyGUIEngine").rglob("*") if p.suffix in (".h", ".cpp"))
    for name, options in (("raw.json", []), ("summary.json", ["--summary-only"])):
        with (output / name).open("w", encoding="utf-8") as stream:
            run([cov, "export", *common, *options, *sources], stdout=stream)
    display = [f"--show-instantiation-summary={'true' if args.show_instantiations else 'false'}"]
    with (output / "report.txt").open("w", encoding="utf-8") as stream:
        run([cov, "report", *common, *display, *sources], stdout=stream)
    run([cov, "show", *common, *display, "--format=html", f"--output-dir={output}",
         "--project-title=MyGUIEngine source-based coverage", "--show-branches=count", "--show-expansions",
         f"--show-instantiations={'true' if args.show_instantiations else 'false'}", *sources],
        stdout=subprocess.DEVNULL)
    totals = json.loads((output / "summary.json").read_text())["data"][0]["totals"]
    metrics = ["lines", "functions", "branches"]
    if args.show_instantiations:
        metrics.append("instantiations")
    for metric in metrics:
        counts = totals[metric]
        print(f"{metric}: {counts['percent']:.1f}% ({counts['covered']} out of {counts['count']})")
    print(f"HTML report: {(output / 'index.html').as_uri()}")


if __name__ == "__main__":
    try:
        main()
    except (OSError, RuntimeError, subprocess.CalledProcessError) as error:
        sys.exit(f"Coverage failed: {error}")
