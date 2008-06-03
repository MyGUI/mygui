/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_H__
#define __MYGUI_WIDGET_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_CroppedRectangleInterface.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_WidgetUserData.h"
#include "MyGUI_WidgetEvent.h"
#include "MyGUI_WidgetCreator.h"

namespace MyGUI
{

	class _MyGUIExport Widget : public CroppedRectangleInterface , public LayerItem, public UserData, public WidgetEvent, public WidgetCreator
	{
		// дл€ вызова закрытых деструкторов
		friend class WidgetCreator;
		// дл€ вызова закрытого конструктора
		friend class factory::WidgetFactory;

	protected:
		// все создание только через фабрику
		Widget(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		virtual ~Widget();

		void _updateView(); // обновлени€ себ€ и детей

		void _setAlign(const IntSize& _size, bool _update);
		void _setAlign(const IntCoord& _coord, bool _update);

		// показывает скрывает все сабскины
		void _setVisible(bool _visible);

		// создает виджет
		virtual WidgetPtr _createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name);

		// уд€л€ет неудачника
		virtual void _destroyChildWidget(WidgetPtr _widget);

		// удал€ет всех детей
		virtual void _destroyAllChildWidget();

		static Ogre::String WidgetTypeName;

	public:

		// методы и шаблоны дл€ создани€ виджета
		/** Create child widget
			@param _type widget type
			@param _skin widget skin
			@param _coord int coordinates of widget (_left, _top, _width, _height)
			@param _align widget align (possible values can be found in enum ALIGN_INFO)
			@param _name if needed (you can use it for finding widget by name later)
		*/
		inline WidgetPtr createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _name = "")
		{
			return _createWidget(_type, _skin, _coord, _align, "", _name);
		}
		/** See Gui::createWidgetT */
		inline WidgetPtr createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _name = "")
		{
			return createWidgetT(_type, _skin, IntCoord(_left, _top, _width, _height), _align, _name);
		}

		/** Create widget using coordinates relative to parent. see Gui::createWidgetT */
		WidgetPtr createWidgetRealT(const Ogre::String & _type, const Ogre::String & _skin, const FloatCoord& _coord, Align _align, const Ogre::String & _name = "");
		/** Create widget using coordinates relative to parent. see Gui::createWidgetT */
		inline WidgetPtr createWidgetRealT(const Ogre::String & _type, const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _name = "")
		{
			return createWidgetRealT(_type, _skin, FloatCoord(_left, _top, _width, _height), _align, _name);
		}

		// templates for creating widgets by type
		/** Same as Widget::createWidgetT but return T* instead of WidgetPtr */
		template <class T> inline T* createWidget(const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetT(T::_getType(), _skin, _coord, _align, _name));
		}
		/** Same as Widget::createWidgetT but return T* instead of WidgetPtr */
		template <class T> inline T* createWidget(const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetT(T::_getType(), _skin, IntCoord(_left, _top, _width, _height), _align, _name));
		}
		/** Same as Widget::createWidgetRealT but return T* instead of WidgetPtr */
		template <class T> inline T* createWidgetReal(const Ogre::String & _skin, const FloatCoord& _coord, Align _align, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetRealT(T::_getType(), _skin, _coord, _align, _name));
		}
		/** Same as Widget::createWidgetRealT but return T* instead of WidgetPtr */
		template <class T> inline T* createWidgetReal(const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetRealT(T::_getType(), _skin, _left, _top, _width, _height, _align, _name));
		}

		//! Get name of widget
		inline const Ogre::String & getName() {return mName;}
		//! Get widget class type
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! Get widget type
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		/** Set widget position (position of left top corner)*/
		virtual void setPosition(const IntPoint& _pos);
		/** Set widget position and size*/
		virtual void setPosition(const IntCoord& _coord);
		/** Set widget size */
		virtual void setSize(const IntSize& _size);

		/** See Widget::setPosition(const IntPoint& _pos) */
		inline void setPosition(int _left, int _top) {setPosition(IntPoint(_left, _top));}
		/** See Widget::setPosition(const IntCoord& _coord) */
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		/** See Widget::setSize(const IntSize& _size) */
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		void _updateAbsolutePoint();

		/** Show widget */
		virtual void show();
		/** Hide widget */
		virtual void hide();

		// дл€ внутреннего использовани€
		void _inheritedShow();
		void _inheritedHide();
		inline bool _isInheritedShow() {return mInheritedShow;}

		/** Set widget caption */
		virtual void setCaption(const Ogre::UTFString & _caption);
		/** Get widget caption */
		virtual const Ogre::UTFString & getCaption();

		/** Set widget text colour */
		virtual void setColour(const Ogre::ColourValue & _colour);
		/** Get widget text colour */
		virtual const Ogre::ColourValue & getColour();

		/** Set widget text font */
		virtual void setFontName(const Ogre::String & _font);
		/** Get widget text font name */
		virtual const Ogre::String & getFontName();

		/** Set widget text font height */
		virtual void setFontHeight(uint16 _height);
		/** Get widget text font height */
		virtual Ogre::ushort getFontHeight();

		/** Set widget text align */
		virtual void setTextAlign(Align _align);
		/** Get widget text align */
		virtual Align getTextAlign();

		/** Get text coordinates */
		IntCoord getTextCoord();
		/** Get text size in pixels */
		IntSize getTextSize();

		/** Set widget opacity */
		void setAlpha(float _alpha);
		/** Get widget opacity */
		inline float getAlpha() {return mAlpha;};
		inline float _getRealAlpha() {return mRealAlpha;}
		/** Get inherits alpha mode flag */
		inline bool isInheritsAlpha() {return mInheritsAlpha;}
		/** Enable or disable inherits alpha mode */
		inline void setInheritsAlpha(bool _inherits)
		{
			mInheritsAlpha = _inherits;
			_updateAlpha();
		}
		void _updateAlpha();

		/** Set widget's state */
		void setState(const Ogre::String & _state);
		void _setUVSet(const FloatRect& _rect);

		virtual void _setTextureName(const Ogre::String& _texture);
		virtual const Ogre::String& _getTextureName();

		// €вл€емс€ ли мы рутовым виджетом
		/** Is this widget is root widget (root == without parents) */
		inline bool isRootWidget() {return null == mParent;}

		// закрываем метод базового класса
		/** Get parent widget */
		inline WidgetPtr getParent() {return static_cast<WidgetPtr>(mParent);}

		// дл€ поддержки окон напр€мую не €вл€ющиес€ детьми
		inline WidgetPtr _getOwner() {return mOwner;}
		inline void _setOwner(WidgetPtr _widget) { if (isRootWidget()) mOwner = _widget; }

		/** Get copy of child widgets vector */
		virtual VectorWidgetPtr getChilds();

		// наследуемс€ он LayerInfo
		virtual LayerItem * _findLayerItem(int _left, int _top);
		virtual void _attachToLayerItemKeeper(LayerItemKeeper * _item);
		virtual void _detachFromLayerItemKeeper();

		/** Is need key focus */
		inline bool isNeedKeyFocus() {return mNeedKeyFocus;}
		/** Set need key focus flag */
		inline void setNeedKeyFocus(bool _need) {mNeedKeyFocus = _need;}
		/** Is need mouse focus */
		inline bool isNeedMouseFocus() {return mNeedMouseFocus;}
		/** Set need mouse focus flag */
		inline void setNeedMouseFocus(bool _need) {mNeedMouseFocus = _need;}

		/** Is widget enabled */
		inline bool isEnabled() {return mEnabled;}
		/** Enable or disable widget */
		virtual void setEnabled(bool _enabled);

		// мен€ет доступность без изменени€ стейтов
		inline void setEnabledSilent(bool _enabled) { mEnabled = _enabled; }

		/** Get mouse pointer name for this widget */
		inline const std::string& getPointer()
		{
			if (false == mEnabled) {
				static std::string empty;
				return empty;
			}
			return mPointer;
		}

		/** Set mouse pointer for this widget */
		inline void setPointer(const std::string& _pointer)
		{
			mPointer = _pointer;
		}

		// дает приоритет виджету при пиккинге
		void _forcePeek(WidgetPtr _widget);

		/** Get widget's layer, return "" if widget is not root widget (root == without parents) */
		const std::string& getLayerName();

		inline WidgetCreator * _getWidgetCreator()
		{
			return mWidgetCreator;
		}

		/** Get rect where child widgets placed */
		IntCoord getClientCoord();

		// возвращает клиентскую зону
		inline WidgetPtr getClientWidget() {return mWidgetClient;}


		// метод дл€ запроса номера айтема и главного окна списка
		virtual void _getDragItemInfo(WidgetPtr & _list, size_t & _index);
		// метод дл€ установлени€ стейта айтема
		virtual void _setDragItemInfo(size_t _index, bool _set, bool _accept);
		// информаци€ об элементе дропа по индексу
		virtual void * _getDropItemData(size_t _index);


      /** Set drag'n'drop mode flag */
		inline void setNeedDragDrop(bool _need) {mNeedDragDrop = _need;}
      /** Get drag'n'drop mode flag */
		inline bool getNeedDragDrop() {return mNeedDragDrop;}

	protected:
		// список всех стейтов
		const MapWidgetStateInfo & mStateInfo;
		// информаци€ о маске дл€ пикинга
		const MaskPeekInfo & mMaskPeekInfo;

		// вектор всех детей виджетов
		VectorWidgetPtr mWidgetChild;
		// вектор всех детей сабскинов
		VectorCroppedRectanglePtr mSubSkinChild;

		// указатель на окно текста
		SubWidgetTextInterfacePtr mText;
		// указатель на первый не текстовой сабскин
		CroppedRectangleInterface * mMainSkin;

		// доступен ли на виджет
		bool mEnabled;
		// скрыты ли все сабскины при выходе за границу
		bool mVisible;
		// дл€ иерархического скрыти€
		bool mInheritedShow;
		// прозрачность и флаг наследовани€ альфы нашего оверле€
		float mAlpha;
		float mRealAlpha;
		bool mInheritsAlpha;
		// им€ виджета
		Ogre::String mName;
		// курсор который будет показан при наведении
		std::string mPointer;
		std::string mTexture;

		// дл€ поддержки окон, напр€мую не €вл€ющимис€ детьми
		// всплывающие окна, списки комбобоксов и т.д.
		WidgetPtr mOwner;

		// это тот кто нас создал, и кто нас будет удал€ть
		WidgetCreator * mWidgetCreator;

		// нужен ли виджету ввод с клавы
		bool mNeedKeyFocus;
		// нужен ли виджету фокус мыши
		bool mNeedMouseFocus;

		// нужно и виджету поддержка драг энд дропа
		bool mNeedDragDrop;

		// клиентска€ зона окна
		WidgetPtr mWidgetClient;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_H__
