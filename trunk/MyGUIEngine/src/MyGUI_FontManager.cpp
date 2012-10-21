/*!
	@file
	@author		Albert Semenov
	@date		11/2007
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_XmlDocument.h"

#include "MyGUI_ResourceManualFont.h"
#include "MyGUI_ResourceTrueTypeFont.h"

namespace MyGUI
{
	template <> FontManager* Singleton<FontManager>::msInstance = nullptr;
	template <> const char* Singleton<FontManager>::mClassTypeName("FontManager");

	FontManager::FontManager() :
		mIsInitialise(false),
		mXmlFontTagName("Font"),
		mXmlPropertyTagName("Property"),
		mXmlDefaultFontValue("Default")
	{
	}

	void FontManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		ResourceManager::getInstance().registerLoadXmlDelegate(mXmlFontTagName) = newDelegate(this, &FontManager::_load);

		std::string resourceCategory = ResourceManager::getInstance().getCategoryName();
		FactoryManager::getInstance().registerFactory<ResourceManualFont>(resourceCategory);
		FactoryManager::getInstance().registerFactory<ResourceTrueTypeFont>(resourceCategory);

		mDefaultName = "Default";

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void FontManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		MyGUI::ResourceManager::getInstance().unregisterLoadXmlDelegate(mXmlFontTagName);

		std::string resourceCategory = ResourceManager::getInstance().getCategoryName();
		FactoryManager::getInstance().unregisterFactory<ResourceManualFont>(resourceCategory);
		FactoryManager::getInstance().unregisterFactory<ResourceTrueTypeFont>(resourceCategory);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void FontManager::_load(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		xml::ElementEnumerator font = _node->getElementEnumerator();
		while (font.next())
		{
#ifndef MYGUI_DONT_USE_OBSOLETE
			// Совместимость со старым форматом, где шрифт был в теге Font.
			// Сейчас шрифт это ресурс и загружается сразу в ресурс менеджере.
			if (font->getName() == mXmlFontTagName)
			{
				loadOldFontFormat(font.current(), _file, _version);
			}
#endif // MYGUI_DONT_USE_OBSOLETE

			if (font->getName() == mXmlPropertyTagName)
			{
				const std::string& key = font->findAttribute("key");
				const std::string& value = font->findAttribute("value");
#ifdef MYGUI_USE_FREETYPE
				if (key == "Default")
#else
				if (key == "DefaultGenerated")
#endif
					mDefaultName = value;
			}
		}
	}

	void FontManager::setDefaultFont(const std::string& _value)
	{
		mDefaultName = _value;
	}

	IFont* FontManager::getByName(const std::string& _name) const
	{
		IResource* result = nullptr;
		//FIXME для совместимости шрифт может иметь имя Default
		if (!_name.empty() && _name != mXmlDefaultFontValue)
			result = ResourceManager::getInstance().getByName(_name, false);

		if (result == nullptr)
		{
			result = ResourceManager::getInstance().getByName(mDefaultName, false);
			if (!_name.empty() && _name != mXmlDefaultFontValue)
			{
				MYGUI_LOG(Error, "Font '" << _name << "' not found. Replaced with default font.");
			}
		}

		return result ? result->castType<IFont>(false) : nullptr;
	}

	const std::string& FontManager::getDefaultFont() const
	{
		return mDefaultName;
	}

} // namespace MyGUI
