VisualStudio 2005 and 2008:
	Open MyGUI_v8.sln. If you use VisualStudio 2008 solution and projects will be converted automatically.

VisualStudio 2003:
	There are MyGUI_v7.sln but it can be outdated. You can create working solution and projects by editing _v8 files and changing lines 
	  Microsoft Visual Studio Solution File, Format Version 9.00
	  # Visual Studio 2005
	in solution to
	  Microsoft Visual Studio Solution File, Format Version 8.00
	in solution and
	  Version="8,00"
	to
	  Version="7.10"
	in vcproj files.

In linux:
	  sh bootstrap
	  ./configure
	  make
	  make install

	You also can precompile MyGUI_Precompiled.h and Common/precompiled.h (with same flags as cpp files) to speed up compilation.
	Then copy binaries of demos and LayoutEditor to Shared/linux/Release and run them.

Code::Blocks:
	For Code::Blocs users there are MyGUIEngine/MyGUIEngin.cbp and LayoutEditor/LayoutEditor.cbp
	But be careful this files can be outdated because of no one from MyGUI developers use Code::Blocs.
	Before compile run "Add files recursively" from dir "src" and copy dir freetype2 (sf.net) to "include"
	dir. For linking use freetype6.dll.
