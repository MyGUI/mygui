/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#include "MyGUI_Singleton.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	class MYGUI_EXPORT SkinManager :
		public Singleton<SkinManager>,
		public MemberObsolete<SkinManager>
	{
	public:
		SkinManager();

		void initialise();
		void shutdown();

		/** Get ResourceSkin by name */
		ResourceSkin* getByName(const std::string& _name) const;

		/** Check if skin with specified name exist */
		bool isExist(const std::string& _name) const;

		/** Get default skin name.
			Default skin used when creating widget with skin that doesn't exist.
		*/
		const std::string getDefaultSkin() const;
		/** Set default skin name.
			Default skin used when creating widget with skin that doesn't exist.
		*/
		void setDefaultSkin(const std::string& _value);

	private:
		void createDefault(const std::string& _value);
		void _load(xml::ElementPtr _node, const std::string& _file, Version _version);

	private:
		std::string mDefaultName;

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_SKIN_MANAGER_H__
