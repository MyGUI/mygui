/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef _65e1b826_c9fe_44a2_a771_1d9498291ad9_
#define _65e1b826_c9fe_44a2_a771_1d9498291ad9_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_OverlappedLayer.h"

namespace MyGUI
{

	class CustomLayer :
		public OverlappedLayer
	{
		MYGUI_RTTI_DERIVED( CustomLayer )

	public:
		virtual ILayerNode* createChildItemNode();
		virtual void renderToTarget(IRenderTarget* _target, bool _update);
	};

}

#endif
