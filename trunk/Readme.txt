Change [Configuration properties->Debugging->working directory] of TestApp and LayoutEditor to 
../Shared/$(ConfigurationName)
if you want run projects from Visual Studio.

In linux you can now build by typing:

sh bootstrap
./configure
make
make install

For Code::Blocs users there are MyGUIEngine/MyGUIEngin.cbp and LayoutEditor/LayoutEditor.cbp
But be careful this files can be outdated because of no one from MyGUI developers use Code::Blocs.
Before compile run "Add files recursively" from dir "src" and copy dir freetype2 (sf.net) to "include"
dir. For linking use freetype6.dll.