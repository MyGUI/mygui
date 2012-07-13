#!/usr/bin/env python
# python 2.6+ required (for os.path.relpath)

# Generate *.list files with CMake SOURCE_GROUP from *.vcproj files
# goes through recursively from ../ directory


import xml.dom.minidom, os, filecmp, sys

headers = []
source = []
filters = []
currentFolder = ""
lastFilter = ""

def addSourceOrHeader(fileName):
    #print line
    if fileName.endswith("CMakeLists.txt"):
        return
    if fileName.endswith('.h'):
        headers.append("  " + fileName + "\n")
    else:
        source.append("  " + fileName + "\n")

def get_a_document(name):
    return xml.dom.minidom.parse(name)

def convertRelativePath(fileName):
    fileName = os.path.join(root, fileName)
    fileName = fileName.replace('\\','/')
    fileName = os.path.relpath(fileName, currentFolder)
    fileName = fileName.replace('\\','/')
    return fileName

def parseIncludedFile(_node):
    fileName = str(_node.attributes["Include"].nodeValue)
    fileName = convertRelativePath(fileName)
    addSourceOrHeader(fileName)

def parseFilterFile(_node):
    fileName = str(_node.attributes["Include"].nodeValue)
    fileName = convertRelativePath(fileName)

    filterName = _node.getElementsByTagName("Filter")[0].childNodes[0].nodeValue
    filterName = filterName.replace("\\", "\\\\")

    global lastFilter
    if (filterName != lastFilter):
      if lastFilter != "":
        # close last filter
        filters.append(")\n")
      filters.append("SOURCE_GROUP(\"" + filterName + "\" FILES\n")
      lastFilter = filterName

    filters.append("  " + fileName + "\n")

def createFilesList(fileName, listName):

    print "Converting " + fileName
    doc = get_a_document(fileName)

    headers.append("set (HEADER_FILES\n")
    source.append("set (SOURCE_FILES\n")

    for rootNode in doc.childNodes:
        for subNode in rootNode.childNodes:
            if subNode.localName == "ItemGroup":
                for subSubNode in subNode.childNodes:
                    if subSubNode.nodeType == subSubNode.ELEMENT_NODE and (subSubNode.localName == "ClInclude" or subSubNode.localName == "ClCompile" or subSubNode.localName == "ResourceCompile"):
                        parseIncludedFile(subSubNode)

    headers.append(")\n")
    source.append(")\n")

    global lastFilter
    lastFilter = ""
    doc = get_a_document(fileName + ".filters")
    for rootNode in doc.childNodes:
        for subNode in rootNode.childNodes:
            if subNode.localName == "ItemGroup":
                for subSubNode in subNode.childNodes:
                    if subSubNode.nodeType == subSubNode.ELEMENT_NODE and (subSubNode.localName == "ClInclude" or subSubNode.localName == "ClCompile" or subSubNode.localName == "ResourceCompile"):
                        parseFilterFile(subSubNode)

    filters.append(")\n")

    FILE = open(listName, "w")
    FILE.writelines(headers)
    FILE.writelines(source)
    FILE.writelines(filters)
    FILE.close()

    del headers[:]
    del source[:]
    del filters[:]

def isIgnoredProject(name):
    ignores = ["Common", "api-docs", "INSTALL", "ALL_BUILD", "ZERO_CHECK", "PACKAGE"]
    for ignore in ignores:
        if name.startswith(ignore):
            return True
    return False

# ----------
dir_src = '../'
try:
  dir_solution = sys.argv[1]
  print "Sources directory is: " + dir_src
  print "Solution directory is: " + dir_solution
except:
  print "Error: missing argument"
  print "Usage: parseXML <path_to_solution>"
else:
  for root, dirs, files in os.walk(dir_solution):
    for name in files:
      if name.endswith('.vcxproj') and not isIgnoredProject(name):
          f_src = os.path.join(root, name)
          f_src = f_src.replace('\\','/')
          currentFolder = f_src #os.path.realpath(f_src)
          currentFolder = currentFolder.replace(name, "")
          currentFolder = currentFolder.replace('\\','/')

          currentFolder = os.path.join(dir_src, os.path.relpath(currentFolder, dir_solution))

          listName = f_src.replace(".vcxproj", ".list")
          listName = os.path.relpath(listName, dir_solution)
          listName = os.path.join(dir_src, listName)
          #print listName
          createFilesList(f_src, listName)

  print "Done"