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
#ifndef __MYGUI_OVERLAPPED_LAYER_FACTORY_H__
#define __MYGUI_OVERLAPPED_LAYER_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_ILayerFactory.h"
#include "MyGUI_OverlappedLayer.h"

namespace MyGUI
{

	class MYGUI_EXPORT OverlappedLayerFactory : public ILayerFactory
	{
	public:
		virtual ~OverlappedLayerFactory() { }
		virtual ILayer * createLayer(xml::ElementPtr _node, Version _version)
		{
			return new OverlappedLayer(
				_node->findAttribute("name"),
				utility::parseBool(_version < Version(1, 0) ? _node->findAttribute("peek") : _node->findAttribute("pick"))
				);
		}
	};

} // namespace MyGUI

#endif // __MYGUI_OVERLAPPED_LAYER_FACTORY_H__
