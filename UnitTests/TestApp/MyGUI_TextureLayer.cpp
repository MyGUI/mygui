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
#include "MyGUI_TextureLayer.h"
#include "MyGUI_TextureLayerNode.h"

namespace MyGUI
{

	TextureLayer::TextureLayer(const std::string& _name) :
		ILayer(_name)
	{
	}

	TextureLayer::~TextureLayer()
	{
		//MYGUI_ASSERT(mChildItems.empty(), "Layer '" << getName() << "' must be empty before destroy");
	}

	ILayerNode * TextureLayer::createItemNode(ILayerNode* _parent)
	{
		// пусть парент сам рулит
		if (_parent)
		{
			return _parent->createItemNode();
		}

		// создаем рутовый айтем
		TextureLayerNode * layer = new TextureLayerNode(this);
		mChildItems.push_back(layer);

		layer->_addUsing();
		return layer;
	}

	void TextureLayer::destroyItemNode(ILayerNode* _item)
	{
		TextureLayerNode* item = static_cast<TextureLayerNode*>(_item);
		TextureLayerNode * parent = item->getParent();
		// если есть отец, то русть сам и удаляет
		if (parent)
		{
			parent->destroyItemNode(item);
		}
		// айтем рутовый, мы удаляем
		else
		{
			for (VectorTextureLayerItemNode::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
			{
				if ((*iter) == _item)
				{
					item->_removeUsing();
					if (0 == item->_countUsing())
					{
						delete item;
						mChildItems.erase(iter);
					}
					return;
				}
			}
			MYGUI_EXCEPT("item keeper not found");
		}
	}

	bool TextureLayer::existItemNode(ILayerNode* _item)
	{
		TextureLayerNode* item = static_cast<TextureLayerNode*>(_item);
		for (VectorTextureLayerItemNode::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			if ((*iter) == _item || (*iter)->existItemNode(item)) return true;
		}
		return false;
	}

	void TextureLayer::upItemNode(ILayerNode* _item)
	{
		TextureLayerNode* item = static_cast<TextureLayerNode*>(_item);
		TextureLayerNode * parent = item->getParent();
		// если есть отец, то пусть сам рулит
		if (parent)
		{
			// возвращается рутовый айтем
			item = parent->upItemNode(item);
		}

		if ((2 > mChildItems.size()) || (mChildItems.back() == _item)) return;
		for (VectorTextureLayerItemNode::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			if ((*iter) == item)
			{
				mChildItems.erase(iter);
				mChildItems.push_back(item);
				return;
			}
		}

		MYGUI_EXCEPT("item keeper not found");
	}

	ILayerItem * TextureLayer::getLayerItemByPoint(int _left, int _top)
	{
		VectorTextureLayerItemNode::reverse_iterator iter = mChildItems.rbegin();
		while (iter != mChildItems.rend())
		{
			ILayerItem * item = (*iter)->getLayerItemByPoint(_left, _top);
			if (item != nullptr) return item;
			++iter;
		}
		return nullptr;
	}

	void TextureLayer::renderToTarget(IRenderTarget* _target, bool _update)
	{
		for (VectorTextureLayerItemNode::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			(*iter)->renderToTarget(_target, _update);
		}
	}

	size_t TextureLayer::getItemCount()
	{
		size_t count = 0;
		for (VectorTextureLayerItemNode::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			count += (*iter)->getItemCount();
		}
		return count;
	}

	size_t TextureLayer::getSubItemCount()
	{
		size_t count = 0;
		for (VectorTextureLayerItemNode::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			count += (*iter)->getItemCount();
		}
		return count - mChildItems.size();
	}

} // namespace MyGUI
