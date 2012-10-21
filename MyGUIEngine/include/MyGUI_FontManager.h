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
#ifndef __MYGUI_FONT_MANAGER_H__
#define __MYGUI_FONT_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_IFont.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	class MYGUI_EXPORT FontManager :
		public Singleton<FontManager>,
		public MemberObsolete<FontManager>
	{
	public:
		FontManager();

		void initialise();
		void shutdown();

		/** Get default font name.
			Default skin also used when creating widget with skin that doesn't exist.
		*/
		const std::string& getDefaultFont() const;
		/** Get default font name.
			Default skin also used when creating widget with skin that doesn't exist.
		*/
		void setDefaultFont(const std::string& _value);

		/** Get font resource */
		IFont* getByName(const std::string& _name) const;

	private:
		void _load(xml::ElementPtr _node, const std::string& _file, Version _version);

	private:
		std::string mDefaultName;

		bool mIsInitialise;
		std::string mXmlFontTagName;
		std::string mXmlPropertyTagName;
		std::string mXmlDefaultFontValue;
	};

} // namespace MyGUI

#endif // __MYGUI_FONT_MANAGER_H__
