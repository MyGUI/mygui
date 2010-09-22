/*!
	@file
	@author		Denis Koronchik
	@author		Georgiy Evmenov
	@author		Ну и я чуть чуть =)
	@date		09/2007
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __MYGUI_PREREQUEST_H__
#define __MYGUI_PREREQUEST_H__

#include "MyGUI_Platform.h"

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
#	ifndef _CRT_SECURE_NO_WARNINGS
#		define _CRT_SECURE_NO_WARNINGS
#	endif
#	ifdef MYGUI_CHECK_MEMORY_LEAKS
#		define _CRTDBG_MAP_ALLOC
#		include <stdlib.h>
#		include <crtdbg.h>
#	endif
#endif

#define MYGUI_DEFINE_VERSION(major, minor, patch) ((major << 16) | (minor << 8) | patch)

#ifndef MYGUI_DONT_REPLACE_NULLPTR
	#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
		#ifndef _MANAGED
			#define nullptr 0
		#endif
	#else
		#define nullptr 0
	#endif
#endif

namespace MyGUI
{
	class Gui;
	class LogManager;
	class InputManager;
	class SubWidgetManager;
	class LayerManager;
	class SkinManager;
	class WidgetManager;
	class FontManager;
	class ControllerManager;
	class PointerManager;
	class ClipboardManager;
	class LayoutManager;
	class PluginManager;
	class DynLibManager;
	class LanguageManager;
	class ResourceManager;
	class RenderManager;
	class FactoryManager;
	class TextureManager;
	class ToolTipManager;

	class Widget;
	class Button;
	class Window;
	class List;
	class HScroll;
	class VScroll;
	class Edit;
	class ComboBox;
	class StaticText;
	class Tab;
	class TabItem;
	class Progress;
	class ItemBox;
	class MultiList;
	class StaticImage;
	class Message;
	class MenuCtrl;
	class MenuItem;
	class PopupMenu;
	class MenuBar;
	class ScrollView;
	class DDContainer;
	class Canvas;
	class ListCtrl;
	class ListBox;

	// Define version
	#define MYGUI_VERSION_MAJOR 3
	#define MYGUI_VERSION_MINOR 2
	#define MYGUI_VERSION_PATCH 0

	#define MYGUI_VERSION    MYGUI_DEFINE_VERSION(MYGUI_VERSION_MAJOR, MYGUI_VERSION_MINOR, MYGUI_VERSION_PATCH)

	// Disable warnings for MSVC compiler
#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC

// disable: warning C4512: '***' : assignment operator could not be generated
#	pragma warning (disable : 4512)

// disable: warning C4127: conditional expression is constant
#	pragma warning (disable : 4127)

// disable: warning C4100: '***' : unreferenced formal parameter
#	pragma warning (disable : 4100)

// disable: warning C4275: non dll-interface class '***' used as base for dll-interface clas '***'
#	pragma warning (disable : 4275)

// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#	pragma warning (disable : 4251)

// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs for no justifiable reason on all
// #includes of Singleton
#	pragma warning( disable: 4661)

#endif

} // namespace MyGUI

#include "MyGUI_DeprecatedTypes.h"

#endif // __MYGUI_PREREQUEST_H__
