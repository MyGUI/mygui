/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_PREREQUEST_H_
#define MYGUI_PREREQUEST_H_

#include "MyGUI_Platform.h"

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
#	ifdef MYGUI_CHECK_MEMORY_LEAKS
#		define _CRTDBG_MAP_ALLOC
#		include <crtdbg.h>
#	endif
#endif

#define MYGUI_DEFINE_VERSION(major, minor, patch) ((major << 16) | (minor << 8) | patch)

#ifndef MYGUI_DONT_REPLACE_NULLPTR
#	if __cplusplus < 201103L && !defined(_NATIVE_NULLPTR_SUPPORTED)
#		define nullptr 0
#	endif
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
	class ListBox;
	class EditBox;
	class ComboBox;
	class TextBox;
	class TabControl;
	class TabItem;
	class ProgressBar;
	class ItemBox;
	class MultiListBox;
	class MultiListItem;
	class ImageBox;
	class MenuControl;
	class MenuItem;
	class PopupMenu;
	class MenuBar;
	class ScrollBar;
	class ScrollView;
	class DDContainer;
	class Canvas;

	// Define version
#define MYGUI_VERSION_MAJOR 3
#define MYGUI_VERSION_MINOR 2
#define MYGUI_VERSION_PATCH 3

#define MYGUI_VERSION    MYGUI_DEFINE_VERSION(MYGUI_VERSION_MAJOR, MYGUI_VERSION_MINOR, MYGUI_VERSION_PATCH)

	// Disable warnings for MSVC compiler
#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC

// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#	pragma warning (disable : 4251)

// also some warnings are disabled in CMake

#endif

} // namespace MyGUI

#include "MyGUI_DeprecatedTypes.h"

#endif // MYGUI_PREREQUEST_H_
