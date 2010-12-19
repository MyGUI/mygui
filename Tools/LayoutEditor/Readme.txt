LayoutEditor

Save/Load:
  absolute names (C:/LayoutEditor/Media/my.layout)
  relative names (../Media/my.layout)

Controls:
  Keyboard:
    ESC                - exit or exit from test mode
    Ctrl + O, Ctrl + L - open file
    Ctrl + S           - save file
    Ctrl + Z           - undo
    Ctrl + Y           - redo
    Ctrl + T           - test layout
    Ctrl + R           - toggle relative coordinates for selected widget

    Keyboard arrows - move widget
    Shift           - omit grid when moving/resizing widget
    Delete          - delete selected widget and all child widgets
    Tab             - change selected sheet (for Tab widget only)

  Mouse:
    click on widget - select widget (click several times for selecting parent widgets)
    doubleclick on any widget in widget panel - create widget in center or inside selected widget if possible

Ignoreg properties:
Visible, AutoAlpha(Window) MinSize(Window) and MaxSize(Window) properties are ingnored in edit mode (but this properties will be saved and will work fine in test mode and your application).

Editor setting and values can be found in Tools/LayoutEditor/Settings folder
  For example if you want add you own skin in editor add your my_skin.skin file to Media/LayoutEditor/editor.xml
  and add <Property key="Skin" value="MySkin"/> to widget's xml node.

Items panel is used to add and edit List, ComboBox or Tab items. Press enter to edit selected item.

UserData panel used to add pairs of string (key and value) to any widget. It is used to store any needed info in widget.
Here are two examples:
  * you can add some info data to all widgets and in runtime you'll get it by std::string info = widget->getUserString(key); and show popup with info about widget.
  * you have 10 buttons and want to have one event handler for all. You can add button number into user data and read it in event handler to figure which button was pressed.
  