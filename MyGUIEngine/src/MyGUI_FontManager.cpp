/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Font.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_Common.h"
#include "xmlDocument.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(FontManager);

	void FontManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise);
		MYGUI_LOG("* Initialise: ", INSTANCE_TYPE_NAME);

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully initialized");
		mIsInitialise = true;
	}

	void FontManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG("* Shutdown: ", INSTANCE_TYPE_NAME);

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully shutdown");
		mIsInitialise = false;
	}

	bool FontManager::load(const std::string& _file, bool _resource)
	{
		xml::xmlDocument doc;
		if (false == doc.open((_resource ? helper::getResourcePath(_file) : _file).c_str())) {
			MYGUI_ERROR(doc.getLastError());
			return false;
		}

		xml::xmlNodePtr root = doc.getRoot();
		if ( (root == 0) || (root->getName() != "MyGUI") ) {
			MYGUI_ERROR("not find root tag 'MyGUI'");
			return false;
		}

		std::string type;
		if ( (false == root->findAttribute("type", type)) || (type != "Font") ) {
			MYGUI_ERROR("not find root type 'Font'");
			return false;
		}

		xml::xmlNodeIterator font = root->getNodeIterator();

		while (font.nextNode("Font")) {

			std::string source, name, size, resolution, antialias, space, tab, spacer;
			if (false == font->findAttribute("name", name)) continue;
			if (false == font->findAttribute("source", source)) continue;
			if (false == font->findAttribute("size", size)) continue;
			if (false == font->findAttribute("resolution", resolution)) continue;

			font->findAttribute("antialias_colour", antialias);
			font->findAttribute("space_width", space);
			font->findAttribute("tab_count", tab);
			font->findAttribute("spaser", spacer);

			FontPtr pFont = create(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			pFont->_notifyOrigin(_file);
			pFont->setSource(source);
			pFont->setTrueTypeSize(util::parseValue<Ogre::Real>(size));
			pFont->setTrueTypeResolution(util::parseValue<Ogre::Real>(resolution));

			if (false == antialias.empty()) pFont->setAntialiasColour(util::parseBool(antialias));
			if (false == space.empty()) pFont->setSpaceSimbol(space[0]);
			if (false == tab.empty()) pFont->setCountSpaceTab(util::parseUChar(tab));
			if (false == spacer.empty()) pFont->setCharSpacer(util::parseUChar(spacer));

			xml::xmlNodeIterator range = font->getNodeIterator();

			while (range.nextNode("Code")) {
				std::string range_value;
				std::vector<std::string> parse_range;
				// диапазон включений
				if (range->findAttribute("range", range_value)) {
					parse_range = util::split(range_value);
					if (parse_range.size() == 2) pFont->addCodePointRange(util::parseValue<Ogre::Real>(parse_range[0]), util::parseValue<Ogre::Real>(parse_range[1]));
				}
				// диапазон исключений
				else if (range->findAttribute("hide", range_value)) {
					parse_range = util::split(range_value);
					if (parse_range.size() == 2) pFont->addHideCodePointRange(util::parseValue<Ogre::Real>(parse_range[0]), util::parseValue<Ogre::Real>(parse_range[1]));
					else if (parse_range.size() == 1) pFont->addHideCodePointRange(util::parseValue<Ogre::Real>(parse_range[0]), util::parseValue<Ogre::Real>(parse_range[0]));
				}
			};

		};
		
		return true;
	}

	Ogre::Resource* FontManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
		const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader,
        const Ogre::NameValuePairList* params)
	{
		return new Font(this, name, handle, group, isManual, loader);
	}

} // namespace MyGUI