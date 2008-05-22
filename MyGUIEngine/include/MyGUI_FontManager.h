/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_FONT_MANAGER_H__
#define __MYGUI_FONT_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include <OgrePrerequisites.h>
#include <OgreSingleton.h>
#include <OgreResourceManager.h>
#include "MyGUI_Font.h"
#include "MyGUI_Instance.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{

	class _MyGUIExport FontManager : public Ogre::ResourceManager
	{
		INSTANCE_HEADER(FontManager);

	public:
		void initialise();
		void shutdown();

		/** Load additional MyGUI *.font file */
		bool load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void _load(xml::xmlNodePtr _node, const std::string & _file);

		void saveFontTexture(const std::string & _font, const std::string & _file);

    protected:

        /// Internal methods
		Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
			const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
			const Ogre::NameValuePairList* params);

	}; // class _MyGUIExport FontManager : public Ogre::ResourceManager

} // namespace MyGUI

#endif // __MYGUI_FONT_MANAGER_H__
