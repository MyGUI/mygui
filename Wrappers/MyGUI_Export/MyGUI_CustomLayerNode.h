/*!
	@file
	@author		Albert Semenov
	@date		05/2008
*/

#ifndef _8b05f275_74eb_41b3_8bf0_08bb66ce8c7d_
#define _8b05f275_74eb_41b3_8bf0_08bb66ce8c7d_

#include "MyGUI_Prerequest.h"
#include "MyGUI_SharedLayerNode.h"

namespace MyGUI
{

	class CustomLayerNode :
		public SharedLayerNode
	{
		MYGUI_RTTI_DERIVED( CustomLayerNode )

	public:
		explicit CustomLayerNode(ILayer* _layer, ILayerNode* _parent = nullptr);

		virtual ILayerNode* createChildItemNode();

		virtual float getNodeDepth();
	};

}

#endif
