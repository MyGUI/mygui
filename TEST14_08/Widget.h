#pragma once

#include <vector>
#include <Ogre.h>
#include "MainSkin.h"
#include "SubSkin.h"
#include "SubWidget.h"

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
		void addSubSkin(int _x, int _y, int _cx, int _cy, float _leftUV, float _topUV, float _rightUV, float _bottomUV, const String & _material, char _align, bool _main);

		void move(int _x, int _y);
		void move(int _x, int _y, int _cx, int _cy);

		void update(); // ���������� ���� � �����

		void show(bool _show);

		void size(int _cx, int _cy);

		void align(int _cx, int _cy);

	protected:

		// ���������� �������� ��� ��������
		void visible(bool _visible);

		void attach(Ogre::OverlayElement * _element);

		typedef std::vector<Widget *> widgetChild;
		typedef widgetChild::iterator widgetIterator;

		// ������ ���� ����� ��������
		widgetChild m_widgetChild;
		// ������ ���� ����� ���������
		skinChild m_subSkinChild;

		// ������ �� ��� �������� ��� ������ �� �������
		bool m_visible;

	}; // class Widget

} // namespace widget