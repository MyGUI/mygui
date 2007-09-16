#ifndef _MAINSKIN_H_
#define _MAINSKIN_H_

#include "Prerequest.h"
//#include <Ogre.h>
#include "PanelAlphaOverlayElement.h"
#include "BasisWidget.h"

namespace MyGUI
{
	using namespace Ogre;

	class _MyGUIExport MainSkin : public BasisWidget
	{

	public:
		MainSkin(const tagBasisWidgetInfo &_info, const String & _material, BasisWidgetPtr _parent);
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
		OverlayElementPtr getOverlayElement();

		void setUVSet(const floatRect & _rect);

		void setAlpha(float _alpha);

		inline const static Ogre::String & getType() {static Ogre::String type("MainSkin"); return type;};

	protected:

		PanelAlphaOverlayElement * m_overlayContainer;

		floatRect m_rectTexture;

	}; // class SubSkin

} // namespace MyGUI

#endif
