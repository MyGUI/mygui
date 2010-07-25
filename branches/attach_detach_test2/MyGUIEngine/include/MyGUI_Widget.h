/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#ifndef __MYGUI_WIDGET_H__
#define __MYGUI_WIDGET_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Any.h"
#include "MyGUI_ICroppedRectangle.h"
#include "MyGUI_WidgetUserData.h"
#include "MyGUI_WidgetInput.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_IObject.h"
#include "MyGUI_SkinItem.h"

namespace MyGUI
{

	typedef delegates::CDelegate3<Widget*, const std::string&, const std::string&> EventHandle_WidgetStringString;

	class MYGUI_EXPORT Widget :
		public ICroppedRectangle,
		public UserData,
		public WidgetInput,
		public delegates::IDelegateUnlink,
		public SkinItem
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
			@param _name if needed (you can use it for finding widget by name later)
		*/
		Widget* createWidgetT(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer = "", const std::string& _name = "");

		/** Same as Widget::createWidgetT but return T* instead of Widget* */
		template <typename T>
		T* createWidget(WidgetStyle _style, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer = "", const std::string& _name = "")
		{
			return static_cast<T*>(createWidgetT(_style, T::getClassTypeName(), _skin, _coord, _align, _layer, _name));
		}

		/** Get parent widget or nullptr if no parent */
		Widget* getParent() { return mParent; }

		IntSize getParentSize();

		/** Get child widgets Enumerator */
		EnumeratorWidgetPtr getEnumerator();

		/** Get child count */
		size_t getChildCount();

		/** Get child by index (index from 0 to child_count - 1) */
		Widget* getChildAt(size_t _index);

		/** Find widget by name (search recursively through all childs starting from this widget) */
		Widget* findWidget(const std::string& _name);

		/** Destroy child widget or throw exception if this child widget not found */
		void destroyChildWidget(Widget* _widget);

		/** Destroy all child widgets */
		void destroyAllChildWidget();

		/** Set widget position (position of left top corner) */
		virtual void setPosition(const IntPoint& _value);
		/** Set widget size */
		virtual void setSize(const IntSize& _value);
		/** Set widget position and size */
		virtual void setCoord(const IntCoord& _value);

		/** See Widget::setPosition(const IntPoint& _pos) */
		void setPosition(int _left, int _top) { setPosition(IntPoint(_left, _top)); }
		/** See Widget::setSize(const IntSize& _size) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** See Widget::setCoord(const IntCoord& _coord) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

		/** Set widget position (position of left top corner)*/
		void setRealPosition(const FloatPoint& _value);
		/** Set widget size */
		void setRealSize(const FloatSize& _value);
		/** Set widget position and size*/
		void setRealCoord(const FloatCoord& _value);

		/** See Widget::setRealPosition(const FloatPoint& _point) */
		void setRealPosition(float _left, float _top) { setRealPosition(FloatPoint(_left, _top)); }
		/** See Widget::setRealSize(const FloatSize& _size) */
		void setRealSize(float _width, float _height) { setRealSize(FloatSize(_width, _height)); }
		/** See Widget::setRealPosition(const FloatCoord& _coord) */
		void setRealCoord(float _left, float _top, float _width, float _height) { setRealCoord(FloatCoord(_left, _top, _width, _height)); }

		/** Get position in screen coordinates */
		const IntPoint& getAbsolutePosition() const { return mAbsolutePosition; }
		/** Get rectangle in screen coordinates */
		IntRect getAbsoluteRect() const { return IntRect(mAbsolutePosition.left, mAbsolutePosition.top, mAbsolutePosition.left+mCoord.width, mAbsolutePosition.top+mCoord.height); }
		/** Get coordinate in screen coordinates */
		IntCoord getAbsoluteCoord() const { return IntCoord(mAbsolutePosition.left, mAbsolutePosition.top, mCoord.width, mCoord.height); }

		/** Get X in screen coordinates */
		int getAbsoluteLeft() const { return mAbsolutePosition.left; }
		/** Get Y in screen coordinates */
		int getAbsoluteTop() const { return mAbsolutePosition.top; }

		//! Set name of widget
		void setName(const std::string& _value) { mName = _value; }
		//! Get name of widget
		const std::string& getName() const { return mName; }

		/** Hide or show widget */
		virtual void setVisible(bool _value);
		/** Return true if visible */
		bool getVisible() const { return mVisible; }

		/** Set align */
		virtual void setAlign(Align _value);
		/** Get align */
		Align getAlign() const { return mAlign; }

		/** Set widget opacity */
		void setAlpha(float _value);
		/** Get widget opacity */
		float getAlpha() const { return mAlpha; }

		/** Enable or disable inherits alpha mode.\n
			Inherits alpha mode: when enabled widget alpha it it's own
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
		bool getInheritsAlpha()  const { return mInheritsAlpha; }

		/** Enable or disable widget */
		virtual void setEnabled(bool _value);
		/** Enable or disable widget without changing widget's state */
		void setEnabledSilent(bool _value) { mEnabled = _value; }
		/** Is widget enabled */
		bool getEnabled() const { return mEnabled; }

		/** Get rect where child widgets placed */
		IntCoord getClientCoord();

		/** Get clien area widget or nullptr if widget don't have client */
		Widget* getClientWidget() { return mWidgetClient; }

