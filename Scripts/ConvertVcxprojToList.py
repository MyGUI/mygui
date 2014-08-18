#!/usr/bin/env python
# python 2.6+ required (for os.path.relpath)

# Generate *.list files with CMake SOURCE_GROUP from *.vcproj files
# goes through recursively from ../ directory


import xml.dom.minidom, os, filecmp, sys, filecmp

headers = []
sources = []
filters = {}
currentFolder = ""
currentRoot = ""

def addSourceOrHeader(fileName):
    #print line
    if fileName.endswith('.h'):
        headers.append("  " + fileName + "\n")
    else:
        sources.append("  " + fileName + "\n")

def get_a_document(name):
    return xml.dom.minidom.parse(name)

def convertRelativePath(fileName):
    fileName = os.path.join(currentRoot, fileName)
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

    if (len(_node.getElementsByTagName("Filter")) != 0):
      filterName = _node.getElementsByTagName("Filter")[0].childNodes[0].nodeValue
      filterName = filterName.replace("\\", "\\\\")

      if not filters.has_key(filterName):
        filters[filterName] = []
      filters[filterName].append("  " + fileName + "\n")

def writeHeaders(FILE):
    headers.sort()
    FILE.writelines("set (HEADER_FILES\n")
    FILE.writelines(headers)
    FILE.writelines(")\n")
def writeSources(FILE):
    sources.sort()
    FILE.writelines("set (SOURCE_FILES\n")
    FILE.writelines(sources)
    FILE.writelines(")\n")
def writeFilters(FILE):
    for filterName in sorted(filters.keys()):
      filters[filterName].sort()
      FILE.writelines("SOURCE_GROUP(\"" + filterName + "\" FILES\n")
      FILE.writelines(filters[filterName])
      FILE.writelines(")\n")

def createFilesList(fileName, listName):
    if (not os.path.exists(listName)):
        return

    print "Converting " + fileName
    doc = get_a_document(fileName)

    for rootNode in doc.childNodes:
        for subNode in rootNode.childNodes:
            if subNode.localName == "ItemGroup":
                for subSubNode in subNode.childNodes:
                    if subSubNode.nodeType == subSubNode.ELEMENT_NODE and (subSubNode.localName == "ClInclude" or subSubNode.localName == "ClCompile" or subSubNode.localName == "ResourceCompile"):
                        parseIncludedFile(subSubNode)

    doc = get_a_document(fileName + ".filters")
    for rootNode in doc.childNodes:
        for subNode in rootNode.childNodes:
            if subNode.localName == "ItemGroup":
                for subSubNode in subNode.childNodes:
                    if subSubNode.nodeType == subSubNode.ELEMENT_NODE and (subSubNode.localName == "ClInclude" or subSubNode.localName == "ClCompile" or subSubNode.localName == "ResourceCompile" or subSubNode.localName == "CustomBuild"):
                        parseFilterFile(subSubNode)

    FILE = open("tmp.list", "w")
    writeHeaders(FILE)
    writeSources(FILE)
    writeFilters(FILE)
    FILE.close()

    del headers[:]
    del sources[:]
    filters.clear()

    if (not filecmp.cmp("tmp.list", listName)):
      if (os.path.exists(listName)):
        os.remove(listName)
      os.rename("tmp.list", listName)
    else:
      os.remove("tmp.list")
def isIgnoredProject(name):
    ignores = ["Common", "api-docs", "INSTALL", "ALL_BUILD", "ZERO_CHECK", "PACKAGE"]
    for ignore in ignores:
        if name.startswith(ignore):
            return True
    return False

def main():
  global currentRoot
  global currentFolder
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
            currentRoot = root
            f_src = os.path.join(root, name)
            f_src = f_src.replace('\\','/')
            currentFolder = f_src
            currentFolder = currentFolder.replace(name, "")
            currentFolder = currentFolder.replace('\\','/')

            currentFolder = os.path.join(dir_src, os.path.relpath(currentFolder, dir_solution))

            listName = f_src.replace(".vcxproj", ".list")
            listName = os.path.relpath(listName, dir_solution)
            listName = os.path.join(dir_src, listName)
            #print listName
            createFilesList(f_src, listName)

    print "Done"

if (not os.path.exists("lock")):
  try:
    FILE = open("lock", "w")
    FILE.close()

    main()

    os.remove("lock")
  except:
    if (os.path.exists("lock")):
      os.remove("lock")
    if (os.path.exists("tmp.list")):
      os.remove("tmp.list")
    raise
else:
  print "Already converting... Exiting..."