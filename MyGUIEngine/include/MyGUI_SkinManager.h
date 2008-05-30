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
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{

	class _MyGUIExport SkinManager
	{
		INSTANCE_HEADER(SkinManager);

	public:
		typedef std::map<std::string, Align> MapAlign;

	public:
		void initialise();
		void shutdown();

		/** Parse align string */
		static Align parseAlign(const std::string & _value);

		/** Get skin info */
		WidgetSkinInfo * getSkin(const Ogre::String & _name);

		//	для ручного создания скина
		/** Create new skin (used for creating skin in code), if skin with such name already exist - overwrite it */
		WidgetSkinInfo * create(const Ogre::String & _name);

		/** Load additional MyGUI *.skin file */
		bool load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void _load(xml::xmlNodePtr _node, const std::string & _file);

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
		inline bool isSkinExist(const std::string& _name) { return mSkins.find(_name) != mSkins.end();}

	private:
		void createDefault();

	private:
		MapWidgetSkinInfoPtr mSkins;
		static MapAlign mMapAlign;

	}; // class SkinManager

} // namespace MyGUI

#endif // __MYGUI_SKIN_MANAGER_H__
