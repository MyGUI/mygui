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

	void FontManager::saveFontTexture(const std::string & _font, const std::string & _file)
	{
		FontPtr font = FontManager::getInstance().getByName( _font );
		MYGUI_ASSERT( ! font.isNull(), "Could not find font " << _font);

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

	/*Ogre::uchar * buff = (Ogre::uchar *)texture->getBuffer()->lock(0, texture->getWidth() * texture->getHeight() * 2, Ogre::HardwareBuffer::HBL_READ_ONLY);

		Ogre::ImageCodec::ImageData *imgData = new Ogre::ImageCodec::ImageData();
		imgData->width = texture->getWidth();
		imgData->height = texture->getHeight();
		imgData->depth = 1;
		imgData->format = Ogre::PF_A8R8G8B8;

		// Allocate buffer 
		Ogre::uchar* pBuffer = new Ogre::uchar[texture->getWidth() * texture->getHeight() * 4];

		for (size_t y=0; y < texture->getHeight(); ++y) {
			for (size_t x=0; x < texture->getWidth(); ++x) {
				*pBuffer = *buff;
				pBuffer ++;
				*pBuffer = *buff;
				pBuffer ++;
				*pBuffer = *buff;
				pBuffer ++;
				buff ++;

				*pBuffer = *buff;
				pBuffer ++;
				buff ++;
			}
		}

		//texture->getBuffer()->unlock();

		// Wrap buffer in a memory stream
		//Ogre::DataStreamPtr stream(new Ogre::MemoryDataStream(pBuffer, texture->getWidth() * texture->getHeight() * 4, false));

		// Need to flip the read data over in Y though
		//Ogre::Image img;
		//img.loadRawData(stream, texture->getWidth(), texture->getHeight(), imgData->format );
		//img.flipAroundX();

		Ogre::MemoryDataStreamPtr stream(new Ogre::MemoryDataStream(pBuffer, texture->getWidth() * texture->getHeight() * 4, false));

		// Get codec 
		size_t pos = _file.find_last_of(".");
		Ogre::String extension;
		MYGUI_ASSERT(pos != Ogre::String::npos, "Unable to determine image type for '" << _file << "' - invalid extension.");

		while( pos != _file.length() - 1 )
			extension += _file[++pos];

		// Get the codec
		Ogre::Codec * pCodec = Ogre::Codec::getCodec(extension);

		// Write out
		Ogre::Codec::CodecDataPtr ptr(imgData);
		pCodec->codeToFile(stream, _file, ptr);

		delete [] pBuffer;*/
	}

} // namespace MyGUI
