/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_SharedLayerNode.h"

namespace MyGUI
{

	SharedLayerNode::SharedLayerNode(ILayer* _layer, ILayerNode* _parent) :
		LayerNode(_layer, _parent),
		mCountUsing(0)
	{
	}

	SharedLayerNode::~SharedLayerNode()
	{
	}

	void SharedLayerNode::addUsing()
	{
		mCountUsing++;
	}

	void SharedLayerNode::removeUsing()
	{
		mCountUsing--;
	}

	size_t SharedLayerNode::countUsing() const
	{
		return mCountUsing;
	}

} // namespace MyGUI
