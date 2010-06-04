import os

os.chdir("Scripts/vera++")
os.system("vera++ ../../MyGUIEngine/src/*.cpp -profile default")
os.system("vera++ ../../MyGUIEngine/include/*.h -profile default")
os.chdir("../..")
