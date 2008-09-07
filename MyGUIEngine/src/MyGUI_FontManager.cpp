/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Common.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_Font.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_XmlDocument.h"
#include <OgreImageCodec.h>
#include <OgreFont.h>

namespace MyGUI
{
	const std::string XML_TYPE("Font");

	INSTANCE_IMPLEMENT(FontManager);

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

	void FontManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		xml::xmlNodeIterator font = _node->getNodeIterator();
		while (font.nextNode(XML_TYPE)) {

			std::string source, name, size, resolution, antialias, space, tab, distance, cursor, offsetH;
			if (false == font->findAttribute("name", name)) continue;
			if (false == font->findAttribute("source", source)) continue;
			if (false == font->findAttribute("size", size)) continue;
			if (false == font->findAttribute("resolution", resolution)) continue;

			font->findAttribute("antialias_colour", antialias);
			font->findAttribute("space_width", space);
			font->findAttribute("tab_width", tab);
			font->findAttribute("cursor_width", cursor);
			font->findAttribute("distance", distance);
			font->findAttribute("offset_height", offsetH);

			FontPtr pFont = create(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			pFont->_notifyOrigin(_file);
			pFont->setSource(source);
			pFont->setTrueTypeSize(utility::parseValue<Ogre::Real>(size));
			pFont->setTrueTypeResolution((Ogre::uint)utility::parseValue<Ogre::Real>(resolution));

			pFont->setDefaultHeight(utility::parseUInt(font->findAttribute("default_height")));
			if (false == antialias.empty()) pFont->setAntialiasColour(utility::parseBool(antialias));
			if (false == space.empty()) pFont->setSpaceWidth(utility::parseUChar(space));
			if (false == tab.empty()) pFont->setTabWidth(utility::parseUChar(tab));
			if (false == cursor.empty()) pFont->setCursorWidth(utility::parseUChar(cursor));
			if (false == distance.empty()) pFont->setDistance(utility::parseUChar(distance));
			if (false == offsetH.empty()) pFont->setOffsetHeight(utility::parseUChar(offsetH));

			xml::xmlNodeIterator range = font->getNodeIterator();

			while (range.nextNode("Code")) {
				std::string range_value;
				std::vector<std::string> parse_range;
				// диапазон включений
				if (range->findAttribute("range", range_value)) {
					parse_range = utility::split(range_value);
					if (parse_range.size() == 2) pFont->addCodePointRange(utility::parseValue<Ogre::Real>(parse_range[0]), utility::parseValue<Ogre::Real>(parse_range[1]));
				}
				// диапазон исключений
				else if (range->findAttribute("hide", range_value)) {
					parse_range = utility::split(range_value);
					if (parse_range.size() == 2) pFont->addHideCodePointRange(utility::parseValue<Ogre::Real>(parse_range[0]), utility::parseValue<Ogre::Real>(parse_range[1]));
					else if (parse_range.size() == 1) pFont->addHideCodePointRange(utility::parseValue<Ogre::Real>(parse_range[0]), utility::parseValue<Ogre::Real>(parse_range[0]));
				}
			};
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
		FontPtr font = FontManager::getInstance().getByName( _font );
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

} // namespace MyGUI
