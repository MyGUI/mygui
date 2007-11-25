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

	bool FontManager::load(const std::string& _file)
	{
		xml::xmlDocument doc;
		if (false == doc.open(helper::getResourcePath(_file))) return false;

		xml::xmlNodePtr root = doc.getRoot();
		if ( (root == 0) || (root->getName() != "MyGUI") ) return false;

		std::string source;
		if ( (false == root->findAttribute("type", source)) || (source != "font") ) return false;

		xml::xmlNodeIterator font = root->getNodeIterator();

		while (font.nextNode("font")) {

			std::string name, size, resolution, antialias, space, tab;
			if (false == font->findAttribute("name", name)) continue;
			if (false == font->findAttribute("source", source)) continue;
			if (false == font->findAttribute("size", size)) continue;
			if (false == font->findAttribute("resolution", resolution)) continue;

			font->findAttribute("antialias_colour", antialias);
			font->findAttribute("space_width", space);
			font->findAttribute("tab_count", tab);

			FontPtr pFont = create(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			pFont->_notifyOrigin(_file);
			pFont->setSource(source);
			pFont->setTrueTypeSize(util::parseValue<Ogre::Real>(size));
			pFont->setTrueTypeResolution(util::parseValue<Ogre::Real>(resolution));

			if (false == antialias.empty()) pFont->setAntialiasColour(util::parseBool(antialias));
			if (false == space.empty()) pFont->setSpaceSimbol(space[0]);
			if (false == tab.empty()) pFont->setCountSpaceTab(util::parseUChar(tab));

			xml::xmlNodeIterator range = font->getNodeIterator();

			while (range.nextNode("code")) {
				std::string range_value;
				if (false == range->findAttribute("range", range_value)) continue;
				pFont->addCodePointRange(util::parseValueEx2<Font::CodePointRange, Font::CodePoint>(range_value));
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