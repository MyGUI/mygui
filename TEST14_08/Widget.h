#pragma once

#include <vector>
#include <Ogre.h>
#include "MainSkin.h"

#include "WidgetSkinInfo.h"
#include "LayerItemInfo.h"
#include "WidgetUserData.h"
#include "WidgetEvent.h"
#include "WidgetFactory.h"

namespace widget
{

	class WidgetManager;

	class Widget : public BasisWidget , public LayerItemInfo, public UserData, public WidgetEvent
	{
		// для вызова закрытых деструкторов
		friend WidgetManager;
		// для вызова закрытого конструктора
		friend WidgetFactory<Widget>;


	protected:
		// все создание только через фабрику
		Widget(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		// создаем дочку
		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _name = "");
		WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _x, float _y, float _cx, float _cy, char _align, const Ogre::String & _name = "");

		void move(int _x, int _y);
		void move(int _x, int _y, int _cx, int _cy);
		void size(int _cx, int _cy);

		void show(bool _show);

		virtual void setCaption(const Ogre::DisplayString & _caption);
		virtual const Ogre::DisplayString & getCaption();

		void setColour(const Ogre::ColourValue & _color);
		const Ogre::ColourValue & getColour() {return m_color;};

		void setFontName(const Ogre::String & _font);
		void setFontName(const Ogre::String & _font, Ogre::ushort _height);
		const Ogre::String & getFontName();

		void setFontHeight(Ogre::ushort _height);
		Ogre::ushort getFontHeight();

		void setAlpha(float _alpha);
		inline float getAlpha() {return m_alpha;};
	
		inline const static Ogre::String & getType() {static Ogre::String type("Widget"); return type;};

		void setState(const Ogre::String & _state);

		void attachToOverlay(Ogre::Overlay * _overlay);
		void detachToOverlay(Ogre::Overlay * _overlay);

	protected:
		void update(); // обновления себя и детей

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		inline const Ogre::String & getName() {return m_name;};

		// присоединяемся к отцу
		void attach(BasisWidgetPtr _basis, bool _child);

		// закрываем метод базового класса
		inline WidgetPtr getParent() {return static_cast<WidgetPtr>(m_parent);}

		// удяляет только негодных батюшке государю
		void destroyWidget(WidgetPtr & _widget);
		// удаляет всех детей
		void destroyWidget();

	public:
		// возвращает указатель на айтем в этой точке попадание в виджет (наследуеться от LayerItemInfo)
		LayerItemInfoPtr findItem(int _x, int _y);

		inline bool isNeedKeyFocus() {return m_needKeyFocus;}
		inline void setNeedKeyFocus(bool _need) {m_needKeyFocus = _need;}

	protected:

		// создаем и добавляем саб скин виджету
		BasisWidgetPtr addSubSkin(const tagBasisWidgetInfo & _info, const String & _material);

	public:
		// закрытый деструктор
		virtual ~Widget();

	protected:

		// список всех стейтов
		const StateInfo & m_stateInfo;
		// показывает скрывает все сабскины
		void visible(bool _visible);

		// вектор всех детей виджетов
		WidgetChild m_widgetChild;
		// вектор всех детей сабскинов
		BasisChild m_subSkinChild;

		// указатель на окно текста
		BasisWidgetPtr m_text;

		// доступен ли на виджет
		bool m_enable;
		// скрыты ли все сабскины при выходе за границу
		bool m_visible;
		// прозрачность нашего оверлея
		float m_alpha;
		// цвет текста
		Ogre::ColourValue m_color;
		// имя виджета
		Ogre::String m_name;

	}; // class Widget

} // namespace widget