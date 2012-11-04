# run from root sources directory: python Scripts/cppcheck/cppcheck.py
import os

ignoredContent = [
#	"It is safe to deallocate a NULL pointer", # ignore, fine for us
	"Exception thrown in destructor.", # ignore, fine for us
#	"::initialise' can be const.", # ignore, because some are not const and we keep all non-const for similarity
#	"::shutdown' can be const.", # ignore, because some are not const and we keep all non-const for similarity
	"(style) The function '", # ignore "Function is never used"
	"MyGUI_UString", # ignore warnings from UString, because it wasn't written by the MyGUI developers
	"pugixml", # ignore warnings from pugixml, because it wasn't written by the MyGUI developers
	"sigslot", # ignore warnings from sigslot, because it wasn't written by the MyGUI developers
#	") Include file: ", # ignore "(debug) Include file: "name" can not be found."
#	"Technically the member function 'input::", # useless, because with other input system this function could be non-const
#	"Cppcheck cannot find all the include files",
	"is assigned in constructor body. Consider performing initialization in initialization list.", # ignore, fine for us
	"(performance) Prefer prefix ++/-- operators for non-primitive types", # ignore, fine for us - leave this optimisation to compiler
	"MyGUI_DelegateImplement.h:195]: (warning) 'operator=' should check for assignment to self to avoid problems with dynamic memory.", # false positive
#	"(error) Reference to temporary returned." # only false positives (revert later)
	"does not have a copy constructor which is recommended since the class contains a pointer to allocated memory." # ignore, not important
]

def isIgnoredWarning(warning):
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

checkFolderSources('Demos', '-I Common -I Common/Base/Ogre -I Common/Input/OIS -I Platforms/Ogre/OgrePlatform/include -I Plugins/Plugin_BerkeliumWidget')
checkFolderSources('Tools', '-I Common -I Common/Base/Ogre -I Common/Input/OIS -I Platforms/Ogre/OgrePlatform/include -I Tools/EditorFramework')
checkFolderSources('UnitTests/UnitTest_*', '-I Common -I Common/Base/Ogre -I Common/Input/OIS -I Platforms/Ogre/OgrePlatform/include')
checkFolderSources('Common', '-I Common -I Common/Input/OIS -I Platforms/Ogre/OgrePlatform/include')
checkFolderSources('Platforms/OpenGL/OpenGLPlatform/src', '-I Platforms/OpenGL/OpenGLPlatform/include -I Common')
checkFolderSources('Platforms/Ogre/OgrePlatform/src', '-I Platforms/Ogre/OgrePlatform/include')
checkFolderSources('Platforms/DirectX/DirectXPlatform/src', '-I Platforms/DirectX/DirectXPlatform/include -I Common')
checkFolderSources('Platforms/DirectX11/DirectX11Platform/src', '-I Platforms/DirectX11/DirectX11Platform/include -I Common')
checkFolderSources('Plugins', '')
checkFolderSources('Wrappers/MyGUI_Export', ' -I Common')

