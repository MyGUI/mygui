/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Font.h"
#include "MyGUI_Gui.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_Common.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{
	const std::string XML_TYPE("Font");

	INSTANCE_IMPLEMENT(FontManager);

	void FontManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &FontManager::_load);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void FontManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool FontManager::load(const std::string & _file, const std::string & _group)
	{
		return Gui::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void FontManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		xml::xmlNodeIterator font = _node->getNodeIterator();
		while (font.nextNode(XML_TYPE)) {

			std::string source, name, size, resolution, antialias, space, tab, spacer;
			if (false == font->findAttribute("name", name)) continue;
			if (false == font->findAttribute("source", source)) continue;
			if (false == font->findAttribute("size", size)) continue;
			if (false == font->findAttribute("resolution", resolution)) continue;

			font->findAttribute("antialias_colour", antialias);
			font->findAttribute("space_width", space);
			font->findAttribute("tab_count", tab);
			font->findAttribute("spacer", spacer);

			FontPtr pFont = create(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			pFont->_notifyOrigin(_file);
			pFont->setSource(source);
			pFont->setTrueTypeSize(utility::parseValue<Ogre::Real>(size));
			pFont->setTrueTypeResolution((Ogre::uint)utility::parseValue<Ogre::Real>(resolution));

			if (false == antialias.empty()) pFont->setAntialiasColour(utility::parseBool(antialias));
			if (false == space.empty()) pFont->setSpaceSimbol(space[0]);
			if (false == tab.empty()) pFont->setCountSpaceTab(utility::parseUChar(tab));
			if (false == spacer.empty()) pFont->setCharSpacer(utility::parseUChar(spacer));

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

} // namespace MyGUI
