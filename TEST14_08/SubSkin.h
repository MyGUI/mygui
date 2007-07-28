#pragma once

//#include <vector>
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
		SubSkin(int _x, int _y, int _cx, int _cy, const String & _material, char _align, SubWidget * _parent);
		virtual ~SubSkin();

		void update(); // обновления себя и детей
		void correct();

		void show(bool _show);

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		void addUVSet(float _left, float _top, float _right, float _bottom);
		void setUVSet(size_t _num);

	protected:

		void attach(Ogre::OverlayElement * _element);

//		float m_baseLeftUV, m_baseTopUV, m_baseRightUV, m_baseBottomUV;
		Ogre::PanelOverlayElement * m_overlayContainer;
		std::vector<Ogre::FloatRect> m_uvSet;
		Ogre::FloatRect m_rectTexture;

	}; // class SubSkin

} // namespace widget