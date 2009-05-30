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
#ifndef __MYGUI_OVERLAPPED_LAYER_H__
#define __MYGUI_OVERLAPPED_LAYER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_ILayer.h"

namespace MyGUI
{

	class LayerItem;
	class LayerNode;
	typedef std::vector<LayerNode*> VectorLayerItemNode;

	class MYGUI_EXPORT OverlappedLayer : public ILayer
	{
	public:
		OverlappedLayer(const std::string& _name, bool _pick);
		virtual ~OverlappedLayer();

		virtual ILayerNode* createItemNode(ILayerNode* _parent);
		virtual void destroyItemNode(ILayerNode* _item);
		virtual bool existItemNode(ILayerNode* _item);
		virtual void upItemNode(ILayerNode* _item);

		virtual void doRender(bool _update);
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top);

		size_t getItemCount();
		size_t getSubItemCount();

	private:
		bool mIsPeek;
		VectorLayerItemNode mChildItems;
	};

} // namespace MyGUI

#endif // __MYGUI_OVERLAPPED_LAYER_H__
