#!/usr/bin/env python
# Generate *.list files with CMake SOURCE_GROUP from *.vcproj files
# goes through recursively from ../ directory

import xml.dom.minidom, os, filecmp

headers = []
source = []
lines = []

def addLine(line):
    #print line
    if line.endswith('.h'):
        headers.append(line + '\n')

    if line.endswith('.cpp'):
        source.append(line + '\n')

    lines.append(line + '\n')

def get_a_document(name):
    return xml.dom.minidom.parse(name)

def parseFilter(_baseFilterNode, _filterFolder):
    for filterNode in _baseFilterNode.childNodes:
        if filterNode.nodeType != filterNode.TEXT_NODE:
            if filterNode.localName == "File":
                fileName = str(filterNode.attributes.getNamedItem("RelativePath").nodeValue)
                fileName = fileName.replace('\\','/')
                addLine("  " + fileName)
            if filterNode.localName == "Filter":
                filterName = _filterFolder
                if _filterFolder != "":
                    filterName += "\\\\"
                filterName += str(filterNode.attributes.getNamedItem("Name").nodeValue)

                addLine(")")
                addLine("SOURCE_GROUP(\"" + filterName + "\" FILES")
                #print str(filterName) + " atttr:" + filterName
                parseFilter(filterNode, filterName)
    #print ")"

def createFilesList(fileName):

    print "Converting" + fileName
    FILE = open(fileName.replace(".vcproj", ".list").replace("_v8", ""),"w")
    doc = get_a_document(fileName)

    headers.append("set (HEADER_FILES\n")
    source.append("set (SOURCE_FILES\n")

    for rootNode in doc.childNodes:
        for subNode in rootNode.childNodes:
            if subNode.nodeType == subNode.ELEMENT_NODE and subNode.localName == "Files":
                parseFilter(subNode, "")

    headers.append(")\n")
    source.append(")\n")
    #remove ")" at start and add at end
    lines.remove(")\n")
    lines.append(")\n")
    FILE.writelines(headers)
    FILE.writelines(source)
    FILE.writelines(lines)

    FILE.close()
    del headers[:]
    del source[:]
    del lines[:]

dir_src = '../'

for root, dirs, files in os.walk(dir_src):
  for name in files:
    f_src = os.path.join(root, name)
    if f_src.endswith('.vcproj') and not f_src.endswith('_v7.vcproj'):
      createFilesList(f_src)

print "Done"