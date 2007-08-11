#pragma once

#include <vector>
#include <Ogre.h>
#include "WidgetDefines.h"


namespace widget
{

	// обязательно указатель, так как при добавлении может перераспределяться память
	typedef std::map<Ogre::String, WidgetSkinInfo*> SkinInfo;

	class SkinManager
	{
	private:
		SkinManager(){};
		~SkinManager()
		{
			for (SkinInfo::iterator iter=m_skins.begin(); iter!=m_skins.end(); iter++) {
				delete iter->second;
			}
			m_skins.clear();
		};

	public:
		static /*const*/SkinManager & getInstance() // потом добавить конст
		{
			static SkinManager instance;
			return instance;
		}

		WidgetSkinInfo * getSkin(const Ogre::String & _name)
		{
			SkinInfo::iterator iter = m_skins.find(_name);
			if (iter == m_skins.end()) {
				// потом возвращать дефолтный скин
				assert(0 && "skin is not find");
			}
			return iter->second;
		}

		//	private:
		WidgetSkinInfo * create(const Ogre::String & _name)
		{
			WidgetSkinInfo * skin = new WidgetSkinInfo();
			m_skins[_name] = skin;
			return skin;
		}

	private:
		SkinInfo m_skins;

	}; // class SkinManager

} // namespace widget