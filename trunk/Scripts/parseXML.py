#!/usr/bin/env python
# python 2.6 required (for os.path.relpath)

# Generate *.list files with CMake SOURCE_GROUP from *.vcproj files
# goes through recursively from ../ directory


import xml.dom.minidom, os, filecmp

headers = []
source = []
alllines = []
currentFolder = ""
ignoreV8projects = True

def addSourceOrHeader(line):
    #print line
    if line == "  CMakeLists.txt":
        return
    if line.endswith('.h'):
        headers.append(line + '\n')
    else:
        source.append(line + '\n')

def get_a_document(name):
    return xml.dom.minidom.parse(name)

def parseFilter(_baseFilterNode, _filterFolder):
    lines = []
    filterName = _filterFolder
    if _filterFolder != "":
        filterName += "\\\\"
    if _baseFilterNode.attributes.getNamedItem("Name") != None:
        filterName += str(_baseFilterNode.attributes.getNamedItem("Name").nodeValue)
    lines.append("SOURCE_GROUP(\"" + filterName + "\" FILES\n")
    for filterNode in _baseFilterNode.childNodes:
        if filterNode.nodeType != filterNode.TEXT_NODE:
            if filterNode.localName == "File":
                fileName = str(filterNode.attributes.getNamedItem("RelativePath").nodeValue)
                fileName = os.path.join(root, fileName)
                fileName = fileName.replace('\\','/')
                fileName = os.path.relpath(fileName, currentFolder)
                fileName = fileName.replace('\\','/')
                #fileName = fileName.replace(currentFolder, "")
                addSourceOrHeader("  " + fileName)
                lines.append("  " + fileName + "\n")
            if filterNode.localName == "Filter":
                linesFromFile = parseFilter(filterNode, filterName)
                for line in linesFromFile:
                    alllines.append( line )
    lines.append(")\n")
    return lines

def createFilesList(fileName):

    print "Converting " + fileName
    FILE = open(fileName.replace(".vcproj", ".list").replace("_v8", ""),"w")
    doc = get_a_document(fileName)

    headers.append("set (HEADER_FILES\n")
    source.append("set (SOURCE_FILES\n")

    for rootNode in doc.childNodes:
        for subNode in rootNode.childNodes:
            if subNode.nodeType == subNode.ELEMENT_NODE and subNode.localName == "Files":
                linesFromFile = parseFilter(subNode, "")
                for line in linesFromFile:
                    alllines.append( line )

    headers.append(")\n")
    source.append(")\n")
    #remove ")" at start and add at end
    #lines.remove(")\n")
    #lines.append(")\n")
    FILE.writelines(headers)
    FILE.writelines(source)
    FILE.writelines(alllines)

    FILE.close()
    del headers[:]
    del source[:]
    del alllines[:]

def isIgnoredProject(name):
    ignores = ["api-docs", "INSTALL", "ALL_BUILD", "ZERO_CHECK", "PACKAGE"]
    for ignore in ignores:
        if name.startswith(ignore):
            return True
    if ignoreV8projects and name.endswith('_v8.vcproj') and not name.endswith('MyGUIEngine_v8.vcproj'):
        return True
    return False

# ----------
dir_src = '../'

for root, dirs, files in os.walk(dir_src):
  for name in files:
    if name.endswith('.vcproj') and not isIgnoredProject(name):
        f_src = os.path.join(root, name)
        f_src = f_src.replace('\\','/')
        currentFolder = os.path.realpath(f_src)
        currentFolder = currentFolder.replace(name, "")
        currentFolder = currentFolder.replace('\\','/')
        createFilesList(f_src)

print "Done"