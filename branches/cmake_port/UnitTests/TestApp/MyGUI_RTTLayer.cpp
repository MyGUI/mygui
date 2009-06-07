/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_Common.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_RTTLayer.h"
#include "MyGUI_RTTLayerNode.h"
#include "MyGUI_Enumerator.h"

namespace MyGUI
{

	RTTLayer::RTTLayer(xml::ElementPtr _node, Version _version) :
		OverlappedLayer(_node->findAttribute("name"), utility::parseBool(_version < Version(1, 0) ? _node->findAttribute("peek") : _node->findAttribute("pick"))),
		mData(nullptr)
	{
		mVersion = _version;
		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next("Animator"))
		{
			mData = node->createCopy();
			break;
		}
	}

	RTTLayer::~RTTLayer()
	{
		delete mData;
	}

	ILayerNode * RTTLayer::createChildItemNode()
	{
		// создаем рутовый айтем
		RTTLayerNode* node = new RTTLayerNode(this);
		mChildItems.push_back(node);

		if (mData != nullptr)
		{
			const std::string& type = mData->findAttribute("type");
			if (!type.empty())
			{
				LayerNodeAnimation* animator = createFromFactory(type, mData, mVersion);
				node->setLayerNodeAnimation(animator);
			}
		}

		return node;
	}

	/*void RTTLayer::setLayerNodeAnimation(LayerNodeAnimation* _impl)
	{
		Enumerator<VectorILayerNode> node(mChildItems);
		while (node.next())
		{
			node->castType<RTTLayerNode>()->setLayerNodeAnimation(_impl);
		}
	}*/

	/*void RTTLayer::registerFactory(const std::string& _name, DelegateFactory::IDelegate* _delegate)
	{
		mDelegates[_name] = _delegate;
	}*/

} // namespace MyGUI
