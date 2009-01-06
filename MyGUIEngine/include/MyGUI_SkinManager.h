/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_SKIN_MANAGER_H__
#define __MYGUI_SKIN_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_ResourceManager.h"

namespace MyGUI
{

	typedef Enumerator<MapWidgetSkinInfoPtr> EnumeratorSkinPtr;

	class MYGUI_EXPORT SkinManager
	{
		MYGUI_INSTANCE_HEADER(SkinManager);

	public:
		void initialise();
		void shutdown();

		/** Get skin info */
		WidgetSkinInfo * getSkin(const Ogre::String & _name);

		//	для ручного создания скина
		/** Create new skin (used for creating skin in code), if skin with such name already exist - overwrite it */
		WidgetSkinInfo * create(const Ogre::String & _name);

		/** Load additional MyGUI *_skin.xml file */
		bool load(const std::string & _file, const std::string & _group = MyGUI::ResourceManager::GUIResourceGroupName);
		void _load(xml::ElementPtr _node, const std::string & _file, Version _version);

		/** Get texture size in pixels
			@param _texture file name
		*/
		static IntSize getTextureSize(const std::string & _texture);
		// конвертирует из пиксельных координат в текстурные, в Rect задано начало и размер
		/** Convert pixel coordinates to texture UV coordinates */
		static FloatRect convertTextureCoord(const FloatRect & _source, const IntSize & _textureSize);
		/** Check that texture have power of two size */
		static bool isPowerOfTwo(IntSize _size);

		/** Check is skin exist */
		bool isExist(const std::string& _name) { return mSkins.find(_name) != mSkins.end(); }

		/** Get skins Enumerator */
		EnumeratorSkinPtr getEnumerator() { return EnumeratorSkinPtr(mSkins); }

	private:
		void createDefault();

	private:
		MapWidgetSkinInfoPtr mSkins;

	};

} // namespace MyGUI

#endif // __MYGUI_SKIN_MANAGER_H__
