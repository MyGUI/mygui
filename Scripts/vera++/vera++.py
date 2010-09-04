# run from root sources directory: python Scripts/vera++/vera++.py
import os

ignoredEndings = []
ignoredContent = ["MyGUI_UString", "MyGUI_KeyCode.h:58"]

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
checkDir("MyGUIEngine/src/*.cpp")
checkDir("MyGUIEngine/include/*.h")
checkDir("*/*/*.h")
checkDir("*/*/*.cpp")
checkDir("Common/*/*/*.h")
checkDir("Common/*/*/*.cpp")
checkDir("Platforms/*/*/*/*.h")
checkDir("Platforms/*/*/*/*.cpp")
os.chdir("../..")

