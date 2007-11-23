#ifndef __SUB_SKIN_H__
#define __SUB_SKIN_H__

#include "Prerequest.h"
#include "PanelAlphaOverlayElement.h"
#include "BasisWidget.h"

namespace MyGUI
{

	class _MyGUIExport SubSkin : public BasisWidget
	{

	public:
		SubSkin(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidgetPtr _parent);
		virtual ~SubSkin();

		void show(bool _show);

		void update(); // обновления себя и детей
		void correct();

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		void setUVSet(const FloatRect & _rect);

		void setAlpha(float _alpha);

		inline const static Ogre::String & getType() {static Ogre::String type("SubSkin"); return type;};

		void attach(BasisWidgetPtr _basis, bool _child);
		Ogre::OverlayElement* getOverlayElement();

	protected:

		PanelAlphaOverlayElement * m_overlayContainer;
		FloatRect m_rectTexture;

	}; // class SubSkin

} // namespace MyGUI

#endif // __SUB_SKIN_H__