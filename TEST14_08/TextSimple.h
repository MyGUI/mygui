#pragma once

//#include <vector>
#include <Ogre.h>
//#include <OgrePanelOverlayElement.h>
#include "MainSkin.h"
#include "Widget.h"
#include "SubWidget.h"
#include "TextSimpleOverlayElement.h"

namespace widget
{
	using namespace Ogre;


	class TextSimple : public SubWidget
	{

	public:
		TextSimple(int _x, int _y, int _cx, int _cy, float _leftUV, float _topUV, float _rightUV, float _bottomUV, const String & _material, char _align, SubWidget * _parent);
		virtual ~TextSimple();

		void update(); // обновления себя и детей
//		void correct();

		void show(bool _show);

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		void setCaption(const Ogre::String & _caption);

	protected:

//		void attach(Ogre::OverlayElement * _element);

		float m_baseLeftUV, m_baseTopUV, m_baseRightUV, m_baseBottomUV;
		TextSimpleOverlayElement * m_overlayContainer;

	}; // class SubSkin

} // namespace widget