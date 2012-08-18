/*!
	@file
	@author		Albert Semenov
	@date		05/2008
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_CustomLayerNode.h"

namespace MyGUI
{

	CustomLayerNode::CustomLayerNode(ILayer* _layer, ILayerNode* _parent) :
		Base(_layer, _parent)
	{
	}

	ILayerNode* CustomLayerNode::createChildItemNode()
	{
		ILayerNode* layer = new CustomLayerNode(mLayer, this);
		mChildItems.push_back(layer);

		mOutOfDate = true;

		return layer;
	}

	float CustomLayerNode::getNodeDepth()
	{
		float result = Base::getNodeDepth();

		float offset = (float)(::rand() % 10) / (float)20;

		return result + offset;
	}

}
