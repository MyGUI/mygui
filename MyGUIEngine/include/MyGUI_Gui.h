/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_GUI_H_
#define MYGUI_GUI_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IUnlinkWidget.h"
#include "MyGUI_Widget.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	typedef delegates::CMultiDelegate1<float> EventHandle_FrameEventDelegate;

	class MYGUI_EXPORT Gui :
		public Singleton<Gui>,
		public IUnlinkWidget,
		public MemberObsolete<Gui>
	{
		friend class WidgetManager;

	public:
		Gui();

		/** Initialise GUI and all GUI Managers
			@param
				_core name of core config file for MyGUI (contain main config files with skins, layers, fonts, etc.)
			@param
				_logFileName Log file name
		*/
		void initialise(const std::string& _core = "MyGUI_Core.xml");

#ifndef MYGUI_DONT_USE_OBSOLETE
		MYGUI_OBSOLETE(" is deprecated, use : void Gui::initialise(const std::string& _core) and set log filename in Platform")
		void initialise(const std::string& _core, const std::string& _logFileName);
#endif // MYGUI_DONT_USE_OBSOLETE

		/** Shutdown GUI and all GUI Managers*/
		void shutdown();

		// methods for creating widgets
		/** Create widget
			@param _type widget type
			@param _skin widget skin
			@param _coord int coordinates of widget (_left, _top, _width, _height)
			@param _align widget align (possible values can be found in enum Align)
			@param _layer layer where widget will be created (all layers usually defined in core_layer.xml file).
			@param _name optional widget name (you can use it for finding widget by name later)
		*/
		Widget* createWidgetT(const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name = "");
		/** See Gui::createWidgetT */
		Widget* createWidgetT(const std::string& _type, const std::string& _skin, int _left, int _top, int _width, int _height, Align _align, const std::string& _layer, const std::string& _name = "");
		/** Create widget using coordinates relative to parent widget. see Gui::createWidgetT */
		Widget* createWidgetRealT(const std::string& _type, const std::string& _skin, const FloatCoord& _coord, Align _align, const std::string& _layer, const std::string& _name = "");
		/** Create widget using coordinates relative to parent. see Gui::createWidgetT */
		Widget* createWidgetRealT(const std::string& _type, const std::string& _skin, float _left, float _top, float _width, float _height, Align _align, const std::string& _layer, const std::string& _name = "");

		// templates for creating widgets by type
		/** Same as Gui::createWidgetT but return T* instead of Widget* */
		template <typename T>
		T* createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name = "")
		{
			return static_cast<T*>(createWidgetT(T::getClassTypeName(), _skin, _coord, _align, _layer, _name));
		}
		/** Same as Gui::createWidgetT but return T* instead of Widget* */
		template <typename T>
		T* createWidget(const std::string& _skin, int _left, int _top, int _width, int _height, Align _align, const std::string& _layer, const std::string& _name = "")
		{
			return static_cast<T*>(createWidgetT(T::getClassTypeName(), _skin, IntCoord(_left, _top, _width, _height), _align, _layer, _name));
		}
		/** Same as Gui::createWidgetRealT but return T* instead of Widget* */
		template <typename T>
		T* createWidgetReal(const std::string& _skin, const FloatCoord& _coord, Align _align, const std::string& _layer, const std::string& _name = "")
		{
			return static_cast<T*>(createWidgetRealT(T::getClassTypeName(), _skin, _coord, _align, _layer, _name));
		}
		/** Same as Gui::createWidgetRealT but return T* instead of Widget* */
		template <typename T>
		T* createWidgetReal(const std::string& _skin, float _left, float _top, float _width, float _height, Align _align, const std::string& _layer, const std::string& _name = "")
		{
			return static_cast<T*>(createWidgetRealT(T::getClassTypeName(), _skin, _left, _top, _width, _height, _align, _layer, _name));
		}

		/** Destroy any created widget */
		void destroyWidget(Widget* _widget);

		/** Destroy vector of widgets */
		void destroyWidgets(const VectorWidgetPtr& _widgets);

		/** Destroy Enumerator of widgets */
		void destroyWidgets(EnumeratorWidgetPtr& _widgets);

		/** Find widget by name
			If widget is not found the exception will be thrown, or if the second parameter is false the nullptr pointer will be returned
		*/
		Widget* findWidgetT(const std::string& _name, bool _throw = true);

		/** Find widget by name and prefix
			If widget is not found the exception will be thrown, or if the second parameter is false the nullptr pointer will be returned
		*/
		Widget* findWidgetT(const std::string& _name, const std::string& _prefix, bool _throw = true);

		/** Find widget by name and cast it to T type.
			If widget not found or T and found widget have different types exception will be thrown, or if the second parameter is false the nullptr pointer will be returned
		*/
		template <typename T>
		T* findWidget(const std::string& _name, bool _throw = true)
		{
			Widget* widget = findWidgetT(_name, _throw);
			if (nullptr == widget) return nullptr;
			return widget->castType<T>(_throw);
		}

		/** Find widget by name and prefix and cast it to T type
			If widget not found or T and found widget have different types cause exception, or if the second parameter is false the nullptr pointer will be returned
		*/
		template <typename T>
		T* findWidget(const std::string& _name, const std::string& _prefix, bool _throw = true)
		{
			return findWidget<T>(_prefix + _name, _throw);
		}

		/** Destroy child widget or throw exception if this child widget not found */
		void destroyChildWidget(Widget* _widget);

		/** Destroy all child widgets */
		void destroyAllChildWidget();

		/** Get root widgets Enumerator */
		EnumeratorWidgetPtr getEnumerator() const;

		/** Inject frame entered event (called be renderer, do not call it manually).
			This function is called every frame by renderer.
		*/
		void frameEvent(float _time);

		/*events:*/
		/** Event : Multidelegate. GUI per frame call.\n
			signature : void method(float _time)\n
			@param _time Time elapsed since last frame
		*/
		EventHandle_FrameEventDelegate eventFrameStart;

		/*internal:*/
		void _linkChildWidget(Widget* _widget);
		void _unlinkChildWidget(Widget* _widget);

	private:
		// создает виджет
		Widget* baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name);

		// удяляет неудачника
		void _destroyChildWidget(Widget* _widget);

		// удаляет всех детей
		void _destroyAllChildWidget();

		virtual void _unlinkWidget(Widget* _widget);

	private:
		// вектор всех детей виджетов
		VectorWidgetPtr mWidgetChild;

		// синглтоны гуя
		InputManager* mInputManager;
		SubWidgetManager* mSubWidgetManager;
		LayerManager* mLayerManager;
		SkinManager* mSkinManager;
		WidgetManager* mWidgetManager;
		FontManager* mFontManager;
		ControllerManager* mControllerManager;
		PointerManager* mPointerManager;
		ClipboardManager* mClipboardManager;
		LayoutManager* mLayoutManager;
		DynLibManager* mDynLibManager;
		PluginManager* mPluginManager;
		LanguageManager* mLanguageManager;
		ResourceManager* mResourceManager;
		FactoryManager* mFactoryManager;
		ToolTipManager* mToolTipManager;

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // MYGUI_GUI_H_
