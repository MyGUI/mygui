# run from root sources directory: python Scripts/vera++/vera++.py
import os

ignoredEndings = []
ignoredContent = ["CMakeFiles", "MyGUI_UString", "Wrappers", "MyGUI_KeyCode.h:58", "InputConverter.h:896", "MyGUI_RTTLayer.h:21", "HotKeyManager.cpp:43", "HotKeyManager.cpp:44"]

def isIgnoredWarning(warning):
	for ignore in ignoredEndings:
		if warning.endswith(ignore):
			return True
	for ignore in ignoredContent:
		if warning.find(ignore) != -1:
			return True
	return False

def parseOutput():
	file = open("temp.vera++", 'r')
	
	line = file.readline()
	while line != "":
		line = line[0:len(line)-1]
		if (not isIgnoredWarning(line)):
			print line
		line = file.readline()
	file.close ()

def checkDir(folder):
	os.system("vera++ ../../" + folder + " -profile default 2> temp.vera++")
	parseOutput()

os.chdir("Scripts/vera++")
checkDir("*/*/*.h")
checkDir("*/*/*.cpp")
checkDir("*/*/*/*.h")
checkDir("*/*/*/*.cpp")
checkDir("Platforms/*/*/*/*.h")
checkDir("Platforms/*/*/*/*.cpp")
os.chdir("../..")

