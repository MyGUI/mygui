#!/usr/bin/env python3
"""Generate an SVG line-coverage badge from an llvm-cov JSON summary."""

import argparse
import json
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("report", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()

    try:
        report = json.loads(args.report.read_text(encoding="utf-8"))
        if not isinstance(report, dict) or report.get("type") != "llvm.coverage.json.export":
            raise ValueError("expected an llvm-cov JSON report")
        if len(report["data"]) != 1:
            raise ValueError("expected one combined coverage dataset")
        lines = report["data"][0]["totals"]["lines"]
        covered = lines["covered"]
        total = lines["count"]
    except (OSError, ValueError, KeyError, IndexError, TypeError) as error:
        parser.error(f"Cannot read LLVM coverage totals: {error}")
    if type(covered) is not int or type(total) is not int or total <= 0 or not 0 <= covered <= total:
        parser.error("Coverage report must contain a positive line count and valid covered-line count")

    percent = 100 * covered / total
    label = f"{percent:.1f}%"
    color = "#4c1" if percent >= 80 else "#dfb317" if percent >= 60 else "#e05d44"
    args.output.write_text(
        f'''<svg xmlns="http://www.w3.org/2000/svg" width="130" height="20" role="img" aria-label="coverage: {label}">
  <title>Line coverage: {label} ({covered}/{total})</title>
  <clipPath id="rounded"><rect width="130" height="20" rx="3"/></clipPath>
  <g clip-path="url(#rounded)">
    <rect width="70" height="20" fill="#555"/>
    <rect x="70" width="60" height="20" fill="{color}"/>
  </g>
  <g fill="#fff" text-anchor="middle" font-family="Verdana,Geneva,DejaVu Sans,sans-serif" font-size="11">
    <text x="35" y="14">coverage</text>
    <text x="100" y="14">{label}</text>
  </g>
</svg>
''',
        encoding="utf-8",
    )
    print(f"MyGUIEngine line coverage: **{label}** ({covered}/{total} lines).")


if __name__ == "__main__":
    main()
