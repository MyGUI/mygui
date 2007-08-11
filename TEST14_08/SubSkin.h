#pragma once

#include <Ogre.h>
#include "PanelAlphaOverlayElement.h"
#include "BasisWidget.h"

namespace widget
{
	using namespace Ogre;


	class SubSkin : public BasisWidget
	{

	public:
		SubSkin(const tagBasisWidgetInfo &_info, const String & _material, BasisWidget * _parent);
		virtual ~SubSkin();

		void show(bool _show);

		void update(); // обновления себя и детей
		void correct();

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		void setUVSet(const Ogre::FloatRect & _rect);

		void setAlpha(float _alpha);

		inline const static Ogre::String & getType() {static Ogre::String type("SubSkin"); return type;};

	protected:

		void attach(Ogre::OverlayElement * _element, bool _child);

		PanelAlphaOverlayElement * m_overlayContainer;
		Ogre::FloatRect m_rectTexture;

	}; // class SubSkin

} // namespace widget