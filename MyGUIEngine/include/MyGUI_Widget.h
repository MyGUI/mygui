/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_H__
#define __MYGUI_WIDGET_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_BasisWidget.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_LayerItemInfo.h"
#include "MyGUI_WidgetUserData.h"
#include "MyGUI_WidgetEvent.h"
#include "MyGUI_WidgetFactory.h"

namespace MyGUI
{

	class _MyGUIExport Widget : public BasisWidget , public LayerItemInfo, public UserData, public WidgetEvent
	{
		// дл€ вызова закрытых деструкторов
		friend WidgetManager;
		// дл€ вызова закрытого конструктора
		friend factory::WidgetFactory;

	protected:
		// все создание только через фабрику
		Widget(int _left, int _top, int _width, int _height, Align _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

		void update(); // обновлени€ себ€ и детей

		void align(int _width, int _height, bool _update);
		void align(int _left, int _top, int _width, int _height, bool _update);

		inline const Ogre::String & getName() {return mName;};

		// присоедин€емс€ к отцу
		void attach(BasisWidgetPtr _basis, bool _child);

		// уд€л€ет только негодных батюшке государю
		void _destroyChildWidget(WidgetPtr & _widget);
		// удал€ет всех детей
		void _destroyAllChildWidget();

		// наследуетс€ и переопредел€етс€ дл€ избежани€ динамического преобразовани€
		bool isWidget() {return true;}

		// вспомогательный метод дл€ распарсивани€ сабвиджетофф
		WidgetPtr parseSubWidget(const MapString & _param, const std::string & _type, const std::string & _skin, const std::string & _offset, const std::string & _align, const IntSize &_size);

	public:
		// создаем дочку
		virtual WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _name = "");
		inline WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _name = "");

		virtual void move(int _left, int _top);
		virtual void move(int _left, int _top, int _width, int _height);
		virtual void size(int _width, int _height);

		void show(bool _show);

		virtual void setCaption(const Ogre::DisplayString & _caption);
		virtual const Ogre::DisplayString & getCaption();

		void setColour(const Ogre::ColourValue & _colour);
		const Ogre::ColourValue & getColour() {return mColour;};

		void setFontName(const Ogre::String & _font);
		void setFontName(const Ogre::String & _font, Ogre::ushort _height);
		const Ogre::String & getFontName();

		void setFontHeight(Ogre::ushort _height);
		Ogre::ushort getFontHeight();

		void setTextAlign(Align _align);

		void setAlpha(float _alpha);
		inline float getAlpha() {return mAlpha;};
	
		void setState(const Ogre::String & _state);

		void attachToOverlay(Ogre::Overlay * _overlay);
		void detachToOverlay(Ogre::Overlay * _overlay);


		// закрываем метод базового класса
		inline WidgetPtr getParent() {return static_cast<WidgetPtr>(mParent);}

		// возвращает указатель на айтем в этой точке попадание в виджет (наследуетьс€ от LayerItemInfo)
		LayerItemInfoPtr findItem(int _left, int _top);

		inline bool isNeedKeyFocus() {return mNeedKeyFocus;}
		inline void setNeedKeyFocus(bool _need) {mNeedKeyFocus = _need;}
		inline bool isEnable() {return mEnable;}

	protected:

		// создаем и добавл€ем саб скин виджету
		BasisWidgetPtr addSubSkin(const BasisWidgetInfo& _info, const Ogre::String& _material);

	public:
		// закрытый деструктор
		virtual ~Widget();

	protected:
		// показывает скрывает все сабскины
		void visible(bool _visible);

	protected:
		// список всех стейтов
		const MapWidgetStateInfo & mStateInfo;

		// вектор всех детей виджетов
		VectorWidgetPtr mWidgetChild;
		// вектор всех детей сабскинов
		VectorBasisWidgetPtr mSubSkinChild;

		// указатель на окно текста
		BasisWidgetPtr mText;

		// доступен ли на виджет
		bool mEnable;
		// скрыты ли все сабскины при выходе за границу
		bool mVisible;
		// прозрачность нашего оверле€
		float mAlpha;
		// цвет текста
		Ogre::ColourValue mColour;
		// им€ виджета
		Ogre::String mName;

	}; // class Widget

} // namespace MyGUI

#endif // __MYGUI_WIDGET_H__