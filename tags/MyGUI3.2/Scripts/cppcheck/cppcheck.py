# run from root sources directory: python Scripts/cppcheck/cppcheck.py
import os

ignoredEndings = [
	"It is safe to deallocate a NULL pointer",
	"Throwing exception in destructor",
	"::initialise' can be const.",
	"::shutdown' can be const.",
	"hides typedef with same name"
]
ignoredContent = [
	"(style) The function '", # ignore "Function is never used"
	"MyGUI_UString", #ignore warnings from MyGUI_UString, because it wasn't written by MyGUI authors
	") Include file: ", # ignore "(debug) Include file: "name" can not be found."
	"Technically the member function 'input::", #useless, because with other input system this function could be non-const
	"Cppcheck cannot find all the include files"
]

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
	os.system("cppcheck --enable=all -I Scripts/cppcheck -I MyGUIEngine/include -D MYGUI_COMPILER=2 " + flags + " " + folder + " 2>temp.cppcheck")
	parseOutput()


checkFolderSources('MyGUIEngine', '')

checkFolderSources('Demos', '-I Common -I Common/Base/Ogre -I Common/Input/OIS -I Platforms/Ogre/OgrePlatform/include')
checkFolderSources('Tools', '-I Common -I Common/Base/Ogre -I Common/Input/OIS -I Platforms/Ogre/OgrePlatform/include')
checkFolderSources('UnitTests/UnitTest_*', '-I Common -I Common/Base/Ogre -I Common/Input/OIS -I Platforms/Ogre/OgrePlatform/include')
checkFolderSources('Common', '-I Common')
checkFolderSources('Platforms/OpenGL/OpenGLPlatform/src', '-I Platforms/OpenGL/OpenGLPlatform/include')
checkFolderSources('Platforms/Ogre/OgrePlatform/src', '-I Platforms/Ogre/OgrePlatform/include')
checkFolderSources('Platforms/DirectX/DirectXPlatform/src', '-I Platforms/DirectX/DirectXPlatform/include')
checkFolderSources('Plugins', '')
checkFolderSources('Wrappers/WrapperGenerator', '')