		/** Detach widget from widgets hierarchy
			@param _layer Attach to specified layer (if any)
		*/
		//void detachFromWidget(const std::string& _layer = "");

		/** Attach widget to parent
			@param _style Child widget type
			@param _layer Attach to specified layer (if any)
			@note you might also need to call void Widget::setWidgetStyle(WidgetStyle _style);
				to set widget style (widget attached with MyGUI::WidgetStyle::Popup by default)
		*/
		//void attachToWidget(Widget* _parent, WidgetStyle _style = WidgetStyle::Child, const std::string& _layer = "");

		/** Change widget skin */
		void changeWidgetSkin(const std::string& _skinName);

		/** Set widget style.
			@param _layer Attach to specified layer (if any)
			@note When choosing WidgetStyle::Popup style you also need attach widget to layer
			see LayerManager::attachToLayerNode
		*/
		//void setWidgetStyle(WidgetStyle _style, const std::string& _layer = "");
		/** Get widget style */
		WidgetStyle getWidgetStyle() { return mWidgetStyle; }

		/** Set any widget property
			@param _key Property name (for example Widget_Alpha or Edit_MultiLine)
			@param _value Value converted to string
		*/
		virtual void setProperty(const std::string& _key, const std::string& _value);

		bool getNeedCropped();

		/** Event : Widget property changed through setProperty (in code, or from layout)\n
			signature : void method(MyGUI::Widget* _sender, const std::string& _key, const std::string& _value);
			@param _sender widget that called this event
			@param _key
			@param _value
		*/
		EventHandle_WidgetStringString eventChangeProperty;

	/*internal:*/
		// метод для запроса номера айтема и контейнера
		virtual size_t _getItemIndex(Widget* _item) { return ITEM_NONE; }

		// дает приоритет виджету при пиккинге
		void _forcePeek(Widget* _widget);

		void _initialise(WidgetStyle _style, ResourceSkin* _info, Widget* _parent);
		void _shutdown();

		void _setContainer(Widget* _value) { mContainer = _value; }
		Widget* _getContainer() { return mContainer; }

		void _setAlign(const IntSize& _oldsize);

	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : bool Widget::getEnabled() const")
		bool isEnabled() const { return getEnabled(); }
		MYGUI_OBSOLETE("use : bool Widget::getInheritsAlpha() const")
		bool isInheritsAlpha() const { return getInheritsAlpha(); }
		MYGUI_OBSOLETE("use : bool Widget::getNeedKeyFocus() const")
		bool isNeedKeyFocus() const { return getNeedKeyFocus(); }
		MYGUI_OBSOLETE("use : bool Widget::getNeedMouseFocus() const")
		bool isNeedMouseFocus() const { return getNeedMouseFocus(); }
		MYGUI_OBSOLETE("use : bool Widget::getInheritsPick() const")
		bool isInheritsPick() const { return getInheritsPick(); }
		MYGUI_OBSOLETE("use : bool Widget::getVisible() const")
		bool isVisible() const { return getVisible(); }

#endif // MYGUI_DONT_USE_OBSOLETE

	protected:
		// все создание только через фабрику
		virtual ~Widget();

		virtual void shutdownWidgetSkin() { }
		virtual void initialiseWidgetSkin(ResourceSkin* _info) { }

		void _updateView(); // обновления себя и детей

		// создает виджет
		Widget* baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name, bool _template = false);

		// запрашиваем у конейтера айтем по позиции мыши
		virtual size_t _getContainerIndex(const IntPoint& _point) { return ITEM_NONE; }

		// сброс всех данных контейнера, тултипы и все остальное
		virtual void _resetContainer(bool _update);

		virtual void baseUpdateEnable();

		// наследуемся он LayerInfo
		virtual ILayerItem * getLayerItemByPoint(int _left, int _top);
		//virtual const IntCoord& getLayerItemCoord() { return mCoord; }

		virtual void _onChildAdded(Widget* _child) { }

	private:
		void initialiseWidgetSkinBase(ResourceSkin* _info);
		void shutdownWidgetSkinBase();

		void _updateAlpha();
		void _updateVisible();
		void _updateEnabled();
		void _updateAbsolutePoint();

		bool _isInheritsVisible() { return mInheritsVisible; }
		bool _isInheritsEnable() { return mInheritsEnabled; }

		float _getRealAlpha() { return mRealAlpha; }

		void _createChildSkinWidget(ResourceSkin* _info);
		void _destroyChildSkinWidget();

		void _parseSkinProperties(ResourceSkin* _info);
		void _checkInheristProperties();

		//void _linkChildWidget(Widget* _widget);
		//void _unlinkChildWidget(Widget* _widget);
		void _destroyChildWidget(Widget* _widget, VectorWidgetPtr& _childs);

		void attachVisual();
		void detachVisual();

	protected:
		// клиентская зона окна
		// если виджет имеет пользовательские окна не в себе
		// то обязательно проинициализировать Client
		Widget* mWidgetClient;

		// вектор всех детей виджетов
		VectorWidgetPtr mWidgetChild;
		// вектор детей скина
		VectorWidgetPtr mWidgetChildSkin;

	private:
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
		bool mIsMargin;
		IntPoint mAbsolutePosition; // обсолютные координаты
	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_H__
