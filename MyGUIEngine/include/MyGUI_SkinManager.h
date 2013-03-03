/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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
		std::string mXmlSkinTagName;
		std::string mXmlDefaultSkinValue;
	};

} // namespace MyGUI

#endif // __MYGUI_SKIN_MANAGER_H__
