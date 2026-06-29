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
#include "MyGUI_WidgetContainer.h"
#include "MyGUI_Widget.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	using EventHandle_FrameEventDelegate = delegates::MultiDelegate<float>;

	class MYGUI_EXPORT Gui : public WidgetContainer, public IUnlinkWidget, public MemberObsolete<Gui>
	{
		MYGUI_SINGLETON_DECLARATION(Gui);
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
		Widget* createWidgetT(
			std::string_view _type,
			std::string_view _skin,
			const IntCoord& _coord,
			Align _align,
			std::string_view _layer,
			std::string_view _name = {});
		/** See Gui::createWidgetT */
		Widget* createWidgetT(
			std::string_view _type,
			std::string_view _skin,
			int _left,
			int _top,
			int _width,
			int _height,
			Align _align,
			std::string_view _layer,
			std::string_view _name = {});
		/** Create widget using coordinates relative to parent widget. see Gui::createWidgetT */
		Widget* createWidgetRealT(
			std::string_view _type,
			std::string_view _skin,
			const FloatCoord& _coord,
			Align _align,
			std::string_view _layer,
			std::string_view _name = {});
		/** Create widget using coordinates relative to parent. see Gui::createWidgetT */
		Widget* createWidgetRealT(
			std::string_view _type,
			std::string_view _skin,
			float _left,
			float _top,
			float _width,
			float _height,
			Align _align,
			std::string_view _layer,
			std::string_view _name = {});

		// templates for creating widgets by type
		/** Same as Gui::createWidgetT but return T* instead of Widget* */
		template<typename T>
		T* createWidget(
			std::string_view _skin,
			const IntCoord& _coord,
			Align _align,
			std::string_view _layer,
			std::string_view _name = {})
		{
			return static_cast<T*>(createWidgetT(T::getClassTypeName(), _skin, _coord, _align, _layer, _name));
		}
		/** Same as Gui::createWidgetT but return T* instead of Widget* */
		template<typename T>
		T* createWidget(
			std::string_view _skin,
			int _left,
			int _top,
			int _width,
			int _height,
			Align _align,
			std::string_view _layer,
			std::string_view _name = {})
		{
			return static_cast<T*>(createWidgetT(
				T::getClassTypeName(),
				_skin,
				IntCoord(_left, _top, _width, _height),
				_align,
				_layer,
				_name));
		}
		/** Same as Gui::createWidgetRealT but return T* instead of Widget* */
		template<typename T>
		T* createWidgetReal(
			std::string_view _skin,
			const FloatCoord& _coord,
			Align _align,
			std::string_view _layer,
			std::string_view _name = {})
		{
			return static_cast<T*>(createWidgetRealT(T::getClassTypeName(), _skin, _coord, _align, _layer, _name));
		}
		/** Same as Gui::createWidgetRealT but return T* instead of Widget* */
		template<typename T>
		T* createWidgetReal(
			std::string_view _skin,
			float _left,
			float _top,
			float _width,
			float _height,
			Align _align,
			std::string_view _layer,
			std::string_view _name = {})
		{
			return static_cast<T*>(
				createWidgetRealT(T::getClassTypeName(), _skin, _left, _top, _width, _height, _align, _layer, _name));
		}

		/** Destroy any created widget */
		void destroyWidget(Widget* _widget);

		/** Destroy vector of widgets */
		void destroyWidgets(const VectorWidgetPtr& _widgets);

#ifndef MYGUI_DONT_USE_OBSOLETE
		using MemberObsolete<Gui>::destroyWidgets;
		using MemberObsolete<Gui>::findWidgetT;
		using MemberObsolete<Gui>::findWidget;
#endif

		/** Find widget by name
			If widget is not found the exception will be thrown, or if the second parameter is false the nullptr pointer will be returned
		*/
		Widget* findWidgetT(std::string_view _name, bool _throw = true) const;

		/** Find widget by name and cast it to T type.
			If widget not found or T and found widget have different types exception will be thrown, or if the second parameter is false the nullptr pointer will be returned
		*/
		template<typename T>
		T* findWidget(std::string_view _name, bool _throw = true) const
		{
			Widget* widget = findWidgetT(_name, _throw);
			if (nullptr == widget)
				return nullptr;
			return widget->castType<T>(_throw);
		}

		/** Destroy child widget or throw exception if this child widget not found */
		void destroyChildWidget(Widget* _widget);

		/** Destroy all child widgets */
		void destroyAllChildWidget();

		/** Get root widgets */
		const VectorWidgetPtr& getRootWidgets() const;

		/** Inject frame entered event (called be renderer, do not call it manually).
			This function is called every frame by renderer.
		*/
		void frameEvent(float _time) const;

		/*events:*/
		/** Event : Multidelegate. GUI per frame call.\n
			signature : void method(float _time)\n
			@param _time Time elapsed since last frame
		*/
		EventHandle_FrameEventDelegate eventFrameStart;

	private:
		void _unlinkWidget(Widget* _widget) override;

	private:
		// GUI manager singletons
		InputManager* mInputManager{nullptr};
		SubWidgetManager* mSubWidgetManager{nullptr};
		LayerManager* mLayerManager{nullptr};
		SkinManager* mSkinManager{nullptr};
		WidgetManager* mWidgetManager{nullptr};
		FontManager* mFontManager{nullptr};
		ControllerManager* mControllerManager{nullptr};
		PointerManager* mPointerManager{nullptr};
		ClipboardManager* mClipboardManager{nullptr};
		LayoutManager* mLayoutManager{nullptr};
		DynLibManager* mDynLibManager{nullptr};
		PluginManager* mPluginManager{nullptr};
		LanguageManager* mLanguageManager{nullptr};
		ResourceManager* mResourceManager{nullptr};
		FactoryManager* mFactoryManager{nullptr};
		ToolTipManager* mToolTipManager{nullptr};

		bool mIsInitialise{false};
	};

} // namespace MyGUI

#endif // MYGUI_GUI_H_
