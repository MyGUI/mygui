/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#ifndef __MYGUI_SKIN_MANAGER_H__
#define __MYGUI_SKIN_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_SkinInfo.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_ResourceHolder.h"

namespace MyGUI
{

	class MYGUI_EXPORT SkinManager :
		public ResourceHolder<SkinInfo>
	{
		MYGUI_INSTANCE_HEADER(SkinManager);

	public:
		void initialise();
		void shutdown();

		/** Load additional MyGUI *_skin.xml file */
		bool load(const std::string& _file/*, const std::string& _group = MyGUI::ResourceManager::GUIResourceGroupName*/);
		void _load(xml::ElementPtr _node, const std::string& _file, Version _version);

		// конвертирует из пиксельных координат в текстурные, в Rect задано начало и размер
		/** Convert pixel coordinates to texture UV coordinates */
		//static FloatRect convertTextureCoord(const FloatRect & _source, const IntSize & _textureSize);


	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : SkinInfo* SkinManager::findByName(const std::string& _name)")
		SkinInfo* getSkin(const std::string& _name) { return findByName(_name); }

#endif // MYGUI_DONT_USE_OBSOLETE

	private:
		void createDefault();

	};

} // namespace MyGUI

#endif // __MYGUI_SKIN_MANAGER_H__
