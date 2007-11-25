/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_FONT_MANAGER_H__
#define __MYGUI_FONT_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "OgrePrerequisites.h"
#include "OgreSingleton.h"
#include "OgreResourceManager.h"
#include "MyGUI_Font.h"
#include "MyGUI_Instance.h"

namespace MyGUI
{

	class _MyGUIExport FontManager : public Ogre::ResourceManager
	{
		INSTANCE_HEADER(FontManager);

	public:
		void initialise();
		void shutdown();

		bool load(const std::string & _file, bool _resource = true);

    protected:

        /// Internal methods
		Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
			const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
			const Ogre::NameValuePairList* params);

	}; // class _MyGUIExport FontManager : public Ogre::ResourceManager

} // namespace MyGUI

#endif // __MYGUI_FONT_MANAGER_H__