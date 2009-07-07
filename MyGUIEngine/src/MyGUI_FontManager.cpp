/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
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
#include "MyGUI_Common.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_ManualFont.h"
#include "MyGUI_TrueTypeFont.h"

namespace MyGUI
{
	const std::string XML_TYPE("Font");
	const std::string MYGUI_DEFAULT_FONT_NAME = "Default";

	MYGUI_INSTANCE_IMPLEMENT(FontManager);

	void FontManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &FontManager::_load);

		FactoryManager::getInstance().registryFactory<ManualFont>(XML_TYPE);
		FactoryManager::getInstance().registryFactory<TrueTypeFont>(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void FontManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		MyGUI::ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		FactoryManager::getInstance().unregistryFactory<ManualFont>(XML_TYPE);
		FactoryManager::getInstance().unregistryFactory<TrueTypeFont>(XML_TYPE);

		clear();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool FontManager::load(const std::string& _file/*, const std::string& _group*/)
	{
		return MyGUI::ResourceManager::getInstance()._loadImplement(_file, /*_group, */true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void FontManager::_load(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		xml::ElementEnumerator font = _node->getElementEnumerator();
		while (font.next(XML_TYPE))
		{
			std::string name;
			if (false == font->findAttribute("name", name)) continue;

			std::string type = font->findAttribute("type");
			if (type.empty())
			{
				if (font->findAttribute("resolution").empty()) type = "ManualFont";
				else type = "TrueTypeFont";
			}

			IObject* object = FactoryManager::getInstance().createObject(XML_TYPE, type);
			if (object != nullptr)
			{
				IFont* data = object->castType<IFont>();
				data->deserialization(font.current(), _version);

				mResources[name] = data;
			}
		};
	}

	/*IFont* FontManager::getByName(const std::string& _name)
	{
		MapFont::const_iterator item = mFonts.find(_name);
		if (item == mFonts.end())
		{
			MYGUI_LOG(Error, "Could not find font '" << _name << "', replaced with font '" << MYGUI_DEFAULT_FONT_NAME << "'");
			item = mFonts.find(MYGUI_DEFAULT_FONT_NAME);
			MYGUI_ASSERT(item != mFonts.end(), "Resource '" << MYGUI_DEFAULT_FONT_NAME << "' not found");
		}
		return item->second;
	}*/

	/*bool FontManager::isExist(const std::string& _name)
	{
		return mFonts.find(_name) != mFonts.end();
	}*/

	/*void FontManager::clear()
	{
		for (MapFont::iterator item=mFonts.begin(); item!=mFonts.end(); ++item)
		{
			delete item->second;
		}
		mFonts.clear();
	}*/

	/*void FontManager::remove(const std::string& _name)
	{
		MapFont::const_iterator item = mFonts.find(_name);
		if (item != mFonts.end())
		{
			delete item->second;
			mFonts.erase(item->first);
		}
	}*/

} // namespace MyGUI
