#include "MyGUI_OsgImageLoader.h"
#include "MyGUI_Diagnostic.h"
#include "MyGUI_OsgDiagnostic.h"

#include <osg/Image>
#include <osgDB/FileNameUtils>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/Registry>
#include <string_view>

#ifdef OSG_LIBRARY_STATIC
USE_OSGPLUGIN(png)
#endif

namespace MyGUI
{
	namespace
	{

		osg::ref_ptr<osgDB::ReaderWriter> getPngReaderWriter()
		{
			osgDB::Registry* registry = osgDB::Registry::instance();
			// macOS normally aliases png to imageio. Resolve the actual PNG plugin
			// library, then select its reader explicitly: another PNG-capable plugin
			// may already have been registered by the embedding application.
			registry->addFileExtensionAlias("png", "png");
#ifndef OSG_LIBRARY_STATIC
			registry->loadLibrary(registry->createLibraryNameForExtension("png"));
#endif
			for (const auto& reader : registry->getReaderWriterList())
			{
				if (std::string_view(reader->className()) == "PNG Image Reader/Writer")
					return reader;
			}
			MYGUI_PLATFORM_EXCEPT("The OSG PNG plugin (osgdb_png) is required to read and write PNG images");
		}

	}

	osg::ref_ptr<osg::Image> loadOsgImage(const std::string& _filename)
	{
		if (osgDB::getLowerCaseFileExtension(_filename) == "png")
			return getPngReaderWriter()->readImage(_filename, nullptr).getImage();
		return osgDB::readRefImageFile(_filename);
	}

	bool saveOsgImage(const osg::Image& _image, const std::string& _filename)
	{
		// osgdb_png expects bottom-up rows, including when the source contains MyGUI's
		// top-down CPU pixels. Keep the original image untouched for pending draws.
		osg::ref_ptr<osg::Image> image;
		const osg::Image* source = &_image;
		if (_image.getOrigin() == osg::Image::TOP_LEFT)
		{
			image = new osg::Image(_image, osg::CopyOp::DEEP_COPY_ALL);
			image->flipVertical();
			image->setOrigin(osg::Image::BOTTOM_LEFT);
			source = image;
		}
		if (osgDB::getLowerCaseFileExtension(_filename) == "png")
			return getPngReaderWriter()->writeImage(*source, _filename, nullptr).success();
		return osgDB::writeImageFile(*source, _filename);
	}

}
