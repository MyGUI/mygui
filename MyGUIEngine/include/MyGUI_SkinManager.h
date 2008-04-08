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

		static Align parseAlign(const std::string & _value);
		WidgetSkinInfo * getSkin(const Ogre::String & _name);

		//	для ручного создания скина
		WidgetSkinInfo * create(const Ogre::String & _name);

		bool load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void _load(xml::xmlNodePtr _node, const std::string & _file);

		static FloatSize getTextureSize(const std::string & _texture);
		// конвертирует из пиксельных координат в текстурные, в Rect задано начало и размер
		static FloatRect convertTextureCoord(const FloatRect & _source, const FloatSize & _textureSize);


	private:
		void createDefault();

	private:
		MapWidgetSkinInfoPtr mSkins;
		static MapAlign mMapAlign;

	}; // class SkinManager

} // namespace MyGUI

#endif // __MYGUI_SKIN_MANAGER_H__
