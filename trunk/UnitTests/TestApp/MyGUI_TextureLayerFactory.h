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
#ifndef __MYGUI_TEXTURE_LAYER_FACTORY_H__
#define __MYGUI_TEXTURE_LAYER_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_ILayerFactory.h"
#include "MyGUI_TextureLayer.h"

namespace MyGUI
{

	class/* MYGUI_EXPORT*/ TextureLayerFactory : public ILayerFactory
	{
	public:
		virtual ~TextureLayerFactory() { }
		virtual ILayer * createLayer(xml::ElementPtr _node, Version _version)
		{
			return new TextureLayer(
				_node->findAttribute("name")
				);
		}
	};

} // namespace MyGUI

#endif // __MYGUI_TEXTURE_LAYER_FACTORY_H__
