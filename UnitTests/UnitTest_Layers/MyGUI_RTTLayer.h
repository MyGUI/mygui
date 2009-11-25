/*!
	@file
	@author		Albert Semenov
	@date		06/2008
	@module
*/

#ifndef __MYGUI_RTT_LAYER_H__
#define __MYGUI_RTT_LAYER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_OverlappedLayer.h"

namespace MyGUI
{

	class RTTLayer :
		public OverlappedLayer
	{
		MYGUI_RTTI_DERIVED( RTTLayer );

	public:
		RTTLayer();
		virtual ~RTTLayer();

		// рисует леер
		virtual void renderToTarget(IRenderTarget* _target, bool _update);

		virtual ILayerItem* getLayerItemByPoint(int _left, int _top);

	private:
		MyGUI::ITexture* mTexture;
	};

} // namespace MyGUI

#endif // __MYGUI_RTT_LAYER_H__
