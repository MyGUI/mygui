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
#include "MyGUI_Font.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{
	const std::string XML_TYPE("Font");
	const std::string MYGUI_DEFAULT_FONT_NAME = "Default";

	MYGUI_INSTANCE_IMPLEMENT(FontManager);

	void FontManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &FontManager::_load);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void FontManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		MyGUI::ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		clear();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool FontManager::load(const std::string & _file, const std::string & _group)
	{
		return MyGUI::ResourceManager::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void FontManager::_load(xml::ElementPtr _node, const std::string & _file, Version _version)
	{
		xml::ElementEnumerator font = _node->getElementEnumerator();
		while (font.next(XML_TYPE))
		{
			std::string source, name, size, resolution, antialias, space, tab, distance, cursor, offsetH;
			if (false == font->findAttribute("name", name)) continue;
			if (false == font->findAttribute("source", source)) continue;

			font->findAttribute("size", size);
			font->findAttribute("resolution", resolution);
			font->findAttribute("antialias_colour", antialias);
			font->findAttribute("space_width", space);
			font->findAttribute("tab_width", tab);
			font->findAttribute("cursor_width", cursor);
			font->findAttribute("distance", distance);
			font->findAttribute("offset_height", offsetH);

			Font* pFont = create(name);
			pFont->setSource(source);

			if (!size.empty()) pFont->setTrueTypeSize(utility::parseFloat(size));
			if (!resolution.empty()) pFont->setTrueTypeResolution(utility::parseUInt(resolution));
			pFont->setDefaultHeight(utility::parseInt(font->findAttribute("default_height")));

			if (false == antialias.empty()) pFont->setAntialiasColour(utility::parseBool(antialias));
			if (false == space.empty()) pFont->setSpaceWidth(utility::parseInt(space));
			if (false == tab.empty()) pFont->setTabWidth(utility::parseInt(tab));
			if (false == cursor.empty()) pFont->setCursorWidth(utility::parseInt(cursor));
			if (false == distance.empty()) pFont->setDistance(utility::parseInt(distance));
			if (false == offsetH.empty()) pFont->setOffsetHeight(utility::parseInt(offsetH));

			xml::ElementEnumerator range = font->getElementEnumerator();

			while (range.next("Code"))
			{
				std::string range_value;
				std::vector<std::string> parse_range;
				// диапазон включений
				if (range->findAttribute("range", range_value))
				{
					parse_range = utility::split(range_value);
					if (!parse_range.empty())
					{
						int first = utility::parseInt(parse_range[0]);
						int last = parse_range.size() > 1 ? utility::parseInt(parse_range[1]) : first;
						pFont->addCodePointRange(first, last);
					}
				}
				// диапазон исключений
				else if (range->findAttribute("hide", range_value))
				{
					parse_range = utility::split(range_value);
					if (!parse_range.empty())
					{
						int first = utility::parseInt(parse_range[0]);
						int last = parse_range.size() > 1 ? utility::parseInt(parse_range[1]) : first;
						pFont->addHideCodePointRange(first, last);
					}
				}
				// описане глифов
				else if (range->findAttribute("index", range_value))
				{
					pFont->addGlyph(utility::parseUInt(range_value), utility::parseValue<IntCoord>(range->findAttribute("coord")));
				}

			};

			// инициализируем
			pFont->initialise();

		};
	}

	Font* FontManager::getByName(const std::string& _name)
	{
		MapFont::const_iterator item = mFonts.find(_name);
		if (item == mFonts.end())
		{
			MYGUI_LOG(Error, "Could not find font '" << _name << "', replaced with font '" << MYGUI_DEFAULT_FONT_NAME << "'");
			item = mFonts.find(MYGUI_DEFAULT_FONT_NAME);
			MYGUI_ASSERT(item != mFonts.end(), "Resource '" << MYGUI_DEFAULT_FONT_NAME << "' not found");
		}
		return item->second;
	}

	bool FontManager::isExist(const std::string& _name)
	{
		return mFonts.find(_name) != mFonts.end();
	}

	Font* FontManager::create(const std::string& _name)
	{
		MapFont::const_iterator item = mFonts.find(_name);
		MYGUI_ASSERT(item==mFonts.end(), "Resource '" << _name << "' already exist");

		Font* font = new Font(_name);
		mFonts[_name] = font;
		
		return font;
	}

	void FontManager::clear()
	{
		for (MapFont::iterator item=mFonts.begin(); item!=mFonts.end(); ++item)
		{
			delete item->second;
		}
		mFonts.clear();
	}

	void FontManager::remove(const std::string& _name)
	{
		MapFont::const_iterator item = mFonts.find(_name);
		if (item != mFonts.end())
		{
			mFonts.erase(item->first);
		}
	}

} // namespace MyGUI
