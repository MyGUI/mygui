#pragma once

#include <vector>
#include <Ogre.h>
#include "MainSkin.h"
#include "delegate.h"

#include "WidgetSkinInfo.h"

namespace widget
{
	using namespace Ogre;

	class Widget : public BasisWidget
	{

	public:
		// все создание только через фабрику
		Widget(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfo * _info, Widget * _parent = 0);
		virtual ~Widget();

		// создаем дочку
		Widget * createChild(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align);

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

		void setCharHeight(Ogre::ushort _height);
		Ogre::ushort getCharHeight();

		void setAlpha(float _alpha);
		inline float getAlpha() {return m_alpha;};

		void update(); // обновления себя и детей

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);


		void setState(const Ogre::String & _state);
		inline const static Ogre::String & getType() {static Ogre::String type("Widget"); return type;};

	protected:

		// создаем и добавляем саб скин виджету
		BasisWidget * addSubSkin(const tagBasisWidgetInfo &_info, const String & _material);

	protected:

		// список всех стейтов
		const StateInfo & m_stateInfo;
		// показывает скрывает все сабскины
		void visible(bool _visible);

		void attach(Ogre::OverlayElement * _element, bool _child);

		typedef std::vector<Widget *> widgetChild;
		typedef widgetChild::iterator widgetIterator;

		// вектор всех детей виджетов
		widgetChild m_widgetChild;
		// вектор всех детей сабскинов
		skinChild m_subSkinChild;

		// указатель на окно текста
		BasisWidget * m_text;

		// скрыты ли все сабскины при выходе за границу
		bool m_visible;
		// прозрачность нашего оверлея
		float m_alpha;
		// цвет текста
		Ogre::ColourValue m_color;

	}; // class Widget

} // namespace widget