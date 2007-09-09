#pragma once

#include <Ogre.h>
#include <string>
#include "Instance.h"
#include "WidgetDefines.h"


namespace widget
{

	class SkinManager
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
		static floatRect convertMaterialCoord(const floatRect & _source, const floatSize & _materialSize);

	private:
		void initialise();
		void createDefault();

	private:
		SkinInfo m_skins;
		MapFlags m_mapAlign;

	}; // class SkinManager

} // namespace widget