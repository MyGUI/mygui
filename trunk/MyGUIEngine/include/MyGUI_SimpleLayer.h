/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*//*
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
#ifndef __MYGUI_DEFAULT_LAYER_H__
#define __MYGUI_DEFAULT_LAYER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_ILayer.h"

namespace MyGUI
{

	class LayerItem;
	class LayerNode;

	class MYGUI_EXPORT SimpleLayer : public ILayer
	{
	public:
		SimpleLayer(const std::string& _name, bool _pick);
		virtual ~SimpleLayer();

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
		LayerNode* mChildItem;
	};

} // namespace MyGUI

#endif // __MYGUI_DEFAULT_LAYER_H__
