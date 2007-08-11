#pragma once

#include <Ogre.h>
#include "PanelAlphaOverlayElement.h"
#include "BasisWidget.h"

namespace widget
{
	using namespace Ogre;

	class MainSkin : public BasisWidget
	{

	public:
		MainSkin(const tagBasisWidgetInfo &_info, const String & _material, BasisWidget * _parent);
		~MainSkin();

		// вызывается виджетом, если этот саб скин во весь виджет
		void update();
		// скрываем показывает оверлей
		void show(bool _show);
		// корректируем положение скина, нужно при обрезке отца
		void correct();

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		void attach(Ogre::OverlayElement * _element, bool _child);

		void setUVSet(const Ogre::FloatRect & _rect);

		void setAlpha(float _alpha);

		inline const static Ogre::String & getType() {static Ogre::String type("MainSkin"); return type;};

	protected:

		PanelAlphaOverlayElement * m_overlayContainer;

		Ogre::FloatRect m_rectTexture;

	}; // class SubSkin

} // namespace widget