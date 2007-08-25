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
		SubSkin(const tagBasisWidgetInfo &_info, const String & _material, BasisWidgetPtr _parent);
		virtual ~SubSkin();

		void show(bool _show);

		void update(); // обновления себя и детей
		void correct();

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		void setUVSet(const Ogre::FloatRect & _rect);

		void setAlpha(float _alpha);

		inline const static Ogre::String & getType() {static Ogre::String type("SubSkin"); return type;};

		void attach(BasisWidgetPtr _basis, bool _child);
		OverlayElementPtr getOverlayElement();

	protected:

		PanelAlphaOverlayElement * m_overlayContainer;
		Ogre::FloatRect m_rectTexture;

	}; // class SubSkin

} // namespace widget