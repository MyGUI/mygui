/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_WIDGET_H__
#define __MYGUI_WIDGET_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Any.h"
#include "MyGUI_ICroppedRectangle.h"
#include "MyGUI_WidgetUserData.h"
#include "MyGUI_WidgetInput.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_ResourceLayout.h"
#include "MyGUI_IObject.h"
#include "MyGUI_SkinItem.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	typedef delegates::CMultiDelegate3<Widget*, const std::string&, const std::string&> EventHandle_WidgetStringString;

	/** \brief @wpage{Widget}
		Widget widget description should be here.
	*/
	class MYGUI_EXPORT Widget :
		public IObject,
		public ICroppedRectangle,
		public UserData,
		public WidgetInput,
		public delegates::IDelegateUnlink,
		public SkinItem,
		public MemberObsolete<Widget>
	{
		// для вызова закрытых деструкторов
		friend class WidgetManager;

		MYGUI_RTTI_DERIVED( Widget )

	public:
		Widget();

		/** Create child widget
			@param _type widget type
			@param _skin widget skin
			@param _coord int coordinates of widget (_left, _top, _width, _height)
			@param _align widget align (possible values can be found in enum Align)
			@param _name if needed (you can use it for finding widget by name later)
		*/
		Widget* createWidgetT(const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _name = "");

		/** See Widget::createWidgetT(const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _name = "") */
		Widget* createWidgetT(const std::string& _type, const std::string& _skin, int _left, int _top, int _width, int _height, Align _align, const std::string& _name = "");

		/** Create widget using coordinates relative to parent. see Widget::createWidgetT(const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _name = "") */
		Widget* createWidgetRealT(const std::string& _type, const std::string& _skin, const FloatCoord& _coord, Align _align, const std::string& _name = "");

		/** Create widget using coordinates relative to parent. see Widget::createWidgetT(const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _name = "") */
		Widget* createWidgetRealT(const std::string& _type, const std::string& _skin, float _left, float _top, float _width, float _height, Align _align, const std::string& _name = "");

		// templates for creating widgets by type
		/** Same as Widget::createWidgetT but return T pointer instead of Widget* */
		template <typename T>
		T* createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _name = "")
		{
			return static_cast<T*>(createWidgetT(T::getClassTypeName(), _skin, _coord, _align, _name));
		}

		/** Same as Widget::createWidgetT but return T pointer instead of Widget* */
		template <typename T>
		T* createWidget(const std::string& _skin, int _left, int _top, int _width, int _height, Align _align, const std::string& _name = "")
		{
			return static_cast<T*>(createWidgetT(T::getClassTypeName(), _skin, IntCoord(_left, _top, _width, _height), _align, _name));
		}

		/** Same as Widget::createWidgetRealT but return T* instead of Widget* */
		template <typename T>
		T* createWidgetReal(const std::string& _skin, const FloatCoord& _coord, Align _align, const std::string& _name = "")
		{
			return static_cast<T*>(createWidgetRealT(T::getClassTypeName(), _skin, _coord, _align, _name));
		}

		/** Same as Widget::createWidgetRealT but return T* instead of Widget* */
		template <typename T>
		T* createWidgetReal(const std::string& _skin, float _left, float _top, float _width, float _height, Align _align, const std::string& _name = "")
		{
			return static_cast<T*>(createWidgetRealT(T::getClassTypeName(), _skin, _left, _top, _width, _height, _align, _name));
		}

		/** Create child widget
			@param _style Child, Popup or Overlapped widget style
			@param _type widget type
			@param _skin widget skin
			@param _coord int coordinates of widget (_left, _top, _width, _height)
			@param _align widget align (possible values can be found in enum Align)
			@param _layer layer where widget will be created (all layers usually defined in core_layer.xml file).
			@param _name optional widget name (you can use it for finding widget by name later)
		*/
		Widget* createWidgetT(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer = "", const std::string& _name = "");

		/** Same as Widget::createWidgetT but return T* instead of Widget* */
		template <typename T>
		T* createWidget(WidgetStyle _style, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer = "", const std::string& _name = "")
		{
			return static_cast<T*>(createWidgetT(_style, T::getClassTypeName(), _skin, _coord, _align, _layer, _name));
		}

		/** Set widget position (position of left top corner) */
		virtual void setPosition(const IntPoint& _value);
		/** Set widget size */
		virtual void setSize(const IntSize& _value);
		/** Set widget position and size */
		virtual void setCoord(const IntCoord& _value);

		/** See Widget::setPosition(const IntPoint& _pos) */
		void setPosition(int _left, int _top);
		/** See Widget::setSize(const IntSize& _size) */
		void setSize(int _width, int _height);
		/** See Widget::setCoord(const IntCoord& _coord) */
		void setCoord(int _left, int _top, int _width, int _height);

		/** Set widget position (position of left top corner)*/
		void setRealPosition(const FloatPoint& _value);
		/** Set widget size */
		void setRealSize(const FloatSize& _value);
		/** Set widget position and size*/
		void setRealCoord(const FloatCoord& _value);

		/** See Widget::setRealPosition(const FloatPoint& _point) */
		void setRealPosition(float _left, float _top);
		/** See Widget::setRealSize(const FloatSize& _size) */
		void setRealSize(float _width, float _height);
		/** See Widget::setRealPosition(const FloatCoord& _coord) */
		void setRealCoord(float _left, float _top, float _width, float _height);

		//! Get name of widget
		const std::string& getName() const;

		/** Hide or show widget */
		virtual void setVisible(bool _value);
		/** Return true if visible */
		bool getVisible() const;

		/** Return widget's visibility based on it's and parents visibility. */
		bool getInheritedVisible() const;

		/** Set align */
		virtual void setAlign(Align _value);
		/** Get align */
		Align getAlign() const;

		/** Set widget opacity */
		void setAlpha(float _value);
		/** Get widget opacity */
		float getAlpha() const;

		/** Enable or disable inherits alpha mode.\n
			Inherits alpha mode: when enabled widget alpha is it's own
			alpha value multiplied by parent's real alpha (that depend
			on parent's parent and so on).\n
			When disabled widget's alpha doesn't depend on parent's alpha.
			So this is used when you need things like semi-transparent
			window with non-transparent text on it and window's alpha
			changes.\n
			Enabled (true) by default.
		*/
		void setInheritsAlpha(bool _value);
		/** Get inherits alpha mode flag */
		bool getInheritsAlpha() const;

		void setColour(const Colour& _value);

		// являемся ли мы рутовым виджетом
		/** Is this widget is root widget (root == without parents) */
		bool isRootWidget() const;

		/** Get parent widget or nullptr if no parent */
		Widget* getParent() const;

		IntSize getParentSize() const;

		/** Get child widgets Enumerator */
		EnumeratorWidgetPtr getEnumerator() const;

		/** Get child count */
		size_t getChildCount();

		/** Get child by index (index from 0 to child_count - 1) */
		Widget* getChildAt(size_t _index);

		/** Find widget by name.
			Search recursively through all childs starting from this widget.
			@return Return first found widget with given name
		*/
		Widget* findWidget(const std::string& _name);

		/** Find all widgets with given name and add them into _result.
			Search recursively through all childs starting from this widget.
		*/
		void findWidgets(const std::string& _name, VectorWidgetPtr& _result);

		/** Enable or disable widget */
		virtual void setEnabled(bool _value);
		/** Enable or disable widget without changing widget's state */
		void setEnabledSilent(bool _value);
		/** Is widget enabled */
		bool getEnabled() const;

		/** Is widget enabled and all it's parents in hierarchy are enabled. */
		bool getInheritedEnabled() const;

		/** Get rectangle where child widgets placed */
		IntCoord getClientCoord();

		/** Get client area widget or nullptr if widget don't have client */
		Widget* getClientWidget();

		/** Detach widget from widgets hierarchy
			@param _layer Attach to specified layer (if any)
		*/
		void detachFromWidget(const std::string& _layer = "");

		/** Attach widget to parent
			@param _parent New parent
			@param _style New widget style (see WidgetStyle::Enum)
			@param _layer Attach to specified layer (if any)
		*/
		void attachToWidget(Widget* _parent, WidgetStyle _style = WidgetStyle::Child, const std::string& _layer = "");

		/** Change widget skin */
		void changeWidgetSkin(const std::string& _skinName);

		/** Set widget style.
			@param _style New widget style (see WidgetStyle::Enum)
			@param _layer Attach to specified layer (if any)
			@note When choosing WidgetStyle::Popup style you also need attach widget to layer
		*/
		void setWidgetStyle(WidgetStyle _style, const std::string& _layer = "");
		/** Get widget style */
		WidgetStyle getWidgetStyle() const;

		/** Set any widget property
			@param _key Property name (for example Alpha or Enabled)
			@param _value Value converted to string
		*/
		void setProperty(const std::string& _key, const std::string& _value);


		/** Event : Widget property changed through setProperty (in code, or from layout)\n
			signature : void method(MyGUI::Widget* _sender, const std::string& _key, const std::string& _value);
			@param _sender widget that called this event
			@param _key
			@param _value
		*/
		EventHandle_WidgetStringString eventChangeProperty;

		/** Event : Widget coordinate changed (widget was resized or moved).\n
			signature : void method(MyGUI::Widget* _sender)
			@param _sender widget that called this event
		*/
		EventHandle_WidgetVoid eventChangeCoord;

		/*internal:*/
		// метод для запроса номера айтема и контейнера
		virtual size_t _getItemIndex(Widget* _item);

		// дает приоритет виджету при пиккинге
		void _forcePick(Widget* _widget);

		void _initialise(WidgetStyle _style, const IntCoord& _coord, const std::string& _skinName, Widget* _parent, ICroppedRectangle* _croppedParent, const std::string& _name);
		void _shutdown();

		// удяляет неудачника
		void _destroyChildWidget(Widget* _widget);

		void _setContainer(Widget* _value);
		Widget* _getContainer();

		void _setAlign(const IntSize& _oldsize, const IntSize& _newSize);
		bool _checkPoint(int _left, int _top) const;

		Widget* _createSkinWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer = "", const std::string& _name = "");

		// сброс всех данных контейнера, тултипы и все остальное
		virtual void _resetContainer(bool _update);

		bool _setWidgetState(const std::string& _value);

	protected:
		// все создание только через фабрику
		virtual ~Widget();

		virtual void shutdownOverride();
		virtual void initialiseOverride();

		void _updateView(); // обновления себя и детей

		// создает виджет
		Widget* baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name, bool _template);

		// удаляет всех детей
		void _destroyAllChildWidget();

		// запрашиваем у конейтера айтем по позиции мыши
		virtual size_t _getContainerIndex(const IntPoint& _point);

		virtual void baseUpdateEnable();

		// наследуемся он LayerInfo
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) const;
		virtual const IntCoord& getLayerItemCoord() const;

		template <typename T>
		void assignWidget(T*& _widget, const std::string& _name)
		{
			_widget = nullptr;
			for (VectorWidgetPtr::iterator iter = mWidgetChildSkin.begin(); iter != mWidgetChildSkin.end(); ++iter)
			{
				Widget* find = (*iter)->findWidget(_name);
				if (nullptr != find)
				{
					_widget = find->castType<T>(false);
					break;
				}
			}
		}

		VectorWidgetPtr getSkinWidgetsByName(const std::string& _name);

		void destroySkinWidget(Widget* _widget);

		virtual void onWidgetCreated(Widget* _widget);
		virtual void onWidgetDestroy(Widget* _widget);

		void setWidgetClient(Widget* _widget);

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		void frameEntered(float _frame);

		const WidgetInfo* initialiseWidgetSkinBase(ResourceSkin* _info, ResourceLayout* _templateInfo);
		void shutdownWidgetSkinBase();

		void _updateAlpha();
		void _updateAbsolutePoint();

		// для внутреннего использования
		void _updateVisible();

		void _updateEnabled();

		float _getRealAlpha() const;

		void _createChildSkinWidget(ResourceSkin* _info);
		void _destroyChildSkinWidget();

		void _parseSkinProperties(ResourceSkin* _info);
		void _checkInheristProperties();

		void _linkChildWidget(Widget* _widget);
		void _unlinkChildWidget(Widget* _widget);

		void setSkinProperty(ResourceSkin* _info);

		virtual void resizeLayerItemView(const IntSize& _oldView, const IntSize& _newView);

	private:
		// клиентская зона окна
		// если виджет имеет пользовательские окна не в себе
		// то обязательно проинициализировать Client
		Widget* mWidgetClient;

		// вектор всех детей виджетов
		VectorWidgetPtr mWidgetChild;

		// вектор детей скина
		VectorWidgetPtr mWidgetChildSkin;

		// доступен ли на виджет
		bool mEnabled;
		bool mInheritsEnabled;
		// для иерархического скрытия
		bool mInheritsVisible;
		// прозрачность и флаг наследования альфы нашего оверлея
		float mAlpha;
		float mRealAlpha;
		bool mInheritsAlpha;
		// имя виджета
		std::string mName;

		// наш отец в иерархии виджетов
		Widget* mParent;

		// поведение виджета, перекрывающийся дочерний или всплывающий
		WidgetStyle mWidgetStyle;

		Widget* mContainer;

		Align mAlign;
		bool mVisible;
	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_H__
