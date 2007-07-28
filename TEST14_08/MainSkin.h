#pragma once

#include <Ogre.h>
#include <OgrePanelOverlayElement.h>
#include "SubWidget.h"

namespace widget
{
	using namespace Ogre;

	class MainSkin : public SubWidget
	{

	public:
		MainSkin(int _x, int _y, int _cx, int _cy, const String & _material, char _align, SubWidget * _parent);
		~MainSkin();

		// вызывается виджетом, если этот саб скин во весь виджет
		void update();
		// скрываем показывает оверлей
		void show(bool _show);
		// корректируем положение скина, нужно при обрезке отца
		void correct();

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		void attach(Ogre::OverlayElement * _element);

		void addUVSet(float _left, float _top, float _right, float _bottom);
		void setUVSet(size_t _num);

	protected:

		Ogre::PanelOverlayElement * m_overlayContainer;

		std::vector<Ogre::FloatRect> m_uvSet;
		Ogre::FloatRect m_rectTexture;

	}; // class SubSkin

} // namespace widget