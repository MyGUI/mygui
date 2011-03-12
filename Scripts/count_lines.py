#!/usr/bin/env python
# python 2.6 required (for os.path.relpath)

import os, filecmp

def countLines(fileName):
  file = open(fileName, 'r')
  count = 0
  while file.readline() != "":
    count = count + 1
  file.close ()
  return count

def CountLinesInFolder(dir_src):
  numLines = 0

  for root, dirs, files in os.walk(dir_src):
    for name in files:
      if name.endswith('.h') or name.endswith('.cpp'):
          f_src = os.path.join(root, name)
          f_src = f_src.replace('\\','/')
          numLines = numLines + countLines(f_src)

  print "" + dir_src + " : " + str(numLines)
  return numLines

# ----------

i = 0
i += CountLinesInFolder('../MyGUIEngine')
i += CountLinesInFolder('../Demos')
i += CountLinesInFolder('../Tools')
i += CountLinesInFolder('../UnitTests')
i += CountLinesInFolder('../Common')
i += CountLinesInFolder('../Platforms/OpenGL')
i += CountLinesInFolder('../Platforms/OGRE')
i += CountLinesInFolder('../Platforms/DirectX')
i += CountLinesInFolder('../Plugins')
i += CountLinesInFolder('../Wrappers')
print "Total: "+ str(i)

raw_input("Press enter to continue.")