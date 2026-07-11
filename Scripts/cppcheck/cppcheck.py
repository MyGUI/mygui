#!/usr/bin/env python3

# run from root sources directory: python3 Scripts/cppcheck/cppcheck.py
import os
import subprocess
import sys


def checkFolderSources(folder):
    result = subprocess.run(
        "cppcheck -j 8 --suppressions-list=Scripts/cppcheck/suppressions.txt "
        "--enable=all --inconclusive --check-level=exhaustive --std=c++17 --quiet "
        "--project=\"" + folder + "/compile_commands.json\" --checkers-report=/tmp/checkers.txt --error-exitcode=1",
        shell=True)
    sys.exit(result.returncode)

checkFolderSources('cmake-build-debug')
