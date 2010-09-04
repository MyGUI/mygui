# run from root sources directory: python Scripts/cppcheck/cppcheck.py
import os

ignoredEndings = ["is never used", "It is safe to deallocate a NULL pointer", "Throwing exception in destructor"]
ignoredContent = ["MyGUI_UString"]

def isIgnoredWarning(warning):
	for ignore in ignoredEndings:
		if warning.endswith(ignore):
			return True
	for ignore in ignoredContent:
		if warning.find(ignore) != -1:
			return True
	return False

def parseOutput():
	file = open("temp.cppcheck", 'r')
	
	line = file.readline()
	while line != "":
		line = line[0:len(line)-1]
		if (not isIgnoredWarning(line)):
			print line
		line = file.readline()
	file.close ()

def checkFolderSources(folder, flags) :
	os.system("cppcheck --enable=all -I Scripts/cppcheck -I MyGUIEngine/include " + flags + " " + folder + " 2>temp.cppcheck")
	parseOutput()


checkFolderSources('MyGUIEngine', '')

checkFolderSources('Demos', '-I Common -I Common/Base/Ogre -I Common/Input/OIS -I Platforms/Ogre/OgrePlatform/include')
checkFolderSources('Tools', '-I Common -I Common/Base/Ogre -I Common/Input/OIS -I Platforms/Ogre/OgrePlatform/include')
checkFolderSources('UnitTests/UnitTest_*', '-I Common -I Common/Base/Ogre -I Common/Input/OIS -I Platforms/Ogre/OgrePlatform/include')
checkFolderSources('Common', '-I Common')
# include temporary disabled due to cppcheck bug
#checkFolderSources('Platforms/OpenGL/src', '-I Platforms/OpenGL/OpenGLPlatform/include')
checkFolderSources('Platforms/Ogre/src', '-I Platforms/Ogre/OgrePlatform/include')
checkFolderSources('Platforms/DirectX/src', '-I Platforms/DirectX/DirectXPlatform/include')
checkFolderSources('Plugins', '')
checkFolderSources('Wrapper', '')

