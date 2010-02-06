# run from root sources directory: python Scripts/cppcheck/cppcheck.py

import os

os.system("cppcheck --enable=all -I . -I MyGUIEngine/include MyGUIEngine/src")
