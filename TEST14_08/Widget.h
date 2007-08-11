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
		// ��� �������� ������ ����� �������
		Widget(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfo * _info, Widget * _parent = 0);
		virtual ~Widget();

		// ������� �����
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

		void update(); // ���������� ���� � �����

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);


		void setState(const Ogre::String & _state);
		inline const static Ogre::String & getType() {static Ogre::String type("Widget"); return type;};

	protected:

		// ������� � ��������� ��� ���� �������
		BasisWidget * addSubSkin(const tagBasisWidgetInfo &_info, const String & _material);

	protected:

		// ������ ���� �������
		const StateInfo & m_stateInfo;
		// ���������� �������� ��� ��������
		void visible(bool _visible);

		void attach(Ogre::OverlayElement * _element, bool _child);

		typedef std::vector<Widget *> widgetChild;
		typedef widgetChild::iterator widgetIterator;

		// ������ ���� ����� ��������
		widgetChild m_widgetChild;
		// ������ ���� ����� ���������
		skinChild m_subSkinChild;

		// ��������� �� ���� ������
		BasisWidget * m_text;

		// ������ �� ��� �������� ��� ������ �� �������
		bool m_visible;
		// ������������ ������ �������
		float m_alpha;
		// ���� ������
		Ogre::ColourValue m_color;

	}; // class Widget

} // namespace widget