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
		Base(_layer, _parent),
		mLocalDepth(0),
		mInheritsDepth(0)
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

		return result + mInheritsDepth + mLocalDepth;
	}

	void CustomLayerNode::setDepth(float _value)
	{
		mLocalDepth = _value;

		invalidateAll();
	}

	void CustomLayerNode::invalidateAll()
	{
		mOutOfDate = true;

		for (VectorRenderItem::iterator iter = mFirstRenderItems.begin(); iter != mFirstRenderItems.end(); ++iter)
			(*iter)->outOfDate();

		for (VectorRenderItem::iterator iter = mSecondRenderItems.begin(); iter != mSecondRenderItems.end(); ++iter)
			(*iter)->outOfDate();

		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
			(*iter)->castType<CustomLayerNode>()->updateInheritsDepth(mInheritsDepth + mLocalDepth);
	}

	void CustomLayerNode::updateInheritsDepth(float _value)
	{
		mInheritsDepth = _value;

		invalidateAll();
	}

}
