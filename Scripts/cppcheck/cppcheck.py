#!/usr/bin/env python3

# run from root sources directory: python3 Scripts/cppcheck/cppcheck.py
import os


def printOutput():
    file = open("temp.cppcheck", 'r')

    line = file.readline()
    while line != "":
        line = line[0:len(line) - 1]
        print(line)
        line = file.readline()
    file.close()


def checkFolderSources(folder):
    os.system(
        "cppcheck -j 8 --suppressions-list=Scripts/cppcheck/suppressions.txt --enable=all --std=c++17 --quiet --project=\"" + folder + "/compile_commands.json\"")
    printOutput()


checkFolderSources('cmake-build-debug')
