/*!
	@file
	@author		Albert Semenov
	@date		05/2008
*/

#ifndef _8b05f275_74eb_41b3_8bf0_08bb66ce8c7d_
#define _8b05f275_74eb_41b3_8bf0_08bb66ce8c7d_

#include "MyGUI_Prerequest.h"
#include "MyGUI_LayerNode.h"

namespace MyGUI
{

	class CustomLayerNode :
		public LayerNode
	{
		MYGUI_RTTI_DERIVED( CustomLayerNode )

	public:
		explicit CustomLayerNode(ILayer* _layer, ILayerNode* _parent = nullptr);

		virtual ILayerNode* createChildItemNode();

		virtual float getNodeDepth();

		void setDepth(float _value);

	private:
		void updateInheritsDepth(float _value);
		void invalidateAll();

	private:
		float mLocalDepth;
		float mInheritsDepth;
	};

}

#endif
