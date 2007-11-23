#ifndef __MAIN_SKIN_H__
#define __MAIN_SKIN_H__

#include "Prerequest.h"
#include "PanelAlphaOverlayElement.h"
#include "BasisWidget.h"

namespace MyGUI
{

	class _MyGUIExport MainSkin : public BasisWidget
	{

	public:
		MainSkin(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidgetPtr _parent);
		~MainSkin();

		// вызывается виджетом, если этот саб скин во весь виджет
		void update();
		// скрываем показывает оверлей
		void show(bool _show);
		// корректируем положение скина, нужно при обрезке отца
		void correct();

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		void attach(BasisWidgetPtr _basis, bool _child);
		Ogre::OverlayElement* getOverlayElement();

		void setUVSet(const FloatRect & _rect);

		void setAlpha(float _alpha);

		inline const static Ogre::String & getType() {static Ogre::String type("MainSkin"); return type;};

	protected:

		PanelAlphaOverlayElement * m_overlayContainer;

		FloatRect m_rectTexture;

	}; // class SubSkin

} // namespace MyGUI

#endif // __MAIN_SKIN_H__