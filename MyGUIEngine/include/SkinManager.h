#ifndef _SKINMANAGER_H_
#define _SKINMANAGER_H_


#include "Prerequest.h"
#include <Ogre.h>
#include <string>
#include "Instance.h"
#include "WidgetDefines.h"


namespace MyGUI
{

	class _MyGUIExport SkinManager
	{
	private:
		SkinManager();
		~SkinManager();

	public:
		INSTANCE(SkinManager)

		char parseAlign(const std::string & _value);
		WidgetSkinInfo * getSkin(const Ogre::String & _name);

		//	для ручного создания скина
		WidgetSkinInfo * create(const Ogre::String & _name);

		void load(const std::string & _file);

		static floatSize getMaterialSize(const std::string & _material);
		// конвертирует из пиксельных координат в текстурные, в Rect задано начало и размер
		static floatRect convertMaterialCoord(const floatRect & _source, const floatSize & _materialSize);

	private:
		void initialise();
		void createDefault();

	private:
		SkinInfo m_skins;
		MapFlags m_mapAlign;

	}; // class SkinManager

} // namespace MyGUI

#endif
