/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_FONT_MANAGER_H_
#define MYGUI_FONT_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_IFont.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	class MYGUI_EXPORT FontManager : public MemberObsolete<FontManager>
	{
		MYGUI_SINGLETON_DECLARATION(FontManager);

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
		void setDefaultFont(std::string_view _value);

		/** Get font resource */
		IFont* getByName(std::string_view _name) const;

	private:
		void _load(xml::ElementPtr _node, std::string_view _file, Version _version);

	private:
		std::string mDefaultName;

		bool mIsInitialise{false};
		std::string mXmlFontTagName;
		std::string mXmlPropertyTagName;
		std::string mXmlDefaultFontValue;
	};

} // namespace MyGUI

#endif // MYGUI_FONT_MANAGER_H_
