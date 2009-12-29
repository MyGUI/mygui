c:\python26\python ../../../pyinstaller/Makespec.py -X -F -o parseXML parseXML.py
c:\python26\python -O ../../../pyinstaller/Build.py parseXML\parseXML.spec >> pyinstaller_log.txt
copy parseXML\dist\parseXML.exe parseXML.exe

c:\python26\python ../../../pyinstaller/Makespec.py -X -F -o prepareRelease prepareRelease.py
c:\python26\python -O ../../../pyinstaller/Build.py prepareRelease\prepareRelease.spec >> pyinstaller_log.txt
copy prepareRelease\dist\prepareRelease.exe prepareRelease.exe
