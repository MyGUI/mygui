#!/usr/bin/env python
# python 2.6 required (for os.path.relpath)

# Replace absolute paths with relative
# goes through recursively from ../release directory

import os, filecmp

directories_release = ['..\solution_directx', '..\solution_ogre', '..\solution_opengl']
dir_sources = '..'

alllines = []
currentFolder = ""

def findChar(line, character, frompos):
	pos = line.find(character, frompos)
	if pos == -1:
		return 999999
	else:
		return pos
def replaceAbsolutePaths(fileName):

	print "Converting " + fileName
	file = open(fileName,"r")

	relpath = os.path.relpath(dir_sources, currentFolder)
	relpath = relpath.replace('\\','/')
	#print "Current folder: " + currentFolder
	#print "Relative path: " + relpath
	#print "dir_sources: " + dir_sources
	#print os.path.join(currentFolder, relpath)
	
	# trackPrint used for debug only
	trackPrint = False
	if (trackPrint):
		trackPrint = True
		trackFile = open("trackedLine.txt","w")
		trackFile.write("original :" + line)

	line = file.readline()
	while (line) != "":
		backSlash = False
		
		if (line.find(dir_sources.replace('/','\\')) != -1):
			backSlash = True
			line = line.replace('\\','/')
			if trackPrint:
				trackFile.write("replace \\:" + line)
		pos = line.find(dir_sources)
		while (pos != -1):
			#print "Line changed: " + line.lstrip()
			rpos1 = findChar(line,"\"", pos)
			rpos2 = findChar(line,"<", pos)
			rpos3 = findChar(line,";", pos)
			rpos = min(rpos1, min(rpos2, rpos3))
			path = line[pos : rpos] #.replace('\\','/')
			relpath = os.path.relpath(path, currentFolder).replace('\\','/')
			#print path + " | " + relpath
			
			if trackPrint:
				trackFile.write("relpath  :" + relpath + "\n")
			
			if (backSlash):
				relpath = relpath.replace('/','\\')
			line = line.replace(path, relpath, 1)
			
			if trackPrint:
				trackFile.write("relpath \\:" + relpath + "\n")
				trackFile.write("line unrel" + line)
			
			#print "to next line: " + line.lstrip()
			
			pos = line.find(dir_sources)
		
		line = line.replace("C:/MYGUIHACK ", "$(")
		line = line.replace("C:\\MYGUIHACK ", "$(")
		line = line.replace(" MYGUIBRACKETHACK", ")")
		
		if (line.find("cmake.exe") != -1 and line.find("CommandLine") != -1):
			lpos = line.find("\"", 0)
			rpos = line.find("\"", lpos + 1)
			if (trackPrint):
				trackFile.write("Remove cmake.exe: " + line)
				trackFile.write("           lines: " + str(lpos) + " " + str(rpos) + "\n")
				trackFile.write("        replaced: " + line.replace(line[lpos + 1 : rpos], ""))
			line = line.replace(line[lpos + 1 : rpos], "")
			stopParse = True
		
		alllines.append( line )
		line = file.readline()

	if trackPrint:
		trackFile.close()

	file = open(fileName,"w")
	#file = open(fileName + ".txt","w")
	file.writelines(alllines)
	file.close()

	del alllines[:]

def isIgnoredProject(name):
	#ignores = ["api-docs", "INSTALL", "ALL_BUILD", "ZERO_CHECK", "PACKAGE"]
	ignores = []
	for ignore in ignores:
		if name.startswith(ignore):
			return True
	if name.endswith(".cmake") or name.endswith("tmp"):
		return True
	return False

# ----------

dir_sources = os.path.realpath(dir_sources)
dir_sources = dir_sources.replace('\\','/')

index = 0
for dir_release in directories_release:
	index = index+1
	os.system("mkdir " + dir_release)
	os.chdir(dir_release)
	os.system("cmake ../ -DMYGUI_RENDERSYSTEM=" + str(index) + " -DMYGUI_BUILD_UNITTESTS=True -DMYGUI_BUILD_WRAPPER=True -DMYGUI_STANDALONE_BUILD=True -DMYGUI_BUILD_DOCS=True")
	os.chdir("../Scripts")
	for root, dirs, files in os.walk(dir_release):
		for name in files:
			if not isIgnoredProject(name):
				
				f_src = os.path.join(root, name)
				f_src = f_src.replace('\\','/')
				currentFolder = os.path.realpath(f_src)
				currentFolder = currentFolder.replace(name, "")
				currentFolder = currentFolder.replace('\\','/')
				replaceAbsolutePaths(f_src)

print "Done"