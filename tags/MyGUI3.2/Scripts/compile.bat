c:\python26\python ../../../pyinstaller/Makespec.py -X -F -o parseXML parseXML.py
c:\python26\python -O ../../../pyinstaller/Build.py parseXML\parseXML.spec
copy parseXML\dist\parseXML.exe parseXML.exe

c:\python26\python ../../../pyinstaller/Makespec.py -X -F -o prepareRelease prepareRelease.py
c:\python26\python -O ../../../pyinstaller/Build.py prepareRelease\prepareRelease.spec
copy prepareRelease\dist\prepareRelease.exe prepareRelease.exe
