#!/usr/bin/env python3
"""
Compare screenshots produced by demos/tools/unit tests against committed references.

Every demo/tool/unit test supports the "--screenshot <path>.png" command line option:
it renders the scene, saves a screenshot after a few frames and exits. This script
runs each application from a fixed list and compares the resulting image with a
reference screenshot stored in Scripts/Tests/References/<App>.png.

Requires numpy and Pillow:

    python3 -m pip install numpy pillow

Usage:
    python3 Scripts/Tests/compare_screenshots.py <build_dir> [--baseline] [--tolerance N]

By default, the script compares generated screenshots with references and reports
the result for every application:

    OK         - the screenshot is pixel-identical to the reference
    WARNING    - the screenshot differs from the reference within the tolerance
    FAIL       - the screenshot is missing or differs too much from the reference

With "--tolerance N" pixels that differ by at most N per channel are treated as
matching. This is useful when the renderer and the reference use different
texture filtering/rounding (e.g. D3D truncates bilinear interpolation where the
GL3 references round to nearest), which can shift interpolated pixels by exactly
one unit per channel. The default is 0, i.e. an exact pixel-identical match.

With "--baseline" the generated screenshots overwrite the reference images instead.
"""

import argparse
import os
import shutil
import signal
import subprocess
import sys
import tempfile
from pathlib import Path

import numpy as np
from PIL import Image

# Each entry is the name of the executable. Missing binaries are skipped.
APPS = (
    # Demos
    "Demo_Colour",
    "Demo_Console",
    "Demo_Controllers",
    "Demo_Gui",
    "Demo_ItemBox",
    "Demo_MsdfFont",
    "Demo_PanelView",
    "Demo_Picking",
    "Demo_Pointers",
    "Demo_RenderBox",
    "Demo_ScrollView",
    "Demo_Themes",
    # Tools (FontConverter is a command line utility)
    "FontEditor",
    "ImageEditor",
    "LayoutEditor",
    "SkinEditor",
    # Unit tests
    "UnitTest_AttachDetach",
    "UnitTest_ChildAttach",
    "UnitTest_Delegates",
    "UnitTest_GraphView",
    "UnitTest_HyperTextBox",
    "UnitTest_ItemBox_Info",
    # "UnitTest_Layers", # Ogre only
    "UnitTest_MultiList",
    "UnitTest_RTTLayer",
    "UnitTest_RotatingSkin",
    "UnitTest_Spline",
    "UnitTest_TextureAnimations",
    "UnitTest_TreeControl",
)

DEFAULT_FAIL_MEAN = 2.0
DEFAULT_FAIL_RATIO = 0.10
RUN_TIMEOUT = 120

REFERENCE_DIR = Path(__file__).resolve().parent / "References"
DIFF_DIR = Path(__file__).resolve().parent / "Diffs"


class PngError(Exception):
    pass


def _load_rgba(path: Path) -> np.ndarray:
    """Decode a PNG into an RGBA array of shape (height, width, 4)."""
    try:
        with Image.open(path) as img:
            return np.asarray(img.convert("RGBA"))
    except (OSError, ValueError) as exc:
        raise PngError(f"cannot decode {path.name}: {exc}") from exc


def find_binary(build_dir: Path, app: str) -> Path | None:
    if os.name == "nt":
        candidates = [build_dir / "bin" / f"{app}.exe"]
    else:
        candidates = [build_dir / "bin" / app]
    for candidate in candidates:
        if candidate.is_file():
            return candidate
    return None


def run_app(binary: Path, app: str, work_dir: Path) -> tuple[Path, str]:
    """Run an application and return (screenshot_path, log)."""
    screenshot = work_dir / f"{app}.png"
    command = [str(binary), "--screenshot", str(screenshot)]
    env = os.environ.copy()
    xvfb_run = shutil.which("xvfb-run")
    if xvfb_run:
        command = [xvfb_run, "-a", *command]
    elif "DISPLAY" not in env and os.name != "nt":
        return screenshot, (
            "no DISPLAY set and xvfb-run not found; "
            "cannot create a rendering window"
        )
    try:
        #print(f"Running app: {' '.join(command)}")
        process = subprocess.Popen(
            command,
            cwd=str(work_dir),
            env=env,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            start_new_session=True,
        )
    except OSError as exc:
        return screenshot, f"failed to launch: {exc}"
    try:
        stdout, stderr = process.communicate(timeout=RUN_TIMEOUT)
    except subprocess.TimeoutExpired:
        _kill_process_tree(process)
        return screenshot, f"timed out after {RUN_TIMEOUT} seconds"
    if process.returncode != 0:
        log = (stdout or "") + (stderr or "")
        return screenshot, log.strip() or f"exited with code {process.returncode}"
    if not screenshot.is_file():
        return screenshot, "screenshot was not created"
    return screenshot, ""


def _kill_process_tree(process: subprocess.Popen):
    """Kill the process and its children (xvfb-run spawns Xvfb + the app)."""
    try:
        os.killpg(os.getpgid(process.pid), signal.SIGKILL)
    except (ProcessLookupError, PermissionError):
        try:
            process.kill()
        except ProcessLookupError:
            pass
    process.wait()


