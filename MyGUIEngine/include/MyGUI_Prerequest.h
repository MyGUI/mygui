/*!
	@file
	@author		Denis Koronchik
	@author		Georgiy Evmenov
	@author		Ну и я чуть чуть =)
	@date		09/2007
*/

#ifndef __MYGUI_PREREQUEST_H__
#define __MYGUI_PREREQUEST_H__

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
#		define _CRT_SECURE_NO_WARNINGS
#endif

#include "MyGUI_Platform.h"

// for OGRE_VERSION
#include <OgrePrerequisites.h>

// для полной информации о выделении памяти
#if OGRE_VERSION < ((1 << 16) | (6 << 8) | 0)

#   include <OgreMemoryManager.h>
#   define MYGUI_VALIDATE_PTR(ptr) assert(ptr == 0 || Ogre::MemoryManager::instance().validateAddr(ptr))

#   define OGRE_MALLOC(bytes, category) new unsigned char(bytes)
#   define OGRE_ALLOC_T(T, count, category) new T(count)
#   define OGRE_FREE(ptr, category) { delete ptr; ptr=0; }
 
#   define OGRE_NEW_T(T, category) new T
#   define OGRE_NEW_ARRAY_T(T, count, category) new T[count]
#   define OGRE_DELETE_T(ptr, T, category) { delete ptr; ptr=0; }
#   define OGRE_DELETE_ARRAY_T(ptr, T, count, category) { delete [] ptr; ptr=0; }

#else

	#define MYGUI_VALIDATE_PTR(ptr)

#endif

#include <string>
#include <list>
#include <set>
#include <map>
#include <vector>
#include <deque>
#include "MyGUI_Utility.h"
#include "MyGUI_Delegate.h"

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	class Gui;

	using MyGUI::delegates::newDelegate;

	class WidgetSkinInfo;
	class MaskPickInfo;
	class IWidgetCreator;

	// managers
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
	class DelegateManager;
	class LanguageManager;
	class ResourceManager;
	
	class IWidgetFactory;

	class DynLib;

	namespace factory
	{
		template <typename T> class BaseWidgetFactory;

		class WidgetFactory;
		class ButtonFactory;
		class WindowFactory;
		class ListFactory;
		class HScrollFactory;
		class VScrollFactory;
		class EditFactory;
		class ComboBoxFactory;
		class StaticTextFactory;
		class TabFactory;
		class TabItemFactory;
		class ProgressFactory;
		class ItemBoxFactory;
		class MultiListFactory;
		class StaticImageFactory;
		class MessageFactory;
		class RenderBoxFactory;
		class PopupMenuFactory;
		class MenuItemFactory;
		class MenuBarFactory;
		class ScrollViewFactory;
		class DDContainerFactory;
		class GridCtrlFactory;
	}

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
	class RenderBox;
	class MenuCtrl;
	class MenuItem;
	class PopupMenu;
	class MenuBar;
	class ScrollView;
	class DDContainer;
	class GridCtrl;

/*#if MYGUI_DEBUG_MODE == 1
	struct WidgetPtr
	{
	private:
		WidgetPtr() { }

	public:
		WidgetPtr(Widget * _widget) : widget(_widget) {}

		Widget * operator -> ()
		{
			return widget;
		}

		friend bool operator == (const Widget* & a, const Widget* & b) { return a == b; }
		friend bool operator != (const Widget* & a, const Widget* & b) { return !(a == b); }

		//friend bool operator == (const WidgetPtr & a, int b) { return int(a.widget) == b; }
		//friend bool operator != (const WidgetPtr & a, int b) { return !(a == b); }

		//friend bool operator == (int a, const WidgetPtr & b) { return b == a; }
		//friend bool operator != (int a, const WidgetPtr & b) { return !(b == a); }

		//friend bool operator == (const Widget* & a, const WidgetPtr & b) { return b == a; }
		//friend bool operator != (const Widget* & a, const WidgetPtr & b) { return !(b == a); }

		//friend bool operator == (const WidgetPtr & a, const WidgetPtr & b) { return a.widget == b.widget; }
		//friend bool operator != (const WidgetPtr & a, const WidgetPtr & b) { return !(a == b); }

		operator Widget * () { return widget; }

	private:
		Widget * widget;
	};
#else*/
	typedef Widget * WidgetPtr;
//#endif

	typedef Button * ButtonPtr;
	typedef Window * WindowPtr;
	typedef List * ListPtr;
	typedef HScroll * HScrollPtr;
	typedef VScroll * VScrollPtr;
	typedef Edit * EditPtr;
	typedef ComboBox * ComboBoxPtr;
	typedef StaticText * StaticTextPtr;
	typedef Tab * TabPtr;
	typedef TabItem * TabItemPtr;
	typedef Progress * ProgressPtr;
	typedef ItemBox * ItemBoxPtr;
	typedef MultiList * MultiListPtr;
	typedef StaticImage * StaticImagePtr;
	typedef Message * MessagePtr;
	typedef RenderBox * RenderBoxPtr;
	typedef MenuCtrl * MenuCtrlPtr;
	typedef MenuItem * MenuItemPtr;
	typedef PopupMenu * PopupMenuPtr;
	typedef MenuBar * MenuBarPtr;
	typedef ScrollView * ScrollViewPtr;
	typedef DDContainer * DDContainerPtr;
	typedef GridCtrl * GridCtrlPtr;

	typedef TabItem Sheet; // OBSOLETE
	typedef TabItem * SheetPtr; // OBSOLETE

	// Define version
    #define MYGUI_VERSION_MAJOR 2
    #define MYGUI_VERSION_MINOR 2
    #define MYGUI_VERSION_PATCH 0

    #define MYGUI_VERSION    ((MYGUI_VERSION_MAJOR << 16) | (MYGUI_VERSION_MINOR << 8) | MYGUI_VERSION_PATCH)

	// Disable warnings for MSVC compiler
#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC

// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
#   pragma warning (disable : 4786)

// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
#   pragma warning (disable : 4503)

// disable: "conversion from 'double' to 'float', possible loss of data
#   pragma warning (disable : 4244)

// disable: "truncation from 'double' to 'float'
#   pragma warning (disable : 4305)

// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#   pragma warning (disable : 4251)

// disable: "non dll-interface class used as base for dll-interface class"
// Happens when deriving from Singleton because bug in compiler ignores
// template export
#   pragma warning (disable : 4275)

// disable: "C++ Exception Specification ignored"
// This is because MSVC 6 did not implement all the C++ exception
// specifications in the ANSI C++ draft.
#   pragma warning( disable : 4290 )

// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs in VC7 for no justifiable reason on all
// #includes of Singleton
#   pragma warning( disable: 4661)

#endif

} // namespace MyGUI

#endif // __MYGUI_PREREQUEST_H__
