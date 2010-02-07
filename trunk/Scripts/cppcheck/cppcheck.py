# run from root sources directory: python Scripts/cppcheck/cppcheck.py
import os

def checkFolderSources(folder) :
	os.system("cppcheck --enable=all -I MyGUIEngine/Scripts/cppheck " + folder)


os.system("cppcheck --enable=all -I Scripts/cppheck -I MyGUIEngine/include MyGUIEngine/src")
#checkFolderSources('MyGUIEngine')

checkFolderSources('Demos')
checkFolderSources('Tools')
checkFolderSources('UnitTests')
checkFolderSources('Common')
#checkFolderSources('Platforms/OpenGL')
os.system("cppcheck --enable=all -I MyGUIEngine/Scripts/cppheck -I Platforms/OpenGL/OpenGLPlatform/include Platforms/OpenGL/OpenGLPlatform/src")
#checkFolderSources('Platforms/Ogre')
os.system("cppcheck --enable=all -I MyGUIEngine/Scripts/cppheck -I Platforms/Ogre/OgrePlatform/include Platforms/Ogre/OgrePlatform/src")
#checkFolderSources('Platforms/DirectX')
os.system("cppcheck --enable=all -I MyGUIEngine/Scripts/cppheck -I Platforms/DirectX/DirectXPlatform/include Platforms/DirectX/DirectXPlatform/src")
checkFolderSources('Platforms/OpenGL')
checkFolderSources('Platforms/OGRE')
checkFolderSources('Platforms/DirectX')
checkFolderSources('Plugins')
checkFolderSources('Wrapper')
