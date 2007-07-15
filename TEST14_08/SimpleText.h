#pragma once

#include <Ogre.h>
#include <OgrePanelOverlayElement.h>
#include "SubWidget.h"

namespace widget
{
	using namespace Ogre;

	class SimpleText : public SubWidget
	{

	public:
		SimpleText (int _x, int _y, int _cx, int _cy, char _align, SubWidget * _parent);
		~SimpleText ();

		// ���������� ��������, ���� ���� ��� ���� �� ���� ������
		void update();
		// �������� ���������� �������
		void show(bool _show);
		// ������������ ��������� �����, ����� ��� ������� ����
		void correct();

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		void attach(Ogre::OverlayElement * _element);

	protected:

//		float m_baseLeftUV, m_baseTopUV, m_baseRightUV, m_baseBottomUV;

//		Ogre::PanelOverlayElement * m_overlayContainer;

	}; // class SubSkin

} // namespace widget