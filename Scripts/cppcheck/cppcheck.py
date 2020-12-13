#!/usr/bin/env python3

# run from root sources directory: python Scripts/cppcheck/cppcheck.py
import os

# ignoredContent = [
# 	"is not safe, destructor throws exception",
# #	"::initialise' can be const.", # ignore, because some are not const and we keep all non-const for similarity
# #	"::shutdown' can be const.", # ignore, because some are not const and we keep all non-const for similarity
# 	"(style) The function '", # "Function is never used". False positive for library project
# 	"MyGUI_UString", # warnings from UString, gnore because it wasn't written by the MyGUI developers
# 	#"pugixml", # warnings from pugixml, ignore because it wasn't written by the MyGUI developers
# 	#"sigslot", # warnings from sigslot, ignore because it wasn't written by the MyGUI developers
# #	"Technically the member function 'input::", # useless, because with other input system this function could be non-const
# #	"Cppcheck cannot find all the include files",
# 	"is assigned in constructor body. Consider performing initialization in initialization list.", # sometimes it makes sense, when non-trivial logic involved
# 	"(performance) Prefer prefix ++/-- operators for non-primitive types", # fine for us - leave this optimisation to compiler
# #	"(error) Reference to temporary returned." # only false positives (revert later)
# 	#"does not have a copy constructor which is recommended since the class contains a pointer to allocated memory." # ignore, not important
# ]


def printOutput():
	file = open("temp.cppcheck", 'r')

	line = file.readline()
	while line != "":
		line = line[0:len(line)-1]
		print(line)
		line = file.readline()
	file.close ()

def checkFolderSources(folder) :
	os.system("cppcheck -j 8 --suppressions-list=Scripts/cppcheck/suppressions.txt --enable=all --project=\"" + folder + "/compile_commands.json\"" + " 2>temp.cppcheck")
	printOutput()

checkFolderSources('build')
