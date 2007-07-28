#pragma once

#include <vector>
#include <Ogre.h>
#include "MainSkin.h"
#include "SubSkin.h"
#include "SubWidget.h"
#include "delegate.h"

namespace widget
{
	using namespace Ogre;


	class Widget : public SubWidget
	{

	public:
		Widget(int _x, int _y, int _cx, int _cy, char _align, Widget * _parent);
		virtual ~Widget();

		// ������� �����
		Widget * createChild(int _x, int _y, int _cx, int _cy, char _align);
		// ������� � ��������� ��� ���� �������
		SubWidget * addSubSkin(int _x, int _y, int _cx, int _cy, const String & _material, char _align, bool _main);
		void addText(char _align);

		void move(int _x, int _y);
		void move(int _x, int _y, int _cx, int _cy);

		void update(); // ���������� ���� � �����

		void show(bool _show);

		void size(int _cx, int _cy);

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		virtual void setCaption(const Ogre::DisplayString & _caption);

		void setUVSet(size_t _num);


//	protected:

		// ���������� �������� ��� ��������
		void visible(bool _visible);

		void attach(Ogre::OverlayElement * _element);

		typedef std::vector<Widget *> widgetChild;
		typedef widgetChild::iterator widgetIterator;

		// ������ ���� ����� ��������
		widgetChild m_widgetChild;
		// ������ ���� ����� ���������
		skinChild m_subSkinChild;

		// ��������� �� ���� ������
		SubWidget * m_text;

		// ������ �� ��� �������� ��� ������ �� �������
		bool m_visible;

	}; // class Widget

} // namespace widget