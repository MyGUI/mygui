/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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
