#!/usr/bin/env python3
"""Recompile all Vulkan shaders under Media and update their committed SPIR-V files."""

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="Path to glslangValidator (or glslang)")
    args = parser.parse_args()

    search_path = os.environ.get("PATH", "")
    if os.environ.get("VULKAN_SDK"):
        search_path += os.pathsep + str(Path(os.environ["VULKAN_SDK"]) / "bin")
    compiler = args.compiler or shutil.which("glslangValidator", path=search_path) or shutil.which(
        "glslang", path=search_path
    )
    if not compiler:
        parser.error("Install glslang, set VULKAN_SDK, or pass --compiler /path/to/glslangValidator")

    root = Path(__file__).resolve().parent.parent
    sources = sorted(
        path for path in (root / "Media").rglob("*_Vulkan_*")
        if path.suffix in {".vert", ".frag", ".geom", ".tesc", ".tese", ".comp"}
    )
    # Compile everything successfully before replacing any checked-in binaries.
    with tempfile.TemporaryDirectory(prefix="mygui-spv-") as temporary:
        outputs = []
        for index, source in enumerate(sources):
            output = Path(temporary) / f"{index}.spv"
            try:
                subprocess.run([compiler, "-V", str(source), "-o", str(output)], check=True)
            except (OSError, subprocess.CalledProcessError) as error:
                parser.exit(1, f"Shader compilation failed: {error}\n")
            outputs.append((source.with_suffix(".spv"), output.read_bytes()))

        updated = 0
        for destination, data in outputs:
            if not destination.exists() or destination.read_bytes() != data:
                destination.write_bytes(data)
                print(f"Updated {destination.relative_to(root)}")
                updated += 1
        print(f"Compiled {len(sources)} shaders; updated {updated} SPIR-V files.")


if __name__ == "__main__":
    main()
