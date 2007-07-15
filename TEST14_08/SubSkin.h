#pragma once

#include <vector>
#include <Ogre.h>
#include <OgrePanelOverlayElement.h>
#include "MainSkin.h"
#include "Widget.h"
#include "SubWidget.h"

namespace widget
{
	using namespace Ogre;


	class SubSkin : public SubWidget
	{

	public:
		SubSkin(int _x, int _y, int _cx, int _cy, float _leftUV, float _topUV, float _rightUV, float _bottomUV, const String & _material, char _align, SubWidget * _parent);
		virtual ~SubSkin();

		void update(); // обновления себя и детей
		void correct();

		void show(bool _show);

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

	protected:

		void attach(Ogre::OverlayElement * _element);

		float m_baseLeftUV, m_baseTopUV, m_baseRightUV, m_baseBottomUV;
		Ogre::PanelOverlayElement * m_overlayContainer;

	}; // class SubSkin

} // namespace widget