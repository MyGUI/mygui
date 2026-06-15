/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_SKIN_MANAGER_H_
#define MYGUI_SKIN_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	class MYGUI_EXPORT SkinManager : public MemberObsolete<SkinManager>
	{
		MYGUI_SINGLETON_DECLARATION(SkinManager);

	public:
		SkinManager();

		void initialise();
		void shutdown();

		/** Get ResourceSkin by name */
		ResourceSkin* getByName(std::string_view _name) const;

		/** Check if skin with specified name exist */
		bool isExist(std::string_view _name) const;

		/** Get default skin name.
			Default skin used when creating widget with skin that doesn't exist.
		*/
		const std::string& getDefaultSkin() const;
		/** Set default skin name.
			Default skin used when creating widget with skin that doesn't exist.
		*/
		void setDefaultSkin(std::string_view _value);

	private:
		void createDefault(std::string_view _value);
		void _load(xml::ElementPtr _node, std::string_view _file, Version _version);

	private:
		std::string mDefaultName;

		bool mIsInitialise{false};
		std::string mXmlSkinTagName;
		std::string mXmlDefaultSkinValue;
	};

} // namespace MyGUI

#endif // MYGUI_SKIN_MANAGER_H_
