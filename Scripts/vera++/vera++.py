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

os.chdir("Scripts/vera++")
os.system("vera++ ../../MyGUIEngine/src/*.cpp -profile default 2> temp.vera++")
parseOutput()
os.system("vera++ ../../MyGUIEngine/include/*.h -profile default 2> temp.vera++")
parseOutput()
os.chdir("../..")

