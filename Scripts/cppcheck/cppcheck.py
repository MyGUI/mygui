#!/usr/bin/env python3

# run from root sources directory: python3 Scripts/cppcheck/cppcheck.py
import os


def checkFolderSources(folder):
    os.system(
        "cppcheck -j 8 --suppressions-list=Scripts/cppcheck/suppressions.txt "
        "--enable=all --std=c++17 --quiet "
        "--project=\"" + folder + "/compile_commands.json\"")


checkFolderSources('cmake-build-debug')