def compare(
        generated: Path,
        reference: Path,
        fail_mean: float,
        fail_ratio: float,
        tolerance: int = 0,
):
    """Compare a generated screenshot with its reference.

    Returns (status, detail, diff image). status is one of "OK", "WARNING", "FAIL".
    The diff image marks differing pixels in red on black (None on exact match).
    Pixels that differ by at most ``tolerance`` per channel are treated as matching.
    """
    if not reference.is_file():
        return "FAIL", "no reference screenshot", None
    try:
        shot = _load_rgba(generated)
        ref = _load_rgba(reference)
    except PngError as exc:
        return "FAIL", f"cannot decode: {exc}", None

    if shot.shape != ref.shape:
        return (
            "FAIL",
            f"size mismatch: {shot.shape[1]}x{shot.shape[0]} vs "
            f"{ref.shape[1]}x{ref.shape[0]}",
            None,
        )

    if tolerance:
        mask = np.any(np.abs(shot.astype(np.int16) - ref.astype(np.int16)) > tolerance, axis=2)
        detail_extra = f", tolerance +/-{tolerance}"
    else:
        mask = np.any(shot != ref, axis=2)
        detail_extra = ""
    differing_pixels = int(np.count_nonzero(mask))
    total_pixels = shot.shape[0] * shot.shape[1]
    diff_ratio = differing_pixels / total_pixels
    mean_diff = np.abs(shot.astype(np.int16) - ref.astype(np.int16)).mean()

    if diff_ratio == 0.0:
        return "OK", "match", None

    detail = (
        f"{diff_ratio * 100:05.2f}% ({differing_pixels} pixels) differ, "
        f"mean diff {mean_diff:.2f}{detail_extra}"
    )

    diff = Image.fromarray(
        np.where(mask[..., None], (255, 0, 0, 255), (0, 0, 0, 255)).astype(np.uint8)
    )

    if diff_ratio <= fail_ratio and mean_diff <= fail_mean:
        return "WARNING", detail, diff
    return "FAIL", f"{detail} (over tolerance)", diff


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Run demos/tools/unit tests with --screenshot and compare the "
        "result with reference screenshots."
    )
    parser.add_argument("build_dir", type=Path, help="CMake build directory")
    parser.add_argument(
        "--baseline",
        action="store_true",
        help="overwrite reference screenshots instead of comparing",
    )
    parser.add_argument("--app", help="run only the given application (debugging)")
    parser.add_argument(
        "--fail-mean",
        type=float,
        default=DEFAULT_FAIL_MEAN,
        help=f"mean channel difference that marks a result as FAIL (default {DEFAULT_FAIL_MEAN})",
    )
    parser.add_argument(
        "--fail-ratio",
        type=float,
        default=DEFAULT_FAIL_RATIO,
        help=f"fraction of differing pixels that marks a result as FAIL (default {DEFAULT_FAIL_RATIO})",
    )
    parser.add_argument(
        "--tolerance",
        type=int,
        default=0,
        help="per-channel absolute tolerance for considering a pixel as matching "
             "(default 0 = exact match); use 1 to account for D3D vs GL3 texture "
             "filtering rounding differences",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=None,
        help="directory for generated screenshots (kept even without --keep)",
    )
    parser.add_argument(
        "--keep",
        action="store_true",
        help="keep generated screenshots",
    )
    parser.add_argument(
        "--strict",
        action="store_true",
        help="treat WARNING results as failures (exit code 1)",
    )
    args = parser.parse_args(argv)

    # Progress must be visible even when output is piped (CI, tee, etc.)
    try:
        sys.stdout.reconfigure(line_buffering=True)
    except AttributeError:
        pass

    build_dir = args.build_dir.resolve()
    if not build_dir.is_dir():
        print(f"ERROR: build directory not found: {build_dir}", file=sys.stderr)
        return 2

    apps = [args.app] if args.app else APPS

    if args.baseline:
        REFERENCE_DIR.mkdir(parents=True, exist_ok=True)

    results = []
    exit_code = 0
    with tempfile.TemporaryDirectory(prefix="mygui_screenshot_") as tmp:
        tmp_dir = Path(tmp)
        for app in apps:
            binary = find_binary(build_dir, app)
            if not binary:
                print(f"SKIP    {app}: binary not found in {build_dir / 'bin'}")
                continue

            screenshot, log = run_app(binary, app, tmp_dir)
            if log:
                print(f"FAIL    {app}: {log}")
                results.append(("FAIL", app, log))
                exit_code = 1
                continue

            if args.output:
                args.output.mkdir(parents=True, exist_ok=True)
                shutil.copy2(screenshot, args.output / screenshot.name)
            elif args.keep:
                REFERENCE_DIR.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(screenshot, REFERENCE_DIR.parent / screenshot.name)

            ref_screenshot = REFERENCE_DIR / f"{app}.png"
            if args.baseline:
                shutil.copy2(screenshot, ref_screenshot)
                print(f"BASELINE {app}: {ref_screenshot}")
                continue

            print(f"Checking {app}{' ' * 20}", end="\r")

            status, detail, diff = compare(
                screenshot, ref_screenshot, args.fail_mean, args.fail_ratio, args.tolerance
            )
            results.append((status, app, detail))
            if status == "FAIL":
                exit_code = 1
            if status != "OK" and diff is not None:
                DIFF_DIR.mkdir(parents=True, exist_ok=True)
                diff.save(DIFF_DIR / f"{app}.png")
                shutil.copy2(ref_screenshot, DIFF_DIR / f"{app}_old.png")
                shutil.copy2(screenshot, DIFF_DIR / f"{app}_new.png")

    if not args.baseline:
        print(f"{'STATUS':<9} {'APP':<32} DETAIL")
        print("-" * 100)
        for status, app, detail in results:
            print(f"{status:<9} {app:<32} {detail}")
        if exit_code and not args.strict:
            print()
            print(f"Failed: {sum(1 for s, _, _ in results if s == 'FAIL')} app(s).")
        elif args.strict and any(s in ("FAIL", "WARNING") for s, _, _ in results):
            exit_code = 1
            print()
            print("One or more apps are not pixel-identical (--strict).")

    return exit_code


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
