/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*//*
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

#include <OgreImageCodec.h>
#include <OgreFont.h>

#include "MyGUI_LastHeader.h"

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
		while (font.next(XML_TYPE)) {

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

			FontPtr pFont = create(name, MyGUI::ResourceManager::getInstance().getResourceGroup());
			pFont->_notifyOrigin(_file);
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

			while (range.next("Code")) {
				std::string range_value;
				std::vector<std::string> parse_range;
				// диапазон включений
				if (range->findAttribute("range", range_value)) {
					parse_range = utility::split(range_value);
					if (!parse_range.empty()) {
						int first = utility::parseInt(parse_range[0]);
						int last = parse_range.size() > 1 ? utility::parseInt(parse_range[1]) : first;
						pFont->addCodePointRange(first, last);
					}
				}
				// диапазон исключений
				else if (range->findAttribute("hide", range_value)) {
					parse_range = utility::split(range_value);
					if (!parse_range.empty()) {
						int first = utility::parseInt(parse_range[0]);
						int last = parse_range.size() > 1 ? utility::parseInt(parse_range[1]) : first;
						pFont->addHideCodePointRange(first, last);
					}
				}
				// описане глифов
				else if (range->findAttribute("index", range_value)) {
					pFont->addGlyph(utility::parseUInt(range_value), utility::parseValue<IntCoord>(range->findAttribute("coord")));
				}

			};

			// инициализируем
			pFont->initialise();

		};
	}

	Ogre::Resource* FontManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle,
		const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader,
        const Ogre::NameValuePairList* params)
	{
		return new Font(this, name, handle, group, isManual, loader);
	}

	void FontManager::saveFontTexture(const std::string & _font, const std::string & _file)
	{
		FontPtr font = getByName( _font );
		MYGUI_ASSERT( ! font.isNull(), "Could not find font '" << _font << "'");

		font->load();
		Ogre::TexturePtr texture = font->getTextureFont();

		Ogre::HardwarePixelBufferSharedPtr readbuffer;
		readbuffer = texture->getBuffer(0, 0);
		readbuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL );
		const Ogre::PixelBox &readrefpb = readbuffer->getCurrentLock();
		Ogre::uchar *readrefdata = static_cast<Ogre::uchar*>(readrefpb.data);

		Ogre::Image img;
		img = img.loadDynamicImage(readrefdata, texture->getWidth(), texture->getHeight(), texture->getFormat());
		img.save(_file);

		readbuffer->unlock();
	}

	Ogre::ResourcePtr FontManager::getByName(const Ogre::String & _name)
	{
		Ogre::ResourcePtr font = Ogre::ResourceManager::getByName( _name );
		if (font.isNull()) {
			MYGUI_LOG(Error, "Could not find font '" << _name << "', replaced with font '" << MYGUI_DEFAULT_FONT_NAME << "'");
			return Ogre::ResourceManager::getByName( MYGUI_DEFAULT_FONT_NAME );
		}
		return font;
	}

	bool FontManager::resourceExists(const Ogre::String & _name)
	{
		return !Ogre::ResourceManager::getByName(_name).isNull();
	}

} // namespace MyGUI
