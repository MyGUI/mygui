#!/usr/bin/env python3
"""
Check that all MyGUIEngine headers compile cleanly in isolation.
"""
import subprocess
import sys
import os
import glob
import multiprocessing
import shlex
from pathlib import Path

INCLUDE_DIR = Path(__file__).resolve().parent.parent / "MyGUIEngine" / "include"
CLANG = "clang++"
STD = "-std=c++17"

# Headers that are intentionally not standalone (only usable via MyGUI_Prerequest.h)
SKIP = {"MyGUI_DeprecatedTypes.h", "MyGUI_DeprecatedWidgets.h"}


def check_header(header_path: str, define_flags: str) -> tuple[str, bool, str]:
    name = os.path.basename(header_path)
    defines_list = shlex.split(define_flags)
    source = f'#include "{name}"'

    try:
        result = subprocess.run(
            [CLANG, STD, *defines_list, "-I", str(INCLUDE_DIR), "-fsyntax-only", "-x", "c++", "-"],
            input=source,
            capture_output=True,
            text=True,
            timeout=120,
        )
        if result.returncode == 0:
            return name, True, ""
        errors = [line.strip() for line in result.stderr.split("\n") if "error:" in line]
        return name, False, "; ".join(errors[:3])
    except subprocess.TimeoutExpired:
        return name, False, "TIMEOUT"
    except FileNotFoundError:
        print("ERROR: clang++ not found.", file=sys.stderr)
        sys.exit(1)


def check_all():
    headers = sorted(
        h for h in glob.glob(str(INCLUDE_DIR / "*.h"))
        if os.path.basename(h) not in SKIP
    )
    if not headers:
        print(f"ERROR: no headers found in {INCLUDE_DIR}", file=sys.stderr)
        sys.exit(1)

    print(f"Checking {len(headers)} MyGUIEngine headers (skipping {len(SKIP)} by design)\n")

    scenarios = [
        "-D MYGUI_USE_FREETYPE -D MYGUI_MSDF_FONTS",
        "-D MYGUI_DONT_USE_OBSOLETE",
    ]

    all_ok = True
    for define_flags in scenarios:
        print(f"--- {define_flags} ---")

        with multiprocessing.Pool() as pool:
            async_results = [pool.apply_async(check_header, (h, define_flags)) for h in headers]
            results = []
            for i, ar in enumerate(async_results):
                name, ok, detail = ar.get()
                results.append((name, ok, detail))
                print(f"  [{i + 1}/{len(headers)}] {name}{' ' * 20}", end="\r" if ok else "\n")
                sys.stdout.flush()
        print()

        failed = [(name, detail) for name, ok, detail in results if not ok]
        passed = sum(1 for _, ok, _ in results if ok)

        print(f"  Passed: {passed}/{len(headers)}")
        if failed:
            all_ok = False
            print(f"  Failed: {len(failed)}")
            for name, detail in failed:
                print(f"    FAIL  {name}")
                if detail:
                    for line in detail.split("; "):
                        print(f"          {line}")
        print()

    if all_ok:
        print("All standalone headers compile cleanly.")
        return 0
    print("Some headers have unexpected errors (see above).")
    print("To fix: add forward declaration or missing #include(s) to the failing header(s) and re-run.")
    return 1


if __name__ == "__main__":
    sys.exit(check_all())
