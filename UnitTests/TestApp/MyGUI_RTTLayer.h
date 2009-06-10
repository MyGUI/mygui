/*!
	@file
	@author		Albert Semenov
	@date		06/2008
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
#ifndef __MYGUI_RTT_LAYER_H__
#define __MYGUI_RTT_LAYER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_OverlappedLayer.h"

namespace MyGUI
{

	class /*MYGUI_EXPORT */RTTLayer :
		public OverlappedLayer
	{
		MYGUI_RTTI_DERIVED( RTTLayer );

	public:
		RTTLayer();
		virtual ~RTTLayer();

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		// создаем дочерний нод
		virtual ILayerNode* createChildItemNode();
		// удаляем дочерний нод
		virtual void destroyChildItemNode(ILayerNode* _node);

		// возвращает виджет по позиции
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top);

		// рисует леер
		virtual void renderToTarget(IRenderTarget* _target, bool _update);

	private:
		Version mVersion;
		xml::ElementPtr mData;
	};

} // namespace MyGUI

#endif // __MYGUI_RTT_LAYER_H__
