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
#include "MyGUI_Enumerator.h"
#include "MyGUI_Font.h"
#include "MyGUI_Instance.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_ResourceManager.h"

#include <OgreResource.h>
#include <OgreResourceManager.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	typedef HashMap<Ogre::String, Ogre::ResourcePtr> ResourceMap;
	typedef Enumerator<ResourceMap> EnumeratorFontPtr;

	class MYGUI_EXPORT FontManager : public Ogre::ResourceManager
	{
		MYGUI_INSTANCE_HEADER(FontManager);

	public:
		void initialise();
		void shutdown();

		/** Load additional MyGUI *_font.xml file */
		bool load(const std::string & _file, const std::string & _group = MyGUI::ResourceManager::GUIResourceGroupName);
		void _load(xml::ElementPtr _node, const std::string & _file, Version _version);

		/** Save already created font to texture
			@param _font name (for example DejaVuSans.14)
			@param _file name to save (for example font.png)
		*/
		void saveFontTexture(const std::string & _font, const std::string & _file);

		/** Get font resouce */
		virtual Ogre::ResourcePtr getByName(const Ogre::String & _name);
		/** Check is font exist */
		virtual bool resourceExists(const Ogre::String & _name);

		/** Check is font exist */
		bool isExist(const std::string& _name) { return resourceExists(_name); }

		/** Get fonts Enumerator */
		EnumeratorFontPtr getEnumerator() { return EnumeratorFontPtr(mResources); }

    protected:

        /// Internal methods
		Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle,
			const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader,
			const Ogre::NameValuePairList* params);

	}; // class MYGUI_EXPORT FontManager : public Ogre::ResourceManager

} // namespace MyGUI

#endif // __MYGUI_FONT_MANAGER_H__
