LayoutEditor v1.0 RC1

Save/Load:
  absolute names (C:/LayoutEditor/Media/my.layout)
  relative names (../Media/my.layout)
  names from Ogre FileSystem (my.layout)
  save/load any *.layout file in Ogre FileSystem through drop list in save/load menu.

Controls:
  Keyboard:
    Ctrl + O, Ctrl + L - open file
    Ctrl + S           - save file
    Ctrl + Z           - undo
    Ctrl + Y           - redo
	Ctrl + R           - toggle relative coordinates

    Keyboard arrows - move widget
    Shift           - omit grid when moving/resizing widget
	Delete          - delete selected widget and all child widgets
    Tab             - change Sheet (for Tab widget only)

  Mouse:	
    doubleclick on any widget in widget panel - create widget in center
    doubleclick on selected Tab - add Sheet

Hidden widgets can be selected only through combo with all widgets.
You can't move or delete hidden widget.
	
Ignoreg properties:
"Message_Modal", "Window_AutoAlpha" and "Window_Snap" ingnored in edit mode (but this properties will be saved and will work fine in test mode and your application).

All possible values can be found in Media/LayoutEditor/widgets.xml
  For example if you want add you own skin in editor add your my_skin.skin file to Media/LayoutEditor/editor.xml
  and add <Property key="Skin" value="MySkin"/> to appropriate widget.