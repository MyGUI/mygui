/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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
	template <> const char* Singleton<FontManager>::mClassTypeName = "FontManager";

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
#ifndef MYGUI_DONT_USE_OBSOLETE
		loadOldFontFormat(_node, _file, _version, mXmlFontTagName);
#endif // MYGUI_DONT_USE_OBSOLETE

		xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == mXmlPropertyTagName)
			{
				const std::string& key = node->findAttribute("key");
				const std::string& value = node->findAttribute("value");
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
